# PKI Case
      
The __PKI__ Case generates necessary Certificates in order to provide __TLS__ Security for components. This case
adds upon the __Self Signed__ Case by adding more security.


## Environment Variables & Configuration Files

## Steps to Bring the Stack Up

1. Create a network for your stack:

        docker network create iotstack

2. Encrypting the Passwords for Mosquitto Broker:
    ```bash
        docker run -it --rm -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -U /mosquitto/config/passwd
    ```

    If there is no response from the command, the passwords are encrypted. You can see the encrypted passwords using:

        cat mosquitto/config/passwd

4. change the Ownership for the Grafana and Telegraf Certificates using:

    ```bash
        sudo chown -R 472:472 certs/grafana/
        sudo chown -R 888:888 certs/telegraf/
    ```

5. Bring the Stack up:

        docker-compose -f docker-compose.pki.yml up 

6. For your MQTT Clients copy the `ca-chain.cert.pem`, `mqtt-client.cert.pem`, and `mqtt-client.key.pem` and add them to your Apps accordingly.
   
   All files which are needed to setup the ESP32 can be found [here](microfab/hardware/esp/04_pki_auth).

7. Same for OPC: Distribute `opc-server.cert.der` and `opc-server.key.pem` to the Sensor Nodes


### Typical MQTT Client Configuration

| Conf | Value                                                              |
|------|--------------------------------------------------------------------|
| Host | <IP_Address>                                                       |
| Port | 8883                                                               |
| User | `pubclient`                                                        |
| TLS  | `v1.2`                                                             |
| Pass | `microfoo123`                                                      |
| cert | `ca-chain.cert.pem`, `mqtt-client.cert.pem`, `mqtt-client.key.pem` |


## Test the Broker using Paho-MQTT-Python 

- if `mosquitto.conf` has `require_certificates true` then the following code will work:

- Define the path to you selfsigned certificates.

```python
import ssl
import sys
import paho.mqtt.client as mqtt


BROKER = '192.168.88.100'
PORT = 8884

CA_CERT_FILE = 'path/to/pki_cert/ca.crt'
CERT_FILE = 'path/to/pki_cert/mqtt-client.crt'
KEY_FILE = 'path/to/pki_cert/mqtt-client.key'
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

       