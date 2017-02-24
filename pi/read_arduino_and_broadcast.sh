#!/usr/bin/python

import paho.mqtt.client as paho
import time
import datetime
import serial
import re
ser = serial.Serial('/dev/ttyACM0', 9600)

def on_connect(client, userdata, flags, rc):
    print("Connection returned result: "+ paho.connack_string(rc))

client = paho.Client()
client.username_pw_set("user", "password")
client.on_connect = on_connect
client.connect("127.0.0.1", 5672)
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
	(rc, mid) = client.publish("decibelometre", str(liste), qos = 1)
