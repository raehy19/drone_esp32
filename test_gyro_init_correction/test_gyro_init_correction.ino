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

  static int32_t AcXSum = 0, AcYSum = 0, AcZSum = 0;
  static int32_t GyXSum = 0, GyYSum = 0, GyZSum = 0;

  static double AcXOff = 0.0, AcYOff = 0.0, AcZOff = 0.0;
  static double GyXOff = 0.0, GyYOff = 0.0, GyZOff = 0.0;

  static int sample_cnt = 1000;

  if (sample_cnt > 0) {
    AcXSum += AcX;
    AcYSum += AcY;
    AcZSum += AcZ;
    GyXSum += GyX;
    GyYSum += GyY;
    GyZSum += GyZ;
    --sample_cnt;

    if (sample_cnt == 0) {
      AcXOff += AcXSum / 1000.0;
      AcYOff += AcYSum / 1000.0;
      AcZOff += AcZSum / 1000.0;
      GyXOff += GyXSum / 1000.0;
      GyYOff += GyYSum / 1000.0;
      GyZOff += GyZSum / 1000.0;
    }
	  delay(1);
	  return;
  }
  
  double AcXD = AcX - AcXOff;
  double AcYD = AcY - AcYOff;
	double AcZD = AcZ - AcZOff + 16384;
	double GyXD = GyX - GyXOff;
	double GyYD = GyY - GyYOff;
	double GyZD = GyZ - GyZOff;
 
  Serial.printf("| AcX : %8.1f | AcY : %8.1f | AcZ : %8.1f | GyX : %8.1f | GyY : %8.1f | GyZ : %8.1f |\n", AcXD, AcYD, AcZD, GyXD, GyYD, GyZD);


}
