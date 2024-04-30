# chaudiere_vueJs


## Architecture matérielle

liste des arduino utilisés : 
- chaudiere
	- pilotage de la chaudiere
	- regulation de chauffage
	- serveur Web principal (chaudiere.local)
	- répertoire : Arduino_chaudiere
	- matériel : wemos ESP8266 pro

- capteur de temperature intérieure
	- matériel : wemos ESP8266
	- mesure de temperature avec un capteur de temperature DHT
	- serveur web pour fournir temperature intérieure (temperatureInt.local)
	- repertoire : Arduino_capteurTemperature
	- variable de platformio.ini 

- capteur de temperature extérieure
	- idem capteur de temperature intérieure


## Architecture logicielle
- chaudiere_VueJs
	- Arduino_capteurTemperature
		- data : repertoire contenant les fichiers pour la mémoire flash (a recuperer dans le projet frontal_vueJs)
			- index.html
			- favicon.ico et favicon.png
			- style.css
			- bundle.js				
			- pageNotFound.html		
		- include : fichiers include *.hpp
		- src ; fichiers source *.cpp
	- Arduino_chaudiere
	- frontal_vueJs
	- Serveur_node_simul_arduino

## utilisation

### avec simulateur (dev)

Pendant la periode de develooppement il est possible d'utiliser l'environnement de dev constitué de :
- frontal vue JS : 

### avec arduino (prod)