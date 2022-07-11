# Self-Signed Case
      
The __Self-Signed__ Case generates necessary Certificates in order to provide __TLS__ Security for components. This case
adds upon the __Basic Auth__ Case by adding more security in addition to basic authentication.

When it comes to self-signed certificates, however, one must always keep in mind:

      Compromised self-signed certificates can pose many security challenges, since attackers can spoof the identity of the victim. 
      Unlike CA-issued certificates, self-signed certificates cannot be revoked. 
      The inability to quickly find and revoke private key associated with a self-signed certificate creates serious risk.


## Environment Variables & Configuration Files

### Self-Signed Certificates

The bash script `generate-certs.sh` uses `openssl` to generate:
1. __Certificate Authority (CA)__ key and certificate
2. keys and certificates for InfluxDB, Mosquitto, Mosquitto Clients for Publishing

This requires subjects which has some hard-coded information in the script.

## Steps to Bring the Stack Up

1. Create a network for your stack:

        docker network create iotstack

2. Encrypting the Passwords for Mosquitto Broker:
    ```bash
        docker run -it --rm -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -U /mosquitto/config/passwd
    ```

    If there is no response from the command, the passwords are encrypted. You can see the encrypted passwords using:

        cat mosquitto/config/passwd

3. Generate Self-Signed Certificates using the script:

        chmod +x generate-certs.sh
        ./generate-certs.sh
    
    a. During initial Creation for __CA__, you will be asked to enter __PEM Passphrase__. You can keep it whatever you want, but you will need it everytime you create a new certificate. For simplicity use __tiguitto__.

    b. This should create the following certificates and keys in the `certs` folder:

                certs/
                ├── ca.crt
                ├── ca.key
                ├── ca.srl
                ├── grafana
                │   ├── ca.crt
                │   ├── grafana-server.crt
                │   ├── grafana-server.csr
                │   └── grafana-server.key
                ├── influxdb
                │   ├── ca.crt
                │   ├── influx-server.crt
                │   ├── influx-server.csr
                │   └── influx-server.key
                └── mqtt
                │   ├── ca.crt
                │   ├── mqtt-client.crt
                │   ├── mqtt-client.csr
                │   ├── mqtt-client.key
                │   ├── mqtt-server.crt
                │   ├── mqtt-server.csr
                │   ├── mqtt-server.key
                └── opc
                    ├── ca.crt
                    ├── opc-client.crt
                    ├── opc-client.csr
                    ├── opc-client.key
                    ├── opc-server.crt
                    ├── opc-server.csr
                    ├── opc-server.key

        NOTE: We copy the `ca.crt` in each component directory in order to keep the mount volumes in the compose file simple.

5. Distribute `mqtt-client.crt` and `mqtt-client.key` to the Sensor Nodes that need to publish information when `require_certificates` is set to `true` in Mosquitto Configuration File

6. change the Ownership for the Grafana Server Certificates using:

    ```bash
        sudo chown -R 472:472 certs/grafana/
    ```

7. Bring the Stack up:

        USER_ID="$(id -u)" GRP_ID="$(id -g)" docker-compose -f docker-compose.selfsigned.yml up -d

8. For your MQTT Clients copy the `ca.crt`, `mqtt-client.crt`, and `mqtt-client.key` and add them to your Apps accordingly.
   
   All files which are needed to setup the ESP32 can be found [here](microfab/hardware/esp/03_self_signed_auth).

## Publishing with MQTT Clients

You will require all devices or Apps that will publish data to the Microfab Broker to have the `ca.crt` on them along with the user `pubclient`. 
The certificate will enable SSL/TLS and the authentication will only allow dedicated devices to publish data to the Broker.

### Typical MQTT Client Configuration

| Conf | Value        |
|------|--------------|
| Host | <IP_Address> |
| Port | 8883         |
| User | `pubclient`  |
| TLS  | `v1.2`       |
| Pass | `microfoo123`   |
| cert | `ca.crt`, `mqtt-client.crt`, `mqtt-client.key`     |


## Test the Broker using Paho-MQTT-Python 

- if `mosquitto.conf` has `require_certificates true` then the following code will work:

- Define the path to you selfsigned certificates.

```python
import ssl
import sys
import paho.mqtt.client as mqtt


BROKER = '192.168.88.100'
PORT = 8884

CA_CERT_FILE = 'path/to/selfsigned/ca.crt'
CERT_FILE = 'path/to/selfsigned/mqtt-client.crt'
KEY_FILE = 'path/to/selfsigned/mqtt-client.key'
TOPIC = 'test/test'

def on_connect(mqttc, obj, flags, rc):
    print("rc: "+str(rc))

def on_message(mqttc, obj, msg):
    print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))

def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print(string)



# Create Client with Websockets transport
mqttc = mqtt.Client('microfab-selfsigned-ws', transport='websockets')

mqttc.tls_set(ca_certs=CA_CERT_FILE,certfile=CERT_FILE,keyfile=KEY_FILE,tls_version=ssl.PROTOCOL_TLSv1_2)
mqttc.tls_insecure_set(True) # for Self-Signed Certificates


mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
# Uncomment to enable debug messages
mqttc.on_log = on_log
mqttc.connect(BROKER, PORT, 60)
mqttc.subscribe(TOPIC, 0)


try:
    mqttc.loop_forever()
except KeyboardInterrupt as e:
    print('CTRL+C Pressed')
    mqttc.loop_stop()
    mqttc.disconnect()
    sys.exit()

```

# Grafana Configuration

- Browser login to https://192.168.88.100:3000

   - admin:microfoo123
   - Create Database  
      - Go to Configuration -> Data Sources
      - Add Data Source -> InfluxDB
      - Define Server URL: http://influxdb:8086
      - in Auth enable
         - Basic auth
         - With credentials
         - Skip TLS Verify
      - In Basic Auth Details define
         - admin:microfoo123
      - In InfluxDb details define
         - Database: edge
      - Safe & test
    - Create a dashboard
      - Go to Create -> Dashboard -> Add new panel
      - Select Data source: InfluxDB
      - FROM autogen mqtt_consumer WHERE topic = test/arduinoClient SELECT field(data_hall)
       