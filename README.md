# IKEA-Feinstaubsensor Vintrikning mit ESP32 per Bluetooth abfragen
IKEA bietet den Feistaubsensor VINDRIKTNING an
https://www.ikea.com/de/de/p/vindriktning-luftqualitaetssensor-70498242/     
Der Sensor misst die Luftqualität und zeigt sie mit einer dreifarbigen LED Leiste an.

Die Idee basiert auf den Artikeln von heise.de   
https://www.heise.de/ratgeber/Ikea-Feinstaubsensor-Vindriktning-zum-IoT-Device-aufbohren-6164149.html      
und dem Github Beitrag von Hypfer    https://www.ikea.com/de/de/p/vindriktning-luftqualitaetssensor-70498242/ 

Ein ESP32 wird verwendet um die die Messdaten des Sensors seriell abzugreifen. Der ESP32 sendet die Daten über Bluetooth an ein Android Mobiltelefon.   
Auf dem Telefon werden die Daten entweder mit der Bluetooth Terminal App als Zahlenwerte oder mit einer mit Hilfe des MIT App Inventor erzeugten App grafisch dargestellt. 
