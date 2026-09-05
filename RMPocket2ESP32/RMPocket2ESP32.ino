/*
Made by Lucas Vonk

Remix of 'Client_Gamepad' example of the esp32 BLE library 
source: https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/Client_Gamepad/Client_Gamepad.ino


The working of converting the raw bluetooth signals of the controller into usable signals:

The Raw data of the radiomaster pocket is 18 byte using little-endian formatting (0x7FFF = 32767, standard maximum for a 16 bit joystick axis).
so it uses byte pairs, the first one is the low byte and the second is the high byte
   00 09 40 09 40 13 00 31 40 09 40 13 00 09 40 13 00 00
// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17

L y-axis:12/13  L x-axis:8/9  R y-axis:4/5  R x-axis:2/3  L Button:6/7   R Button:16/17
3pos switch L:10/11   3pos switch R:14/15   Left bottom button:1 (state only 0 & 1)

After converting the byte pairs to 16-bit integers the range is 0-32767 (or in reality 19 - 32748),
but for ease of use you want a 0-100% range, so you can do: % = (signal - 19) / 32748     
    or, % = (signal - signal_min_value) / signal_max_value if you wish to calibrate the range first

*/


#include <Arduino.h>
#include <BLEDevice.h>
#include <BLESecurity.h>
#include <SoftwareSerial.h>

SoftwareSerial ATmegaSerial(20,21); // RX, TX

// Use lowercase for the MAC address of the bluetooth device you want to connect to and only connect to to avoid accidental misconnecting
static String targetAddress = "04:b2:47:e7:3d:e6";
// ELRS Joystick, Address: 04:b2:47:e7:3d:e6, appearance: 964, serviceUUID: 00001812-0000-1000-8000-00805f9b34fb 

// HID Service UUID (standard UUID for HID over GATT)
static BLEUUID hidServiceUUID((uint16_t)0x1812);
// HID Report characteristic UUID (used for input/output reports)
static BLEUUID reportCharUUID((uint16_t)0x2A4D);
// HID Report Map characteristic UUID
static BLEUUID reportMapUUID((uint16_t)0x2A4B);

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic *pInputReportCharacteristic = nullptr;
static BLEAdvertisedDevice *myDevice = nullptr;
static BLEClient *pClient = nullptr;

float Rx = 0;
float Ry = 0;
float LB = 0;
float Lx = 0;
float L3 = 0;
float Ly = 0;
float R3 = 0; 
float RB = 0; 

volatile bool newData = false;
unsigned long lastProcessTime = 0;
const int processInterval = 0; // Time between prints

uint8_t *pData;

/*// Callback function to handle gamepad input notifications
// When the controller sends a new signal this function will be ran instantly 
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {

    // print raw data from controller 
    // for (size_t i = 0; i < length; i++) {
    //   Serial.printf("%02X ", pData[i]);
    // }
    // Serial.println(); 

 // Convert the raw bluetooth data into usable signals 
  int16_t Rx_raw = (pData[2] << 8) | pData[1];   // Bytes 2 & 3
    float Rx = ((Rx_raw - 19) / 327.48);
  int16_t Ry_raw = (pData[4] << 8) | pData[3];   // Bytes 4 & 5
    float Ry = ((Ry_raw - 19) / 327.48);
  int16_t LB_raw = (pData[6] << 8) | pData[5];   // Bytes 6 & 7
    float LB = ((LB_raw - 19) / 327.48);
  int16_t Lx_raw = (pData[8] << 8) | pData[7];   // Bytes 8 & 9
    float Lx = ((Lx_raw - 19) / 327.48);
  int16_t L3_raw = (pData[10] << 8) | pData[9];  // Bytes 10 & 11
    float L3 = ((L3_raw - 19) / 327.48);
  int16_t Ly_raw = (pData[12] << 8) | pData[11]; // Bytes 12 & 13
    float Ly = ((Ly_raw - 19) / 327.48);
  int16_t R3_raw = (pData[14] << 8) | pData[13]; // Bytes 14 & 15
    float R3 = ((R3_raw - 19) / 327.48); 
  int16_t RB_raw = (pData[16] << 8) | pData[15]; // Bytes 16 & 17
    float RB = ((RB_raw - 19) / 327.48); 

 // Print said converted signals
  //Serial.printf("Vals: %.2lf, %.2lf, %.2lf, %.2lf\n", Ly, Lx, Ry, Rx);
  Serial.print("Ly:");
  ATmegaSerial.print("Ly:");
  Serial.print(Ly);
  ATmegaSerial.print(Ly);
    //Serial.printf("Ly: %.2lf", Ly);
  Serial.print(",  ");
  ATmegaSerial.print(",  ");
  Serial.print("Lx:");
  ATmegaSerial.print("Lx:");
  Serial.print(Lx);
  ATmegaSerial.println(Lx);
    //Serial.printf("Lx: %.2lf", Lx);
  Serial.print(",  ");
  Serial.print("Ry:");
  Serial.print(Ry);
    //Serial.printf("Ry: %.2lf", Ry);
  Serial.print(",  ");
  Serial.print("Rx:");
  Serial.print(Rx);
    //Serial.printf("Rx: %.2lf\n", Rx);
  Serial.print(",  \t");
  Serial.print("LB:");
  Serial.print(LB);
    //Serial.printf("LB: %.2lf\n", LB);
  Serial.print(",  ");
  Serial.print("RB:");
  Serial.print(RB);
    //Serial.printf("RB: %.2lf\n", RB);
  Serial.print(",  \t");
  Serial.print("L3:");
  Serial.print(L3);
    //Serial.printf("L3: %.2lf\n", L3);
  Serial.print(",");
  Serial.print("R3:");
  Serial.println(R3);
    //Serial.printf("R3: %.2lf\n", R3);   
} */

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  if (length < 17) return;

  int16_t Rx_raw = (pData[2] << 8) | pData[1];
  Rx = ((Rx_raw - 19) / 128.42352941175);
  
  int16_t Ry_raw = (pData[4] << 8) | pData[3];
  Ry = ((Ry_raw - 19) / 128.42352941175);
  
  int16_t LB_raw = (pData[6] << 8) | pData[5];
  LB = ((LB_raw - 19) / 128.42352941175);
  
  int16_t Lx_raw = (pData[8] << 8) | pData[7];
  Lx = ((Lx_raw - 19) / 128.42352941175);
  
  int16_t L3_raw = (pData[10] << 8) | pData[9];
  L3 = ((L3_raw - 19) / 128.42352941175);
  
  int16_t Ly_raw = (pData[12] << 8) | pData[11];
  Ly = ((Ly_raw - 19) / 128.42352941175);
  
  int16_t R3_raw = (pData[14] << 8) | pData[13];
  R3 = ((R3_raw - 19) / 128.42352941175); 
  
  int16_t RB_raw = (pData[16] << 8) | pData[15];
  RB = ((RB_raw - 19) / 128.42352941175); 

  newData = true; 
}

// Client callbacks to hnandle connectio events
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    Serial.println("Connected to gamepad");
  }

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("Disconnected from gamepad");
  }
};

// Function to connect to the gamepad
bool connectToServer() {
  Serial.print("Connecting to gamepad at ");
  Serial.println(myDevice->getAddress().toString().c_str());

  pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the gamepad
  pClient->connect(myDevice);
  Serial.println(" - Connected to server");
  pClient->setMTU(185);  // Set MTU for larger data transfers

  // Obtain a reference to the HID service
  BLERemoteService *pRemoteService = pClient->getService(hidServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.println("Failed to find HID service");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found HID service");

  // Get all characteristics to find input reports
  std::map<std::string, BLERemoteCharacteristic *> *pCharMap = pRemoteService->getCharacteristics();

  // Look for input report characteristics (UUID 0x2A4D)
  for (auto const &entry : *pCharMap) {
    BLERemoteCharacteristic *pChar = entry.second;

    if (pChar->getUUID().equals(reportCharUUID)) {
      // Check if this characteristic has notify property (input report)
      if (pChar->canNotify()) {
        Serial.printf(" - Found input report characteristic (handle: 0x%04X)\n", pChar->getHandle());

        // Try to read Report Reference Descriptor to identify report type and ID
        BLERemoteDescriptor *pReportRefDesc = pChar->getDescriptor(BLEUUID((uint16_t)0x2908));
        if (pReportRefDesc != nullptr) {
          String refValue = pReportRefDesc->readValue();
          if (refValue.length() >= 2) {
            uint8_t reportId = refValue[0];
            uint8_t reportType = refValue[1];
            Serial.printf("   Report ID: %u, Type: %u (1=Input, 2=Output, 3=Feature)\n", reportId, reportType);

            // We want input reports (type = 1)
            if (reportType == 1) {
              pInputReportCharacteristic = pChar;
            }
          }
        } else {
          // No report reference descriptor, assume it's an input report
          pInputReportCharacteristic = pChar;
        }
      }
    }
  }

  if (pInputReportCharacteristic == nullptr) {
    Serial.println("Failed to find input report characteristic");
    pClient->disconnect();
    return false;
  }

  // Subscribe to input report notifications
  Serial.println(" - Subscribing to input report notifications");
  pInputReportCharacteristic->registerForNotify(notifyCallback);

  connected = true;
  Serial.println("Successfully connected and subscribed to gamepad!");

  // Note: Security/encryption will be automatically handled by the BLE stack
  // when the HID device requires it (using "Just Works" pairing).

  return true;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Check if the current device matches your target MAC
    if (advertisedDevice.getAddress().toString() == targetAddress) {
      Serial.printf("Found target device: %s\n", targetAddress.c_str());
      
      // Stop scanning and connect
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

void setup() {
  Serial.begin(115200);
  ATmegaSerial.begin(9600);
  ATmegaSerial.println("ESP32 Controller UART :)");
  Serial.println("\n=== BLE HID Gamepad Client ===");
  Serial.println("Scanning for BLE HID gamepads...\n");

  BLEDevice::init("ESP32-Gamepad-Client");

  // Configure BLE Security for pairing with HID devices
  BLESecurity *pSecurity = new BLESecurity();

  // Set security capabilities and authentication mode
  // HID devices typically use "Just Works" pairing (no MITM) with bonding

  // Set IO capability to NONE for "Just Works" pairing
  pSecurity->setCapability(ESP_IO_CAP_NONE);

  // Bonding, no MITM, secure connections (for "Just Works" pairing)
  pSecurity->setAuthenticationMode(true, false, true);

  // Set security callbacks (using default implementation)
  BLEDevice::setSecurityCallbacks(new BLESecurityCallbacks());

  Serial.println("Security configured: Bonding + Secure Connections\n");

  // Create scanner and set callbacks
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

/* // old loop
void loop() {
  // Connect to gamepad if found
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("\n*** Ready to receive gamepad input ***\n");
    } else {
      Serial.println("Failed to connect to gamepad");
    }
    doConnect = false;
  }

  // Restart scanning if disconnected
  if (!connected && doScan) {
    Serial.println("\nScanning for gamepads...");
    BLEDevice::getScan()->start(5, false);
    delay(1000);
  }

  delay(100);
} */

// new loop
void loop() {
  // Connect to gamepad if found
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("\n*** Ready to receive gamepad input ***\n");
    } else {
      Serial.println("Failed to connect to gamepad");
    }
    doConnect = false;
  }

  // Restart scanning if disconnected
  if (!connected && doScan) {
    Serial.println("\nScanning for gamepads...");
    BLEDevice::getScan()->start(5, false);
    delay(1000);
  }

  // Only print if connected, new data exists, and enough time has passed
  if (connected && newData && (millis() - lastProcessTime >= processInterval)) {
    lastProcessTime = millis(); // Reset timer
    newData = false;            // Reset flag

    // Serial.print("Ly:"); ATmegaSerial.print("Ly:");
    Serial.printf("%.2lf\n", Ly); 
    // ATmegaSerial.printf("%.2lf\n", Ly);
    
    Serial.print(",  "); // ATmegaSerial.print(",  ");
    
    Serial.print("Lx:"); //ATmegaSerial.print("Lx:");
    Serial.print(Lx); //ATmegaSerial.println(Lx);
    
    Serial.print(",  ");
    Serial.print("Ry:"); Serial.print(Ry);
    
    Serial.print(",  ");
    Serial.print("Rx:"); Serial.print(Rx);
    
    Serial.print(",  \t");
    Serial.print("LB:"); Serial.print(LB);
    
    Serial.print(",  ");
    Serial.print("RB:"); Serial.print(RB);
    
    Serial.print(",  \t");
    Serial.print("L3:"); Serial.print(L3);
    
    Serial.print(",");
    Serial.print("R3:"); Serial.println(R3);
  }

  delay(5); 
}
