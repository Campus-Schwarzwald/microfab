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
mqttc.username_pw_set("subclient", "microfoo123")

mqttc.connect(HOST, PORT, 60)

mqttc.publish('test/test', 0)

try:
        mqttc.loop_forever()

except KeyboardInterrupt:
        mqttc.loop_stop()
        mqttc.disconnect()
        sys.exit()