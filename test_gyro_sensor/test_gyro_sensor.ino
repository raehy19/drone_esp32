#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000);

  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0x0);
  Wire.endTransmission(true);
}

void loop() {
  // put your main code here, to run repeatedly:

  
  Wire.beginTransmission(0x68);
  Wire.write(0x3b);
  Wire.endTransmission(false);
  Wire.requestFrom((uint16_t) 0x68, (uint8_t) 14, true);

  int16_t AcXH = Wire.read();
  int16_t AcXL = Wire.read();
  int16_t AcYH = Wire.read();
  int16_t AcYL = Wire.read();
  int16_t AcZH = Wire.read();
  int16_t AcZL = Wire.read();
  
  int16_t TmpH = Wire.read();
  int16_t TmpL = Wire.read();

  
  int16_t GyXH = Wire.read();
  int16_t GyXL = Wire.read();
  int16_t GyYH = Wire.read();
  int16_t GyYL = Wire.read();
  int16_t GyZH = Wire.read();
  int16_t GyZL = Wire.read();

  int16_t AcX = AcXH << 8 | AcXL;
  int16_t AcY = AcYH << 8 | AcYL;
  int16_t AcZ = AcZH << 8 | AcZL;
  int16_t GyX = GyXH << 8 | GyXL;
  int16_t GyY = GyYH << 8 | GyYL;
  int16_t GyZ = GyZH << 8 | GyZL;

  Serial.printf("| AcX : %6d | AcY : %6d | AcZ : %6d | GyX : %6d | GyY : %6d | GyZ : %6d |\n", AcX, AcY, AcZ, GyX, GyY, GyZ);
  
  
}
