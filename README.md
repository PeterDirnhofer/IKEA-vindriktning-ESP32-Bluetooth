# IKEA-Feinstaubsensor VINDRIKTNING mit ESP32 per Bluetooth auf Smartphone anzeigen

**Modernes PlatformIO Projekt für ESP32 Entwicklung**

IKEA bietet den Feinstaubsensor VINDRIKTNING an
[IKEA vindriktning](https://www.ikea.com/de/de/p/vindriktning-luftqualitaetssensor-70498242/)

Der Sensor misst den Feinstaub in der Luft und zeigt die Konzentration mit einer dreifarbigen LED Leiste an.  

Die Artikel von [heise](https://www.heise.de/ratgeber/Ikea-Feinstaubsensor-Vindriktning-zum-IoT-Device-aufbohren-6164149.html) und [Hypfer](https://github.com/Hypfer/esp8266-vindriktning-particle-sensor) beschreiben die Funktion des Ikea Sensors sowie die Methode, wie die Messdaten über die Serielle Schnittstelle abgegriffen werden können. Diese Artikel sind Grundlage für diesen Beitrag. Dank an die Autoren.

Es wird der ESP32 an Stelle des ESP8266 verwendet. Der ESP32 ermöglicht die Kommunikation mit einem Smartphone per Bluetooth anstelle des WLAN.  
Die Messdaten über Bluetooth können auf dem Smartphone mit einer App "von der Stange" als Zahlen dargestellt werden.  
Alternativ kann eine mit dem **MIT App Inventor** erstellte App genutzt oder auch angepasst werden, die die Daten in einer einfachen Grafik darstellt.

**Dieses Projekt verwendet PlatformIO für moderne ESP32-Entwicklung mit VS Code. Legacy Arduino IDE Support ist optional verfügbar.**

**Das ESP32-Programm kommuniziert über 'Bluetooth Classic' mit Apps ausschliesslich auf Android Smartphones. Das von Apple/IOS geforderte 'Blutooth BLE' wird aktuell nicht unterstützt.** 

---

## Datenfluss

*IKEA SENSOR --> ESP32*  
Der ESP32 empfängt die Daten des Sensors über seine serielle UART Schnittstelle.  
Der ESP32 interpretiert den Datenstrom und extrahiert daraus den Messwert.

*ESP32 --> Bluetooth*  
Der ESP32 sendet den Messwert als Zahlenstring über Bluetooth. (Mit einem führenden # Zeichen um den Start des Datenpakets zu markieren)

*Bluetooth --> Android Handy*  
Es gibt zwei Möglichkeiten, die Bluetooth Messwerte mit dem Handy zu empfangen und darzustellen:  

**Messerte anzeigen mit dem Serial Bluetooth Terminal**  
Die Standard-App aus dem Play Store zeigt nur Zahlenwerte

**Messwerte anzeigen mit IKEA VIND Monitor**  
Die eigens erstellte App zeigt die Messwerte in einer einfachen Grafik

---

## Hardware

Der IKEA Sensor wird mit einem Kreuzschlitz Schraubendreher geöffnet. Die serielle Schnittstelle des ESP32 wird mit dem IKEA Sensor verbunden.  
Dazu werden 3 Drähte an die IKEA Platine gelötet: GND, 5V und REST

Anleitung mit Fotos siehe [Hypfer](https://github.com/Hypfer/esp8266-vindriktning-particle-sensor)

Es werden verbunden  
ESP32-GND mit IKEA-GND  
ESP32-VIN mit IKEA-5V  
ESP32-Rx2 mit IKEA-REST (Über einen Spannungsteiler)

Es empfiehlt sich, den 5 Volt-Pegel des seriellen IKEA-REST Ausganssignals auf den 3,5 Volt Eingangpegel des ESP32-Rx2 abzusenken. Die geschieht mit Hilfe eines Spannungsteilers. 

[Spannungsteiler](https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth/blob/ed034369126b07c1828fe7f08cb3ad05aead517b/images/Schematics.png)

---

## ESP32 Software mit Arduino IDE

**Entwicklung mit Arduino IDE**

1. **Arduino IDE installieren** - Download von https://www.arduino.cc/en/software
2. **ESP32 Board Package installieren:**
   - Arduino IDE öffnen
   - `Datei` → `Voreinstellungen` → `Zusätzliche Boardverwalter-URLs`
   - Diesen Link einfügen: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - `OK` klicken
   - `Werkzeuge` → `Board` → `Boardverwalter`
   - Nach "ESP32" suchen und installieren

3. **Projekt-Datei vorbereiten:**
   
   **Copy-Paste aus main.cpp**
   - Datei `src/main.cpp` in einem Texteditor öffnen
   - Gesamten Inhalt kopieren (Ctrl+A, dann Ctrl+C)
   - Neue `.ino` Datei in Arduino IDE erstellen: `Datei` → `Neu`
   - Inhalt einfügen (Ctrl+V)
   - Die Zeile `#include <Arduino.h>` **löschen** (Arduino IDE braucht das nicht)
   - Datei speichern als `IKEA_ESP32.ino`


4. **Board konfigurieren:**
   - `Werkzeuge` → `Board` → `ESP32 Arduino` → `ESP32 Dev Module`
   - `Werkzeuge` → `Port` → Entsprechenden COM-Port auswählen
   - `Werkzeuge` → `Upload Speed` → `921600` (oder langsamer bei Problemen)

5. **Kompilieren und hochladen:**
   - **Kompilieren:** Ctrl+R oder Häkchen-Symbol
   - **Hochladen:** Ctrl+U oder Pfeil-Symbol
   - **Serial Monitor:** Ctrl+Shift+M (115200 Baud einstellen)

**Bluetooth Device Name ändern (Arduino IDE):**  
Hinweis: Sind in einer Arbeitsgruppe mehrere Nutzer\*innen auf engen Raum, muss - um Bluetooth-Konflikte zu vermeiden - jeder ESP32 seinen individuellen Bluetooth devicename besitzen.  
In der `.ino` Datei die Zeile ändern:  
``#define BT_NAME "IKEA_BT_001"`` zu z.B. ``#define BT_NAME "IKEA_BT_002"``

---

## ESP32 Software mit PlatformIO (Alternative)

**Moderne Entwicklungsumgebung als Alternative**

Für Nutzer, die eine moderne IDE mit erweiterten Features bevorzugen:

1. **VS Code installieren** - Falls noch nicht vorhanden: https://code.visualstudio.com/
2. **PlatformIO Extension installieren** - In VS Code: Extensions → Suche nach "PlatformIO IDE"
3. **Projekt klonen oder herunterladen** - Dieses Repository auf den Computer laden
4. **Projekt öffnen** - Ordner in VS Code öffnen (PlatformIO erkennt automatisch die `platformio.ini`)
5. **Kompilieren** - Ctrl+Alt+B oder PlatformIO Build Button
6. **Hochladen** - Ctrl+Alt+U oder PlatformIO Upload Button  
7. **Serial Monitor** - Ctrl+Alt+S für Datenüberwachung bei 115200 Baud

**Bluetooth Device Name ändern (PlatformIO):**  
In der Datei `src/main.cpp` den Bluetooth devicename 'BT_NAME' individuell pro ESP32 ändern:  
``#define BT_NAME "IKEA_BT_001"`` oder  
``#define BT_NAME "IKEA_BT_002"`` usw.

---

## Handy Software: 'Serial Bluetooth Terminal' - Zahlendarstellung

![Serial Bluetooth Terminal](images/serial_bluetooth_terminal_logo.jpg)

Die App 'Serial Bluetooth Terminal' kann aus dem Google Play Store geladen werden. Im Play Store Suche nach "serial bluetooth terminal"  
Die App zeigt die Messwerte aus dem ESP32 als String (mit führendem #-Zeichen)  

[Mess Bespiel](images/sreenshot_serial_bluetooth_terminal.jpg)

---

## Handy Grafik-App: 'IKEA VIND Monitor.app'

'IKEA VIND Monitor' ist eine App zur einfachen grafischen Darstellung der Messwerte.  

Die App wird auf dem Handy installiert durch Scannen dieses  QR Codes

<img src="images/QR_Download_apk.png" alt="Download apk" width="200">

Die App zeigt Messwerte grafisch in einer einfachen XY Darstellung. Im Beispiel wurden Kerzen ausgeblasen. Zunächst unmittelbar neben dem Sensor, dann im Abstand von 30 Zentimetern.  

![Beispiel Messung mit IKEA VIND Monitor](images/screenshot_vind_monitor_klein.jpg)

---

## Handy Grafik-App: 'IKEA VIND Monitor.aia' bearbeiten

Es gibt die Möglichkeit, die App mit dem **MIT App Inventor** nachträglich zu bearbeiten und an die eigenen Vorstellungen anzupassen.

Eine schöne Einführung in den **MIT App Inventor** und die Installation gibt es unter  
[App Inventor Tutorial](https://youtu.be/aM2ktMKAunw)

Hinweis: Bei mir hat die Registrierung bei der Installation des 'MIT App Inventor' nur mit dem Google Account funktioniert, nicht per email Anmeldung.

---

### IKEA VIND Monitor.aia in 'MIT APP inventor' importieren

Zunächst wird die aia Datei von Github auf den Laptop heruntergeladen.  

Die Datei **IKEA_VIND_Monitor_2025.aia** findet sich im Verzeichnis SmartphoneApp. Mit Rechtsklick auf die Datei kann sie auf den Windows Computer geladen werden.


Den **MIT App Inventor** im Browser starten.  
**Import Projects from My Computer | Durchsuchen** und die gerade heruntergeladene Datei im Downloadfolder auswählen und in den 'MIT App Inventor' importieren.

---

# Hilfe


## Testumgebung: Stimulation des Sensors mit einer Kerze

Um beim Experimentieren den Sensor zu stimulieren kann man Zigarettenrauch vewenden. Es geht aber auch ohne Zigaretten: Der Rauch einer gerade ausgeblasenen Kerze in der Nähe treibt den Sensor locker in den roten Bereich und die Messwerte fallen dann langsam in ein, zwei Minuten wieder ab.

Bei den Versuchen ist auch etwas Geduld gefragt. Der Sensor schickt nur alle paar Sekunden Messdaten und dann gleich mehrere Pakete nacheinander. Die blaue LED am ESP32 zeigt an, wenn der ESP gerade Messdaten vom Sensor empfängt.

---


