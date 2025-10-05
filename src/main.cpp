/*****************************************************************
  Find more information on:
  https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth

  Story:
  ESP32 reads IKEA Feinstaubsensor VINDRIKTNING
  via UART2 serial interface.
  Data can be monitored on Arduino's Serial Monitor at 115200 Baud.

  Additionally, data is sent over BLE (Bluetooth Low Energy) and can be monitored on smartphones.

  Code is based on:
  https://github.com/Hypfer/esp8266-vindriktning-particle-sensor

  ***********************************************************************
  Details:
  Communication between IKEA sensor and ESP32:
  - ESP32 uses UART2 (class: ikeaSerial)
  - Baud rate: 9600
  - Voltage: 3.3V on ESP32 side, 5V on IKEA sensor side
    -> Use a voltage divider to protect the ESP32 input!
  - GPIO16 = RX2 (receiving data)
  - GPIO17 = TX2 (not used; no data sent to IKEA sensor)

  A nice explanation of how to use UART2 on the ESP32:
  https://youtu.be/GwShqW39jlE

  ***********************************************************************
  Additionally, ESP32 sends measured data via BLE (Bluetooth Low Energy).
  To see BLE data on your smartphone, you need a BLE scanner app like:
  - "nRF Connect" (Nordic Semiconductor)
  - "BLE Scanner"
  - "LightBlue Explorer" (iOS)

  Your phone can also display sensor data using custom BLE apps that support
  Nordic UART Service (NUS) for serial-like communication over BLE.

  #define BLE_NAME "IKEA_BLE_001"  // Unique device name for ESP32 BLE
***********************************************************************/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/***********************************************************************************************
   CONFIGURATION OPTIONS
************************************************************************************************/
#define SIMULATION 1 // Set to 1 for simulation mode, 0 for real sensor data

/***********************************************************************************************
   IF YOU WORK IN A GROUP, CHANGE BLE_NAME INDIVIDUALLY TO AVOID BLUETOOTH CONFLICTS!
************************************************************************************************/
#define BLE_NAME "IKEA_BLE_001"
// #define BLE_NAME "IKEA_BLE_002"
// #define BLE_NAME "IKEA_BLE_003"

// BLE UUIDs for Nordic UART Service (NUS)
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

/***********************************************************************************************/
// Define pins for RX and TX
#define RXD2 16 // GPIO16 as RX
#define TXD2 17 // GPIO17 as TX (not used but must be defined)

#define SIMULATION 1

// UART2 communication with IKEA sensor
HardwareSerial ikeaSerial(2);

// Buffer for received data
uint8_t serialRxBuf[20];
uint8_t rxBufIdx = 0;

// BLE variables
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Simulation variables
#if SIMULATION == 1
int simulationCounter = 1;
unsigned long lastSimulationTime = 0;
const unsigned long simulationInterval = 2000; // Send data every 2 seconds
#endif

// BLE Server Callbacks
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("BLE Client connected");
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("BLE Client disconnected");
    }
};

// BLE Characteristic Callbacks (for receiving data from client)
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0)
        {
            Serial.print("BLE Received: ");
            for (int i = 0; i < rxValue.length(); i++)
            {
                Serial.print(rxValue[i]);
            }
            Serial.println();
        }
    }
};

/*******************************************************************/
void clearRxBuf()
{
    memset(serialRxBuf, 0, sizeof(serialRxBuf));
    rxBufIdx = 0;
}

/*************************** Setup ************************************/
void setup()
{
    Serial.begin(115200);
    delay(500); // Give time to switch USB from programming to Serial Monitor

#if SIMULATION == 1
    Serial.println("*** SIMULATION MODE ACTIVE ***");
    Serial.println("Sending test values: 1, 2, 3, 4, 5, 1, 2, 3, 4, 5...");
#else
    Serial.println("*** REAL SENSOR MODE ***");
    // Initialize serial communication with IKEA Vindriktning
    ikeaSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

    if (!ikeaSerial)
    {
        Serial.println("Invalid Serial configuration. Check RX/TX pins.");
    }
    else
    {
        Serial.println("+++ UART2 to IKEA sensor initialized");
    }
#endif

    // Initialize BLE
    BLEDevice::init(BLE_NAME);

    // Create BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristic for TX (ESP32 -> Client)
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY);

    pTxCharacteristic->addDescriptor(new BLE2902());

    // Create BLE Characteristic for RX (Client -> ESP32)
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE);

    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();

    Serial.print("+++ BLE initialized as *** ");
    Serial.print(BLE_NAME);
    Serial.println(" ***");
    Serial.println("BLE device is now advertising...");

    clearRxBuf();
    Serial.println("Waiting for sensor ...");
}

/*************************** Loop ************************************/
void loop()
{
    // Handle BLE connection changes
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }

    if (deviceConnected && !oldDeviceConnected)
    {
        oldDeviceConnected = deviceConnected;
    }

#if SIMULATION == 1
    // *** SIMULATION MODE ***

    // Check if it's time to send next simulation value
    if (millis() - lastSimulationTime >= simulationInterval)
    {
        lastSimulationTime = millis();

        // Generate simulation value: 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, ...
        int ikeaValue = simulationCounter;
        simulationCounter++;
        if (simulationCounter > 300)
        {
            simulationCounter = 1;
        }

        // Send value to Serial Monitor
        Serial.print("SIMULATION: ");
        Serial.println(ikeaValue);

        // Send value via BLE with leading '#' (only if client is connected)
        if (deviceConnected)
        {
            String sendString = "#";
            sendString.concat(String(ikeaValue));

            pTxCharacteristic->setValue(sendString.c_str());
            pTxCharacteristic->notify();

            Serial.print("BLE sent: ");
            Serial.println(sendString);
        }
        else
        {
            Serial.println("No BLE client connected");
        }

        delay(100); // Brief delay
    }

    delay(100); // Small delay in simulation mode

#else
    // *** REAL SENSOR MODE ***

    while (!ikeaSerial.available())
    {
        // Wait for data from IKEA sensor
        delay(10); // Small delay to prevent tight loop
    }

    // Read 20-byte data packet from sensor
    while (ikeaSerial.available())
    {
        serialRxBuf[rxBufIdx++] = ikeaSerial.read();
        delay(15); // Prevent data corruption

        if (rxBufIdx > 20)
        {
            clearRxBuf(); // Avoid buffer overflow
        }
    }

    // Check header for validity: first 5 bytes must match
    bool headerValid =
        serialRxBuf[0] == 0x16 &&
        serialRxBuf[1] == 0x11 &&
        serialRxBuf[2] == 0x0B &&
        serialRxBuf[3] == 0x00 &&
        serialRxBuf[4] == 0x00;

    if (headerValid && rxBufIdx == 20)
    {
        // Get PM2.5 value (bytes 5 and 6)
        const int ikeaValue = (serialRxBuf[5] << 8) | serialRxBuf[6];

        // Send value to Serial Monitor
        Serial.println(ikeaValue);

        // Send value via BLE with leading '#' (only if client is connected)
        if (deviceConnected)
        {
            String sendString = "#";
            sendString.concat(String(ikeaValue));

            pTxCharacteristic->setValue(sendString.c_str());
            pTxCharacteristic->notify();

            Serial.print("BLE sent: ");
            Serial.println(sendString);
        }
        else
        {
            Serial.println("No BLE client connected");
        }

        // Debug: print full raw data
        /*
        for (int i = 0; i < rxBufIdx; i++) {
          Serial.printf("%02x ", serialRxBuf[i]);
        }
        Serial.println();
        */
    }

    clearRxBuf();
#endif
}