# IKEA-Feinstaubsensor Vindrikning mit ESP32 per Bluetooth auf Smartphone anzeigen

IKEA bietet den Feistaubsensor VINDRIKTNING an
[IKEA vindriktning](https://www.ikea.com/de/de/p/vindriktning-luftqualitaetssensor-70498242/)

Der Sensor misst den Feinstaub in der Luft und zeigt die Konzentration mit einer dreifarbigen LED Leiste an.  

Eine Beschreibung des Ikea Sensors sowie die Idee, die Messdaten vom Ikea-Sensor abzugreifen und auf einem Mobiltelefon darzustellen basieren auf den Beiträgen von [heise](https://www.heise.de/ratgeber/Ikea-Feinstaubsensor-Vindriktning-zum-IoT-Device-aufbohren-6164149.html) und [Hypfer](https://github.com/Hypfer/esp8266-vindriktning-particle-sensor).

Zusätzlich wird die Bluetooth Kommunikation zwischen dem ESP32 und einem Android Smartphone beschrieben. Die Messdaten können auf dem Smartphone mit einer Standard App als Zahlen dargestellt werden.  
Alternativ kann eine mit dem MIT App Inventor erstellte App genutzt oder auch angepasst werden, die die Daten in einer einfachen Grafik darstellt.

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

Die Arduino basierte Software 'IKEA_ESP.ino' findet sich unter [Arduino SW](https://github.com/PeterDirnhofer/IKEA-vintrikning-ESP32-Bluetooth/blob/main/IKEA_ESP32.ino)

---

## Handy Software 1: 'Serial Bluetooth Terminal'

Die App kann aus dem Google Play Store geladen werden. Suche nach "serial bluetooth terminal"

---

## Handy Software 2: 'IKEA VIND Monitor'

Die App herunterladen von [IKEA_VIND_Monitor.apk](https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth/blob/main/IKEA_VIND_Monitor.apk) und auf dem Handy installieren.

## Handy Software: 'IKEA VIND Monitor' mit MIT dem APP Inventor bearbeiten

Es gibt auch die Möglichkeit, die App mit dem **MIT App Inventor** zu bearbeiten und an die eigenen Vorstellungen anzupassen.

Eine schöne Einführung in den **MIT App Inventor** gibt es unter [App Inventor Tutorial](https://youtu.be/aM2ktMKAunw)  
Die App als mit dem **MIT App Inventor** bearbeitbare **aia** Datei liegt auf [IKEA_VIND_Monitor.aia](https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth/blob/main/IKEA_VIND_Monitor.aia)
