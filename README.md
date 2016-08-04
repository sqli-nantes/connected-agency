Pour tester la partie avec RPI : 

- Se connecter avec putty (login : pi ; pwd : raspberry) ; L'ip est a déterminer sur le RPI en faisant un ifconfig
- Dans la console : mosquitto_sub -h ip -t "decibelometre" -v
- Puis se placer dans le dossier /home/pi/28Janvier2016 et executer la commande python script.py