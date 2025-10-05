# MIT App Inventor BLE - Key Blocks Visualization

## Essential Blocks Overview

### 1. Main Connection Flow
```
[Button1.Click] → [BluetoothLE1.StartScanning]
                      ↓
[BluetoothLE1.DeviceFound] → [Add to ListPicker]
                      ↓
[BluetoothLE1.ScanResult] → [Stop Scanning]
                      ↓
[ListPicker1.AfterPicking] → [BluetoothLE1.Connect]
                      ↓
[BluetoothLE1.Connected] → [DiscoverServices]
                      ↓
[BluetoothLE1.ServicesDiscovered] → [RegisterForCharacteristicNotifications]
```

### 2. Data Reception (Most Important)
```
[BluetoothLE1.CharacteristicChanged]
         ↓
[Check if TX Characteristic UUID matches]
         ↓
[Convert byte array to string]
         ↓
[Check if starts with "#"]
         ↓
[Extract number after "#"]
         ↓
[Update UI Labels]
```

### 3. Critical Block Details

#### Variables to Create:
```
deviceAddress (Text) = ""
serviceUUID (Text) = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  
txCharacteristicUUID (Text) = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
isConnected (Boolean) = false
```

#### Key Method Call:
```
BluetoothLE1.RegisterForCharacteristicNotifications(
    deviceAddress,
    serviceUUID, 
    txCharacteristicUUID
)
```

#### Data Processing:
```
when BluetoothLE1.CharacteristicChanged:
    if characteristic_uuid = txCharacteristicUUID:
        receivedString = BluetoothLE1.StringValue(characteristic_value)
        if starts_at(receivedString, "#"):
            value = select_list_item(split(receivedString, "#"), 2)
            Label2.Text = "Value: " + value
```

## Quick Start Checklist

✅ Add BluetoothLE1 component  
✅ Add scan button and device picker  
✅ Create global variables for UUIDs  
✅ Implement CharacteristicChanged handler  
✅ Register for TX characteristic notifications  
✅ Parse "#123" format data  
✅ Update UI with received values  

## Testing Sequence

1. **Flash ESP32** with SIMULATION = 1
2. **Build MIT App** with blocks above  
3. **Install app** on Android device
4. **Enable Bluetooth** and Location on phone
5. **Press scan** → Select IKEA_BLE_001
6. **Should receive** values 1,2,3,4,5... every 2 seconds

This creates a working BLE receiver app for your ESP32 IKEA sensor project!