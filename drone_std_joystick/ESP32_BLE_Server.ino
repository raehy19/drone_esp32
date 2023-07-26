/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/
#include "sys/time.h"

#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEBeacon.h>
#include <BLECharacteristic.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define BEACON_UUID           "8ec76ea3-6668-48da-9866-75be8bc86f4d" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
#define SERVICE_UUID          "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define IN_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define OUT_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26fa"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharIn;
BLECharacteristic *pCharOut;
BLEAdvertising *pAdvertising;

bool isConnected = false;

/**
 * Characteristic event callback
 */
class ServerEventCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Connected!");
    isConnected = true;
  }
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Disconnected!");
    isConnected = false;
  }
}; // ServerEventCallback

/**
 * Characteristic event callback
 */
class InCharEventCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pChar) {
//    Serial.print("InCharEventCallback - onWrite: ");
//    Serial.println(pChar->getValue().c_str());

    static uint8_t joystickPacket[7] = {'$', 'M', '<', 50, 50, 50, 50};
    static uint32_t cnt;

    const char * joystickData = pChar->getValue().c_str();
    if(joystickData[0]=='$') {
      for(int i=0;joystickData[i]!='\0';i++) {
        if(joystickData[i]!=255)
          joystickPacket[i] = joystickData[i];
      }
    
//      Serial.printf("R,P,Y,T : %3d,%3d,%3d,%3d\n", 
//        joystickPacket[3], joystickPacket[4], 
//        joystickPacket[5], joystickPacket[6]);

      double rol, pch;
      int thr;      
      rol = joystickPacket[3];
      pch = joystickPacket[4];
      thr = joystickPacket[6];

      thr = 100 - thr;
      thr = thr - 50;
      thr = thr * 2;
      if (thr < 0) thr = 0;
          
      extern double tAngleX, tAngleY;
      extern int throttle;
      tAngleX = (rol-50)/1.5;
      tAngleY = -(pch-50)/1.5;
      throttle = thr*10; 
    
//      Serial.printf("R,P,T : %0.3f,%0.3f,%3d\n", 
//        tAngleX, tAngleY,throttle);

    }     
  }
}; // InCharEventCallback

class OutCharEventCallback: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic* pChar) {
    Serial.println("OutCharEventCallback - onRead: ");
    //pChar->setValue("Hello World!");
  }
}; // OutCharEventCallback

InCharEventCallback *pCharInCallback;
OutCharEventCallback *pCharOutCallback;

void ble_setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  initBle();
  
  startAdvertising();
}

void ble_loop() {
  if(isConnected) {
    
  }
}

void initBle() {
  // Create the BLE Device
  BLEDevice::init("");
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerEventCallback());

  setService();
}

void startAdvertising() {
  // Create advertising manager
  pAdvertising = pServer->getAdvertising();

  // Set beacon data
  setBeacon();

  // Start advertising
  pAdvertising->start();
}

void stopAdvertising() {
  // Stop advertising
  pAdvertising->stop();
}

void setBeacon() {
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(1);
  oBeacon.setMinor(1);
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)

  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04
  
  std::string strServiceData = "";
  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData(); 
  oAdvertisementData.addData(strServiceData);

  oScanResponseData.setName("llllllllllllllllllll");

  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

void setService() {
  pService = pServer->createService(SERVICE_UUID);
  pCharIn = pService->createCharacteristic(
                                         IN_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharOut = pService->createCharacteristic(
                                         OUT_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  pCharOut->addDescriptor(new BLE2902());
  
  pCharInCallback = new InCharEventCallback();
  pCharOutCallback = new OutCharEventCallback();
  pCharIn->setCallbacks(pCharInCallback);
  pCharOut->setCallbacks(pCharOutCallback);
  pService->start();
}
