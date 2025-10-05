# MIT App Inventor BLE Reader für IKEA ESP32 Project

## Überblick
Diese Anleitung zeigt, wie man eine MIT App Inventor App erstellt, die BLE-Daten vom ESP32 IKEA Sensor liest.

## ESP32 BLE Details
- **Device Name:** IKEA_BLE_001
- **Service UUID:** 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
- **TX Characteristic UUID:** 6E400003-B5A3-F393-E0A9-E50E24DCCA9E (ESP32 → App)
- **RX Characteristic UUID:** 6E400002-B5A3-F393-E0A9-E50E24DCCA9E (App → ESP32)
- **Datenformat:** #123 (# gefolgt von Zahlenwert)

## MIT App Inventor Setup

### 1. Components hinzufügen (Designer)

#### Connectivity:
- **BluetoothLE1** (aus Connectivity)

#### User Interface:
- **Button1** (Text: "Scan & Connect")
- **Button2** (Text: "Disconnect") 
- **Label1** (Text: "Status: Nicht verbunden")
- **Label2** (Text: "Sensor Wert: --")
- **Label3** (Text: "Letztes Update: --")
- **ListPicker1** (Text: "Gerät wählen")

#### Layout:
- **VerticalArrangement1** (alle Komponenten hineinziehen)

### 2. Blocks (Code)

#### Variablen definieren:
```
Global deviceAddress (Text) = ""
Global serviceUUID (Text) = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
Global txCharacteristicUUID (Text) = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
Global isConnected (Boolean) = false
```

#### Beim App Start:
```
when Screen1.Initialize
  set Label1.Text to "Status: App gestartet"
  set BluetoothLE1.ConnectionTimeout to 10000
```

#### Scan für BLE Geräte:
```
when Button1.Click
  set Label1.Text to "Scanne nach BLE Geräten..."
  call BluetoothLE1.StartScanning
```

#### Gerät gefunden:
```
when BluetoothLE1.DeviceFound
  if contains(get name, "IKEA_BLE")
  then
    call ListPicker1.Elements.add(join(get name, " - ", get address))
```

#### Scan beendet:
```
when BluetoothLE1.ScanResult
  call BluetoothLE1.StopScanning
  set Label1.Text to join("Scan beendet. ", length of(get devices), " Geräte gefunden")
  set ListPicker1.Elements to get devices
  set ListPicker1.Visible to true
```

#### Gerät auswählen und verbinden:
```
when ListPicker1.AfterPicking
  set deviceAddress to select list item(split(get ListPicker1.Selection, " - "), 2)
  set Label1.Text to join("Verbinde mit ", get deviceAddress)
  call BluetoothLE1.Connect(get deviceAddress)
```

#### Verbindung erfolgreich:
```
when BluetoothLE1.Connected
  set isConnected to true
  set Label1.Text to "Verbunden! Lese Services..."
  call BluetoothLE1.DiscoverServices(get deviceAddress)
```

#### Services entdeckt:
```
when BluetoothLE1.ServicesDiscovered
  set Label1.Text to "Services gefunden. Aktiviere Notifications..."
  call BluetoothLE1.RegisterForCharacteristicNotifications(
    get deviceAddress,
    get serviceUUID,
    get txCharacteristicUUID
  )
```

#### Daten empfangen (wichtigster Block):
```
when BluetoothLE1.CharacteristicChanged
  if and(get isConnected, get characteristic_uuid = get txCharacteristicUUID)
  then
    set receivedData to call BluetoothLE1.StringValue(get characteristic_value)
    if starts at(get receivedData, "#")
    then
      set sensorValue to select list item(split(get receivedData, "#"), 2)
      set Label2.Text to join("Sensor Wert: ", get sensorValue)
      set Label3.Text to join("Update: ", format time(now))
      // Hier können Sie weitere Verarbeitung hinzufügen (Grafik, Speichern, etc.)
```

#### Verbindung trennen:
```
when Button2.Click
  if get isConnected
  then
    call BluetoothLE1.Disconnect(get deviceAddress)
    set isConnected to false
    set Label1.Text to "Getrennt"
    set Label2.Text to "Sensor Wert: --"
```

#### Verbindung verloren:
```
when BluetoothLE1.Disconnected
  set isConnected to false
  set Label1.Text to "Verbindung verloren"
  set Label2.Text to "Sensor Wert: --"
```

### 3. Erweiterte Features (Optional)

#### Grafische Darstellung:
- **Canvas1** hinzufügen für einfache Balkendiagramme
- **Clock1** für automatische Updates

#### Daten speichern:
- **TinyDB1** für lokale Speicherung
- **File1** für CSV Export

#### Notifications:
- **Notifier1** für Warnungen bei hohen Werten

## Wichtige Hinweise

### Berechtigungen (in AndroidManifest):
```xml
<uses-permission android:name="android.permission.BLUETOOTH" />
<uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
```

### Debugging:
1. **Serial Monitor** am ESP32 überwachen
2. **Label** für Debug-Ausgaben verwenden
3. **Notifier** für Fehlermeldungen

### Typische Probleme:
- **Keine Geräte gefunden:** Bluetooth und Standort aktivieren
- **Verbindung fehlschlägt:** ESP32 neu starten
- **Keine Daten:** Characteristic Notifications prüfen

## Testen

1. **ESP32 hochladen** und im Serial Monitor überwachen
2. **App installieren** auf Android Gerät  
3. **Scan starten** und "IKEA_BLE_001" auswählen
4. **Daten empfangen** sollte im 2-Sekunden-Takt erfolgen (Simulation)

## Simulation vs Real Mode

**Simulation (SIMULATION = 1):**
- Sendet Werte 1, 2, 3, 4, 5, 1, 2, 3...
- Alle 2 Sekunden
- Perfekt zum Testen der App

**Real Mode (SIMULATION = 0):**
- Echte Sensordaten vom IKEA VINDRIKTNING
- Unregelmäßige Intervalle
- Realistische PM2.5 Werte

Diese App zeigt die Grundlagen für BLE-Kommunikation mit dem ESP32. Sie können sie nach Ihren Bedürfnissen erweitern!