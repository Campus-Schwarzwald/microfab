# No Auth Concept

## Steps to bring the  Stack Up

1. Change directory

``` bash
cd /home/pi/microfab/hardware/raspberry/control/01_no_auth
```

2. Create a network for your stack:

``` bash
docker network create iotstack
```

3. Bring the stack up:

``` bash
docker-compose -f docker-compose.no_auth.yml up

# -d to start docker in the background
```

4. All files which are needed to setup the ESP32 can be found [here](/hardware/esp/01_no_auth).

5. For setting up the OPC UA Server find all files [here](/hardware/raspberry/opc_server/01_no_auth).


##  Ports

| Component    | Port                              |
|--------------|-----------------------------------|
| `influxdb`   | 8086 (internal)                   |
| `telegraf`   | n/a (internal)                    |
| `grafana`    | 3000 (internal)                   |
| `mosquitto`  | 1883 (mqtt), 1884 (ws) (internal) |
| `opc-client` | 8000                              |

## Component Level Security

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
PORT = 1883

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

