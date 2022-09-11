/*****************************************************************
  Find more information on
  https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth
  
  Story:
  ESP32 reads IKEA Feinstaubsensor VINDRIKNING
  via ESP32 UART2 serial interface
  Data can be monitored on Arduino's Serial Monitor with 115200 Baud

  Additionally, data are sent over Bluetooth and can be monitored on a Android phone
   

  Code is based on
  https://github.com/Hypfer/esp8266-vindriktning-particle-sensor

  
  ***********************************************************************
  Details:
  Communication betwenn IKEA sensor an ESP32
  ESP32 uses UART2 for serial communication. Class name 'ìkeaSerial'
  - 9600 Baud
  - 3.3 Volt on ESP side, 5 Volt on IKEA sensor side. Voltage divider need to protect ESP32 input !!!
  - GPIO16=RX2 to receive data
  - PIO17=TX2  Transmission Tx. Info: Tx2 is not used as no data to be sent to IKEA sensor

  A nice explanition how to use ESP32 UART2: https://youtu.be/GwShqW39jlE

  
  ***********************************************************************
<<<<<<< HEAD
  Additionally ESP32 sends measured data via Bluetooth
||||||| parent of 0fc05c8 (ino comments updated)
  Adittionally ESP32 sends measured data via Bluetooth
=======
  How to diplay Bluetooth data on your Android Phone:
>>>>>>> 0fc05c8 (ino comments updated)
  To see the Bluetooth data on your android phone, install 'Serial Bluetooth Terminal' from the Playstore

  Your phone can also display sensor data in a simple graphic using the ESP32BluetoothApp. You find it under
  https://github.com/PeterDirnhofer/IKEA-vindriktning-ESP32-Bluetooth
  Optionally, you can edit ESPBluetoothApp using the MIT App Inventor.

************************************************************************************************/


/***********************************************************************************************/
/* IF YOU WORK IN A GROUP, CHANGE BT_NAME INDIVIDUALLY TO AVOID CONFLICTS ON BLUETOOTH*/

#define BT_NAME "IKEA_BT_001"  // Individual devicename of ESP32 Bluetooth

/**********************************************************************************************/



#define LED_BUILTIN 2          // Blue LED on ESP to indicate: ESP gets data from IKEA sensor

// *******************************************************************
// Communication with IKEA Sensor via UART2
HardwareSerial ikeaSerial(2);

uint8_t serialRxBuf[20];  // Buffer for read data
uint8_t rxBufIdx = 0;      // Buffer pointer


// ********************************************************************
// Provide data via Bluetooth
// https://youtu.be/aM2ktMKAunw
#include <BluetoothSerial.h>

// init Bluetooth class
BluetoothSerial ESP_BT;

/*******************************************************************/
void clearRxBuf()
{
  // Clear everything for the next message
  memset(serialRxBuf, 0, sizeof(serialRxBuf));
  rxBufIdx = 0;
}

/*************************** Sketch Code setup ************************************/
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(500);           // 500 ms is enough time to swith USB between programming and Serial monitor


  /********** Init serial communication to IKEA vintriktning ******************/
  ikeaSerial.begin(9600);

  if (!ikeaSerial)
  { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareSerial pin configuration, check config");
  }
  else
  {
    Serial.println("+++ UART2 to IKEA sensor initialized");
  }

  /********************** Setup  Bluetooth Connection ****************/
  ESP_BT.begin(BT_NAME); //Name of your Bluetooth interface -> will show up on your phone
  Serial.print("+++ Bluetooth initialized as *** ");
  Serial.print(BT_NAME);
  Serial.println(" ***");


  clearRxBuf();  
  Serial.println("Waiting for sensor ...");
}


/********************** Sketch Code loop ********************/
void loop()
{

  digitalWrite(LED_BUILTIN, LOW);    // Blue LED off during waiting for sensor data
  while (!ikeaSerial.available())    // Wait until IKEA sensor sends data
  {
    ;
  }

  // Data comming from sensor
  digitalWrite(LED_BUILTIN, HIGH); // Blue LED on indicates data are comming from IKEA sensor ...

  // Read one paket sensor data (20 Bytes) and save into 'serialRxBuf'
  while (ikeaSerial.available())
  {
    serialRxBuf[rxBufIdx++] = ikeaSerial.read(); // read data to rxBuf
    // Without this delay, receiving data breaks for reasons that are beyond me
    delay(15);

    if (rxBufIdx > 20)  // To avoid stack overflow. 20 Bytes in one paket
      clearRxBuf();
  }

  // Plausibility Check. First five bytes must be 0x16 0x11 0x0b 0x00 0x00
  // length of dataset must be 20
  bool headerValid =
    serialRxBuf[0] == 0x16 &&
    serialRxBuf[1] == 0x11 &&
    serialRxBuf[2] == 0x0B &&
    serialRxBuf[3] == 0x00 &&
    serialRxBuf[3] == 0x00;

  if (headerValid & (rxBufIdx == 20))
  {
    // Relevant information can be found in Byte 5 and 6 from IKEA sent data
    const int ikeaValue = (serialRxBuf[5] << 8) | serialRxBuf[6];

    // Send data to Serial Monitor
    Serial.print(ikeaValue);
    Serial.println();

    // Send data via Bluetooth as String
    // with leading #
    String sendString="#";
    sendString.concat(String(ikeaValue));
    ESP_BT.println(sendString);

    // For information only:
    // Uncomment, if you want to display one line with complete sensor raw-data from IKEA sensor in Serial Monitor
    /*
      for (int i = 0; i < rxBufIdx; i++)
      {
        Serial.printf("%02x ", serialRxBuf[i]);
      }
      Serial.println();
    */

  }
  clearRxBuf();
}
