import paho.mqtt.client as paho
import time
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)

def on_publish(client, userdata, mid):
    print("")

client = paho.Client()
client.on_publish = on_publish
client.connect("10.33.44.228", 1883)
client.loop_start()

while True:
	decibel = ser.readline()
	(rc, mid) = client.publish("decibelometre", str(decibel), qos = 1)