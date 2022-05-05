import paho.mqtt.client as mqtt
import time
import os

HOST = "192.168.88.100"
PORT = 1884

def on_connect(client, userdata, flags, rc):
	client.subscribe('#')
	client.subscribe('$SYS/#')

def on_message(client, userdata, message):
	print('Topic: %s | QOS: %s  | Message: %s' % (message.topic, message.qos, message.payload))

def main():
	client = mqtt.Client()
	client.on_connect = on_connect
	client.on_message = on_message
	client.connect(HOST, PORT)
	client.loop_start()
	time.sleep(10)
	#client.loop_stop()

if __name__ == "__main__":
	main()