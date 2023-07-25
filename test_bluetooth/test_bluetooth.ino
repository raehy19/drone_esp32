#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  // put your setup code here, to run once:
  SerialBT.begin("esp32_Drone_999");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available() > 0 ) {
    char userInput = SerialBT.read();
    SerialBT.println(userInput);
  }
}
