/*****************************************************************
  Find more information on:
  https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth

  Story:
  ESP32 reads IKEA Feinstaubsensor VINDRIKTNING
  via UART2 serial interface.
  Data can be monitored on Arduino's Serial Monitor at 115200 Baud.

  Additionally, data is sent over Bluetooth and can be monitored on an Android phone.

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
  Additionally, ESP32 sends measured data via Bluetooth.
  To see Bluetooth data on your Android phone, install
  "Serial Bluetooth Terminal" from the Play Store.

  Your phone can also display sensor data graphically using the IKEA_VIND_Monitor app:
  https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth

  Optionally, you can edit ESPBluetoothApp using MIT App Inventor.

  #define BT_NAME "IKEA_BT_001"  // Unique device name for ESP32 Bluetooth
***********************************************************************/


/***********************************************************************************************
   IF YOU WORK IN A GROUP, CHANGE BT_NAME INDIVIDUALLY TO AVOID BLUETOOTH CONFLICTS!
************************************************************************************************/
#define BT_NAME "IKEA_BT_001"
// #define BT_NAME "IKEA_BT_002"
// #define BT_NAME "IKEA_BT_003"

/***********************************************************************************************/
// Define pins for RX and TX
#define RXD2 16  // GPIO16 as RX
#define TXD2 17  // GPIO17 as TX (not used but must be defined)

#define LED_BUILTIN 2  // Blue LED on ESP32 indicates data is received from IKEA sensor

// UART2 communication with IKEA sensor
HardwareSerial ikeaSerial(2);

// Buffer for received data
uint8_t serialRxBuf[20];
uint8_t rxBufIdx = 0;

// Bluetooth
#include <BluetoothSerial.h>
BluetoothSerial ESP_BT;  // Init Bluetooth class

/*******************************************************************/
void clearRxBuf() {
  memset(serialRxBuf, 0, sizeof(serialRxBuf));
  rxBufIdx = 0;
}

/*************************** Setup ************************************/
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(500);  // Give time to switch USB from programming to Serial Monitor

  // Initialize serial communication with IKEA Vindriktning
  ikeaSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  if (!ikeaSerial) {
    Serial.println("Invalid Serial configuration. Check RX/TX pins.");
  } else {
    Serial.println("+++ UART2 to IKEA sensor initialized");
  }

  // Initialize Bluetooth
  ESP_BT.begin(BT_NAME);
  Serial.print("+++ Bluetooth initialized as *** ");
  Serial.print(BT_NAME);
  Serial.println(" ***");

  clearRxBuf();
  Serial.println("Waiting for sensor ...");
}

/*************************** Loop ************************************/
void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // LED off while waiting

  while (!ikeaSerial.available()) {
    // Wait for data from IKEA sensor
  }

  digitalWrite(LED_BUILTIN, HIGH);  // LED on when data starts arriving

  // Read 20-byte data packet from sensor
  while (ikeaSerial.available()) {
    serialRxBuf[rxBufIdx++] = ikeaSerial.read();
    delay(15);  // Prevent data corruption

    if (rxBufIdx > 20) {
      clearRxBuf();  // Avoid buffer overflow
    }
  }

  // Check header for validity: first 5 bytes must match
  bool headerValid =
    serialRxBuf[0] == 0x16 &&
    serialRxBuf[1] == 0x11 &&
    serialRxBuf[2] == 0x0B &&
    serialRxBuf[3] == 0x00 &&
    serialRxBuf[4] == 0x00;

  if (headerValid && rxBufIdx == 20) {
    // Get PM2.5 value (bytes 5 and 6)
    const int ikeaValue = (serialRxBuf[5] << 8) | serialRxBuf[6];

    // Send value to Serial Monitor
    Serial.println(ikeaValue);

    // Send value via Bluetooth with leading '#'
    String sendString = "#";
    sendString.concat(String(ikeaValue));
    ESP_BT.println(sendString);

    // Debug: print full raw data
    /*
    for (int i = 0; i < rxBufIdx; i++) {
      Serial.printf("%02x ", serialRxBuf[i]);
    }
    Serial.println();
    */
  }

  clearRxBuf();
}
