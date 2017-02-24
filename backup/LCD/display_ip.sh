#!/usr/bin/python

import time
import socket
import fcntl
import struct
import Adafruit_CharLCD as LCD

# Fonction de recuperation de l'IP
def get_ip_address(ifname):
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	return socket.inet_ntoa(fcntl.ioctl(
		s.fileno(),
		0x8915,   #SIOCGIFADDR
		struct.pack('256s', ifname[:15])
	)[20:24])


ip_addr = "NO IP" 
try:
	ip_addr = get_ip_address('eth0')
except IOError:
	print "eth0 unavailable"
	

# Raspberry Pi pin configuration:
lcd_rs        = 26  # Note this might need to be changed to 21 for older revision Pi's.
lcd_en        = 19 
lcd_d4        = 13 
lcd_d5        = 6 
lcd_d6        = 5 
lcd_d7        = 11
lcd_backlight = 4

# Define LCD column and row size for 16x2 LCD.
lcd_columns = 16
lcd_rows    = 2

# Initialize the LCD using the pins above.
lcd = LCD.Adafruit_CharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7,
                           lcd_columns, lcd_rows, lcd_backlight,True)

lcd.clear()

# Print a two line message
lcd.message('Agence Connectee\n' + ip_addr)

