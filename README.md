Pour tester la partie avec RPI : 

- Se connecter avec putty (10.33.44.152 login : pi ; pwd : raspberry)
- Dans la console : mosquitto_sub -h 10.33.44.152 -t "decibelometre" -v
- Puis se placer dans le dossier /home/pi/28Janvier2016 et executer la commande python script.py