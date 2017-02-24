import paho.mqtt.client as paho
import time
import datetime
import serial
import re
ser = serial.Serial('/dev/ttyACM0', 9600)

def on_publish(client, userdata, mid):
    print("")

client = paho.Client()
client.on_publish = on_publish
client.connect("10.33.44.152", 1883)
client.loop_start()

while True:
	liste = []
	now = datetime.datetime.now()
   	timestamp = time.mktime(now.timetuple())
	decibel = ser.readline()
	regex = re.compile(r'[\n\r\t]')
	db = regex.sub(" ", decibel )
	liste.append(db)
	liste.append(timestamp)
	print(db)
	(rc, mid) = client.publish("decibelometre", str(liste), qos = 1)
