
# Agence Connectée

Le projet est composé de 3 parties :

 * Le capteur de son. Il capte le bruit ambiant et calcule les décibels. L'information est ensuite émise par un signal radio 433Mhz.
 * Le récepteur radio capte les signaux envoyés par l'émetteur decibelmètre. Il transmet ensuite les informations au Raspbery PI par le port série (via USB).
 * Le Raspberry PI qui récupère ces informations et les transmet à un broker MQTT.

# Capteur de son
Matériel :

 * Arduino Nano
 * Arduino Micro
 * Emetteur Radio RF433Mhz : FS1000A
 * Recepteur Radio RF433Mhz : XY-MK-5V
 * Capteur son : Arduino KY-038 Microphone sound sensor module

![capteurs][doc/rf433.jpg]

## Emetteur
Schéma de cablage :
![schéma][doc/emetteur.png]

 Pour l'installation du Sketch, il faut installer la librairie VirtualWire
 https://www.pjrc.com/teensy/td_libs_VirtualWire.html

Le code source est [disponible ici](decibelmettre/emetteur.ino).

## Recepteur
Schéma de cablage
![schéma][doc/recepteur.png]

Le code source est [disponible ici](decibelmettre/recepteur.ino).

# Afficheur LCD
Le rôle de l'afficheur LCD est d'afficher l'adresse IP du système. L'objectif est avant tout de faciliter l'expérience utilisateur : celui-ci n'a pas à se connecter à un écran, ni même lancer les commandes réseaux (type nmap) pour récupérer l'IP du Raspberry PI.

On s'appuie sur un écran LCD 16x2.

Il est connecté directement sur le Raspberry PI par le GPIO. Le potentiometre permet de régler la luminosité de l'écran.

![schéma][doc/diagramme_Rpi_LCD.png]

# Raspberry PI
Le raspberry PI est conencté au récepteur par port série via USB. Il récupère l'information du récepteur, puis envoie les informations à un broker MQTT.

## Installation Raspberry PI - détails

### Installation Raspbian
Télécharger [Raspbian Jessie](https://www.raspberrypi.org/downloads/raspbian/).

Installer en suivant les [instructions officielles selon l'OS](https://www.raspberrypi.org/documentation/installation/installing-images/README.md).

Lors de la première installation, le serveur SSH n'est pas activé. Se connecter via écran/clavier sur le PI en utilisant les identifiants/password par défaut (`pi`/`raspberry`. Attention par défaut le clavier est configuré en qwerty).

## Paramétrage
Activer le ssh en lancant : `sudo raspi-config`, puis ` Advanced Options`, `SSH`.

Renommer le Raspberrypi en lancant `sudo raspi-config`, puis `Advanced Options`, `Hostname`. Le renommer en `connectedagencypi`.

## Installation des outils

Installer l'écosystème python :
```
sudo apt-get install python-dev python-rpi.gpio python-pip
```

puis installer les librairies :
```
sudo pip install paho-mqtt
sudo pip install pyserial
```

## Paramétrage des droits
Si la connexion sur le port Série ne se fait pas, vérifier cette démarche ici : http://playground.arduino.cc/Linux/All#Permission

Rebooter pour prendre en compte les nouveaux droits.

## Installation des scripts
Copier les scripts du répertoire `lcd/` et `pi/` dans le répertoire `/home/pi` directement à la racine.

On a donc :
```
/home/pi/
    read_arduino_and_broadcast.sh
    display_ip.sh
```
Rendre ces scripts exécutables, s'il ne le sont pas.

Renommer le fichier `/etc/rc.local` en `rc.local.original` :

```
sudo cp /etc/rc.local /etc/rc.local.original
```
copier le fichier `rc.local` disponible dans les source `/pi/rc.local` dans `/etc/`.

Rebooter le PI, qui devrait normalement afficher son IP sur le LCD au prochain démarrage.

Les connexions peuvent donc désormais se faire en SSH en se connectant sur cette IP dorénavant.

## Paramétrage du broker

Editer le fichier `/home/pi/read_arduino_and_broadcast.sh`.

Modifier les paramétrages en fonction de l'environnement.

# Développements et débogage

### Vérifier le montage de l'Arduino sur PI

lancer la commande `lsusb`. Un montage Arduino doit apparaitre.
Par exemple :

```
Bus 001 Device 004: ID 2341:8037 Arduino SA
Bus 001 Device 003: ID 0424:ec00 Standard Microsystems Corp. SMSC9512/9514 Fast Ethernet Adapter
Bus 001 Device 002: ID 0424:9514 Standard Microsystems Corp.
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```

Si le périphérique n'est pas reconnu, vérifier avec la commande `dmesg` s'il apparait.

### Connaitre le port Série utilisé par l'Arduino
Normallement, c'est le port `/dev/ttyACM0` qui est utilisé. Si ce n'est pas le cas, il est possible de vérifier le nom du périphérique en utilisant la commande `dmesg` et analyser les logs qui devraient faire apparaitre le device.

Une fois après avoir branché l'arduino sur le port USB, lancer la commande `dmesg` qui devrait faire apparaitre `new USB device connected`...`Product Aduino Micro` et faire apparaitre le port utilisé (`/dev/ttyACM0`).

### Valider la réception sur le port Série
Installer `minicom` (pour valider la connexion Série avec l'arduino).

```
sudo apt-get install minicom
```

Il est possible de vérifier la réception sur le port Série en lançant la commande `minicom -b 9600 -o -D /dev/ttyACM0`.

### Problèmes de seuillage
Le capteur de son n'est pas très fiable.
On remonte des informations sans savoir vraiment s'il s'agit de dB.
Selon l'environnement, Il est judicieux de jouer avec le seuillage de déclenchement.

# Améliorations

## Restitution d'autres informations
Récupérer des informations complémentaires à pas cher :
Raspberry Sense HAT permettrait d'implémenter la restitution sonore, mais également de récupérer la température dans la pièce, (la pression et l'humidité sont également possible)

## Miniaturisation
 Les modules fonctionnent encore en Arduino. L'idée serait de les miniaturiser via les ATTiny85

## Solidifier
Il faudrait préférablement souder les composants et ne plus les avoir sous forme de breadboard.

## Autonomie
* Pas d'alimentation autonome du module emetteur. Il serait intéressant de l'alimenter par batterie.

## Sans fil
Le Rpi est pour l'instant en RJ45. dès qu'il est allumé, il affiche son adresse IP. L'idée serait donc qu'il mettes à dispo un serveur Web de parametrage du Wifi et permet par la suite de se connecter via le WiFI via cette interface Web. S'inspirer des façons de faire des autres objets connectés (Nao, Nest, ChromeCast).

## Paramétrage du broker via interface Web
Proposer une interface Web pour paramétrer la connexion sur le Broker.

## Boitier
Il aurait été intéressant d'avoir un boitier imprimé en 3D!
