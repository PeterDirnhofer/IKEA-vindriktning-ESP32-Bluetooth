# IKEA-Feinstaubsensor VINDRIKTNING mit ESP32 per Bluetooth auf Smartphone anzeigen

IKEA bietet den Feinstaubsensor VINDRIKTNING an
[IKEA vindriktning](https://www.ikea.com/de/de/p/vindriktning-luftqualitaetssensor-70498242/)

Der Sensor misst den Feinstaub in der Luft und zeigt die Konzentration mit einer dreifarbigen LED Leiste an.  

Die Artikel von [heise](https://www.heise.de/ratgeber/Ikea-Feinstaubsensor-Vindriktning-zum-IoT-Device-aufbohren-6164149.html) und [Hypfer](https://github.com/Hypfer/esp8266-vindriktning-particle-sensor) beschreiben die Funktion des Ikea Sensors sowie die Methode, wie die Messdaten über die Serielle Schnittstelle abgegriffen werden können. Diese Artikel sind Grundlage für diesen Beitrag. Dank an die Autoren.

Es wird der ESP32 anstatt des ESP8266 verwendet. Der ESP32 ermöglicht die Kommunikation mit einem Smartphone per Bluetooth anstelle des WLAN.  
Die Messdaten über Bluetooth können auf dem Smartphone mit einer App "von der Stange" als Zahlen dargestellt werden.  
Alternativ kann eine mit dem **MIT App Inventor** erstellte App genutzt oder auch angepasst werden, die die Daten in einer einfachen Grafik darstellt.

---

## Konzept

Der IKEA Vindriktning Sensor wird geöffnet. Es werden drei Drähte angelötet für Versorgungsspannung, Ground und das serielle Signal aus dem Sensor. (Spannungsanpassung des seriellen Signals, siehe unten).

Der ESP32 empfängt die Daten des Sensors über seine serielle UART Schnittstelle. Der ESP32 interpretiert den Datenstrom und extrahiert daraus den Messwert.

Der ESP32 sendet den Messwert als Zahl per Bluetooth an das Handy.

Es gibt zwei Möglichkeiten, die Messwerte auf dem Handy darzustellen:  
**Serial Bluetooth Terminal** aus dem Play Store zeigt Zahlenwerte  
**IKEA VIND Monitor** Android App zeigt die Messwerte in einer einfachen Grafik

---

## Hardware

Der IKEA Sensor wird an die Serielle Schnittstelle des ESP32 angeschlossen.  
So geht's: [heise](https://www.heise.de/ratgeber/Ikea-Feinstaubsensor-Vindriktning-zum-IoT-Device-aufbohren-6164149.html).  
Es empfiehlt sich, den Pegel des seriellen Ikea Ausganssignals (5 Volt) and den Eingang des ESP32 (3,5 Volt) mit Hilfe eines Spannungsteilers anzupassen.

---

## ESP32 Software

Die Arduino basierte ESP32 Software 'IKEA_ESP.ino' findet sich unter [Arduino SW](https://github.com/PeterDirnhofer/IKEA-vintrikning-ESP32-Bluetooth/blob/main/IKEA_ESP32.ino)

Hinweis: Um in Arbeitsgruppen Bluetooth-Konflikte mit anderen Nutzer\*innen zu vermeiden, in 'IKEA_ESP.ino' die Bluetooth-ID 'BT_NAME' individuell  pro Nutzer\*in ändern.  
``#define BT_NAME "IKEA_BT_001"``  
``#define BT_NAME "IKEA_BT_002"`` usw.

---

## Handy Software 1: 'Serial Bluetooth Terminal' - Zahlendarstellung

Die App 'Serial Bluetooth Terminal' kann aus dem Google Play Store geladen werden. Suche nach "serial bluetooth terminal"

---

## Handy Software 2: 'IKEA VIND Monitor' - Grafik

Installation auf dem Handy. So gehts:  
Den Link auf die apk ins Handy kopieren (z.B. per Messenger ans Handy schicken) und am Handy anklicken. Damit startet die Installation.  
Hier ist der Link auf die apk:  
https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth/blob/main/IKEA_VIND_Monitor.apk

## Handy Software: 'IKEA VIND Monitor' mit dem 'MIT APP Inventor' bearbeiten

Es gibt auch die Möglichkeit, die App mit dem **MIT App Inventor** zu bearbeiten und an die eigenen Vorstellungen anzupassen.

Eine schöne Einführung in den **MIT App Inventor** gibt es unter [App Inventor Tutorial](https://youtu.be/aM2ktMKAunw)  
Die App als mit dem **MIT App Inventor** bearbeitbare **aia** Datei liegt auf [IKEA_VIND_Monitor.aia](https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth/blob/main/IKEA_VIND_Monitor.aia)
