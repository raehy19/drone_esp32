#include <Wire.h>

void setup() {
  Serial.begin(115200);  

  Wire.begin();
  Wire.setClock(400000);

  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0x0);
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
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

  int16_t AcX = AcXH <<8 |AcXL;
  int16_t AcY = AcYH <<8 |AcYL;
  int16_t AcZ = AcZH <<8 |AcZL;
  int16_t GyX = GyXH <<8 |GyXL;
  int16_t GyY = GyYH <<8 |GyYL;
  int16_t GyZ = GyZH <<8 |GyZL;

  // init correction
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

  // calculate angle
  static unsigned long t_prev = 0;
  unsigned long t_now = micros();
  double dt = (t_now - t_prev) / 1000000.0;
  t_prev = t_now;
  
  const float GYROXYZ_TO_DEGREES_PER_SEC = 131;
  double GyXR = GyXD / GYROXYZ_TO_DEGREES_PER_SEC;
  double GyYR = GyYD / GYROXYZ_TO_DEGREES_PER_SEC;
  double GyZR = GyZD / GYROXYZ_TO_DEGREES_PER_SEC;
  
  static double gyAngleX = 0.0, gyAngleY = 0.0, gyAngleZ = 0.0;
  gyAngleX += GyXR *dt;
  gyAngleY += GyYR *dt;
  gyAngleZ += GyZR *dt;
  
  const float RADIANS_TO_DEGREES = 180 / 3.14159;
  double AcYZD = sqrt(pow(AcYD, 2) + pow(AcZD, 2));
  double AcXZD = sqrt(pow(AcXD, 2) + pow(AcZD, 2));
  double acAngleY = atan(-AcXD / AcYZD) * RADIANS_TO_DEGREES;
  double acAngleX = atan(AcYD / AcXZD) * RADIANS_TO_DEGREES;
  double acAngleZ = 0;
  
  static int cnt_loop;
  ++cnt_loop ;
  if (cnt_loop % 200 != 0) return;

  Serial.printf(" dt = %8.6f", dt);
  Serial.printf(" | gyAngleX = %6.1f", gyAngleX);
  Serial.printf(" | gyAngleY = %6.1f", gyAngleY);
  Serial.printf(" | gyAngleZ = %6.1f", gyAngleZ);
  Serial.printf(" | acAngleX = %6.1f", acAngleX);
  Serial.printf(" | acAngleY = %6.1f", acAngleY);
  Serial.printf(" | acAngleZ = %6.1f", acAngleZ);
  Serial.println();

}
