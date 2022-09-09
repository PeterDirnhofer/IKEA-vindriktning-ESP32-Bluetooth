# IKEA-Feinstaubsensor Vindrikning mit ESP32 per Bluetooth auf Smartphone anzeigen

IKEA bietet den Feistaubsensor VINDRIKTNING an
[IKEA vindriktning](https://www.ikea.com/de/de/p/vindriktning-luftqualitaetssensor-70498242/)

Der Sensor misst die Luftqualität und zeigt sie mit einer dreifarbigen LED Leiste an.  

Eine Beschreibung des Ikea Sensors, sowie die Idee, die Messdaten vom Ikea-Sensor abzugreifen und auf einem Mobiltelefon darzustellen finden sich im Beitrag von [heise](https://www.heise.de/ratgeber/Ikea-Feinstaubsensor-Vindriktning-zum-IoT-Device-aufbohren-6164149.html)

---

## Konzept

Ein ESP32 liest die Daten des Sensors über die serielle Schnittstelle. Der ESP32 interpretiert den Datenstrom und extrahiert daraus den Messwert. Der Messwert wird als Zahl per Bluetooth an das Handy gesendet.

Im Folgenden git es zwei Möglichkeiten, die Messwerte auf einem Android Smartphone darzustellen:  
**Serial Bluetooth Terminal** aus dem Play Store zeigt Zahlenwerte  

**IKEA Vindriktning Monitor** zeigt die Messwerte in einer einfachen Grafik

---

## Hardware

Der IKEA Sensor wird an die Serielle Schnittstelle des ESP32 angeschlossen.  
So geht's: link  

---

## ESP32 Software

Die Arduino basierte Software für den ESP32 findet sich unter [ESP32 Arduino](https://github.com/PeterDirnhofer/IKEA-vintrikning-ESP32-Bluetooth/blob/main/IKEA_ESP32.ino)

---

## Handy Software: Serial Bluetooth Terminal

Die App kann aus dem Play Store geladen werden. Suche nach "serial bluetooth terminal"

---

## Handy Software: IKEA Vindriktning Monitor auf das Handy laden

Die App wurde mit dem **MIT App Inventor** erzeugt. Sie kann einfach auf das Telefon geladen werden.  
Die fertige App herunterladen. So geht's: link

## Handy Software: IKEA Vindriktning Monitor mit MIT APP Inventor bearbeiten

Es gibt aber auch die Möglichkeit, die App mit dem **MIT App Inventor** zu bearbeiten und an die eigenen Vorstellungen anzupassen.

Eine schöne Einführung in den App Inventor gibt es unter [Youtube App Inventor Tutorial
](https://youtu.be/aM2ktMKAunw)  
Die App als apk Datei liegt auf link
