# Basic Auth Concept

The use of passwords is still widespread today. 
However, recent events show that the sole use of user:password carries high risks. Such as:

1. **Social Engineering** attacks such as phishing credentials using fake pages, voice phishing (so-called Vishing), shoulder surfing (e.g. peeping behind a person who is typing their password on a laptop) and even retrieving handwritten passwords from post-it notes.
2. **Stealing** using specialized software or physical keyloggers. Some of these attacks require a physical presence or proximity to a laptop or a device.
3. By **intercepting communications**, using fake access points or by leveraging man-in-the-middle attacks (MiTM) at a network level, more prevalent in public WiFis found in hotels, cafés, airports, etc.
4. **Brute-force** attacks on passwords by trying all the combinations, dictionary attacks or by simply guessing the password.
5. Retrieving passwords directly from **data breaches** and leveraging them using password spraying techniques to other legitimate services.

[[Enisa](https://www.enisa.europa.eu/news/enisa-news/tips-for-secure-user-authentication)]

## Steps to Bring the Basic Authentication Stack Up

1. Create a network for your stack:

        docker network create iotstack

2. Create and encrypt the passwords for Mosquitto broker:

   ``` bash
   docker run -it --rm -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -H sha512 -b /mosquitto/config/passwd pubclient microfoo123  
   ```
   
   ``` bash
   docker run -it --rm -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -H sha512 -b /mosquitto/config/passwd subclient microfoo123  
   ```

> There seems to be an issue with hashing with **sha512-pbkdf2**. Therefore we need to take **sha512**   

2. DEPRECATED: Encrypting the Passwords for Mosquitto Broker (Due to the sha512-pbkdf2 issue not working):

    ```bash
    docker run -it --rm -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -U /mosquitto/config/passwd
    ```

    If there is no response from the command the passwords are encrypted. You can see the encrypted passwords using:

        cat mosquitto/config/passwd

3. Bring the stack up:

        USER_ID="$(id -u)" GRP_ID="$(id -g)" docker-compose -f docker-compose.basic_auth.yml up -d

4. All files which are needed to setup the ESP32 can be found [here](/esp/02_basic_auth).



##  Ports

| Component    | Port                              |
|--------------|-----------------------------------|
| `influxdb`   | 8086 (internal)                   |
| `telegraf`   | n/a (internal)                    |
| `grafana`    | 3000 (internal)                   |
| `mosquitto`  | 1883 (mqtt), 1884 (ws) (internal) |
| `opc-client` | 8000                              |

## Component Level Security

### Mosquitto MQTT Broker

The `mosquitto/config/passwd` file has two users in it:


|   username  |  password  |                         role                         |
|:-----------:|:----------:|:----------------------------------------------------:|
| `pubclient` | `microfoo123` | Publishing Data to MQTT Broker. For IoT Sensor Nodes |
| `subclient` | `microfoo123` |       Subscribing to MQTT Broker. For Telegraf       |

The file needs to be encrypted in order for the Broker to accept it. Passwords in Mosquitto cannot be plain-text.

See __Step 2__ to encrypt your Mosquitto Broker Passwords.

### Telegraf

The configuration file (`telegraf.toml`) will use the following environment variables to write data into
InfluxDB

    INFLUXDB_USER=microfab
    INFLUXDB_USER_PASSWORD=microfoo123

The data will be written to a database called `edge` (`INFLUXDB_DB` in `prototype.env`)

Telegraf will use the following environment variables to subscribe to Mosquitto Broker

    TG_MOSQUITTO_USERNAME=subclient
    TG_MOSQUITTO_PASSWORD=microfoo123


### InfluxDB

- You can control the admin user and password via `INFLUXDB_ADMIN_USER` and `INFLUXDB_ADMIN_PASSWORD` variables in `basic_auth.env`
> `INFLUXDB_USER` can _have read and write privileges ONLY if_ `INFLUXDB_DB` is assigned. If there is no database assigned then the `INFLUXDB_USER` will not have any privileges.


### Grafana
Grafana container will use the following environment variables to set up an admin account

You can access grafana by the following URL: http://192.168.88.100:3000/grafana

    GF_ADMIN_USERNAME=admin
    GF_ADMIN_PASSWORD=microfoo123


## Mosquitto Websocket Client using Paho-MQTT-Python

```python
import paho.mqtt.client as mqtt
import sys
HOST = '192.168.88.100'
PORT = 1884

CLIENT_ID='microfab_client'

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

mqttc = mqtt.Client(CLIENT_ID, transport="websockets")


mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
mqttc.on_log = on_log

mqttc.connect(HOST, PORT, 60)

mqttc.subscribe('test/test', 0)

try:
        mqttc.loop_forever()

except KeyboardInterrupt:
        mqttc.loop_stop()
        mqttc.disconnect()
        sys.exit()
```

