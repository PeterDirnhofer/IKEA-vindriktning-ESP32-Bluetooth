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
So geht's: 

#FOTO EINFÜGEN

Es empfiehlt sich, den Pegel des seriellen Ikea Ausganssignals (5 Volt) and den Eingang des ESP32 (3,5 Volt) mit Hilfe eines Spannungsteilers anzupassen.

---

## ESP32 Software

Die Arduino basierte ESP32 Software 'IKEA_ESP.ino' findet sich unter [Arduino SW](https://github.com/PeterDirnhofer/IKEA-vintrikning-ESP32-Bluetooth/blob/main/IKEA_ESP32.ino)

Hinweis: Um in Arbeitsgruppen Bluetooth-Konflikte mit benachbarten Nutzer\*innen zu vermeiden, in 'IKEA_ESP.ino' den Bluetooth devicename 'BT_NAME' individuell  pro device ändern.  
``#define BT_NAME "IKEA_BT_001"``  
``#define BT_NAME "IKEA_BT_002"`` usw.

---

## Handy Software: 'Serial Bluetooth Terminal' - Zahlendarstellung

Die App 'Serial Bluetooth Terminal' kann aus dem Google Play Store geladen werden. Im Play Store Suche nach "serial bluetooth terminal"

---

## Handy Grafik-App: 'IKEA VIND Monitor.apk' installieren

Installation auf dem Handy. So gehts:  
Den Link auf die apk im Handy anklicken. Damit startet die Installation.  

Hinweis: Der Link kann bei Bedarf einfach vom PC aufs Handy kopiert werden z.B. mit dem messenger.

Hier ist der Link zum starten der Installation auf dem Handy:
<https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth/blob/main/IKEA_VIND_Monitor.apk>

---

## Handy Grafik-App: 'IKEA VIND Monitor.aia' bearbeiten

Es gibt die Möglichkeit, die App mit dem **MIT App Inventor** zu bearbeiten und an die eigenen Vorstellungen anzupassen.

Eine schöne Einführung in den **MIT App Inventor** und die Installation gibt es unter  
[App Inventor Tutorial](https://youtu.be/aM2ktMKAunw)

Hinweis: Bei mir hat die Registrierung bei der Installation des 'MIT App Inventor' nur mit dem Google Account funktioniert, nicht per email Anmeldung.

---

### IKEA VIND Monitor.aia in 'MIT APP inventor' importieren

Um die aia Datei zu laden wird zunächst das komplette Github Repository als zip runtergeladen und danach auf das lokale Laufwerk entpackt. Dazu in Github 'Code' und 'Download ZIP' klicken und anschliessend den Download auf das lokale Laufwerk unzippen.  

Die unzipped Datei 'IKEA_VIND_Monitor.aia' kann dann in de **MIT APP Inventor** importiert und weiterbearbeitet werden.

Download ZIP repository von  
[GitHub Repository](https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth.git)

## Testumgebung: Stimulation des Sensors mit einer Kerze

Um beim Experimentieren den Sensor zu stimulieren kann man Zigarettenrauch vewenden. Es geht aber auch ohne Zigaretten: Der Rauch einer gerade ausgeblasenen Kerze in der Nähe treibt den Sensor locker in den roten Bereich und die Messwerte fallen dann langsam in ein, zwei Minuten wieder ab.

Bei den Versuchen ist auch etwas Geduld gefragt. Der Sensor schickt nur alle paar Sekunden Messdaten und dann gleich mehrere Pakete nacheinander. Die blaue LED am ESP32 zeigt an, wenn der ESP gerade Messdaten vom Sensor empfängt.
