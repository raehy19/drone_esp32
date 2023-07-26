#include <Wire.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int FAN_PIN_FL = 23;
const int FAN_PIN_FR = 19;
const int FAN_PIN_BL = 26;
const int FAN_PIN_BR = 18;

const int FAN_CHANNEL_FL = 1;
const int FAN_CHANNEL_FR = 2;
const int FAN_CHANNEL_BL = 3;
const int FAN_CHANNEL_BR = 4;

const int FAN_FREQUENCY = 5000;
const int FAN_RESOLUTION = 10;

void setup() {
  Serial.begin(115200);  

  Wire.begin();
  Wire.setClock(400000);

  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0x0);
  Wire.endTransmission(true);
  
  ledcAttachPin(FAN_PIN_FL, FAN_CHANNEL_FL);
  ledcAttachPin(FAN_PIN_FR, FAN_CHANNEL_FR);
  ledcAttachPin(FAN_PIN_BL, FAN_CHANNEL_BL);
  ledcAttachPin(FAN_PIN_BR, FAN_CHANNEL_BR);

  ledcSetup(FAN_CHANNEL_FL, FAN_FREQUENCY, FAN_RESOLUTION);
  ledcSetup(FAN_CHANNEL_FR, FAN_FREQUENCY, FAN_RESOLUTION);
  ledcSetup(FAN_CHANNEL_BL, FAN_FREQUENCY, FAN_RESOLUTION);
  ledcSetup(FAN_CHANNEL_BR, FAN_FREQUENCY, FAN_RESOLUTION);

  ledcWrite(FAN_CHANNEL_FL, 0);
  ledcWrite(FAN_CHANNEL_FR, 0);
  ledcWrite(FAN_CHANNEL_BL, 0);
  ledcWrite(FAN_CHANNEL_BR, 0);

  SerialBT.begin("esp32_Drone_999");
}

int throttle = 0;

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

  int16_t AcX = AcXH << 8 | AcXL;
  int16_t AcY = AcYH << 8 | AcYL;
  int16_t AcZ = AcZH << 8 | AcZL;
  int16_t GyX = GyXH << 8 | GyXL;
  int16_t GyY = GyYH << 8 | GyYL;
  int16_t GyZ = GyZH << 8 | GyZL;

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
  
  // cla error Angle
  const double ALPHA = 0.96;
  static double cmAngleX = 0.0, cmAngleY = 0.0, cmAngleZ = 0.0;
  cmAngleX = ALPHA * (cmAngleX + GyXR * dt) + (1.0 - ALPHA) * acAngleX;
  cmAngleY = ALPHA * (cmAngleY + GyYR * dt) + (1.0 - ALPHA) * acAngleY;
  cmAngleZ = gyAngleZ;
  
  static double tAngleX = 0.0, tAngleY = 0.0, tAngleZ = 0.0;
  double eAngleX = tAngleX - cmAngleX;
  double eAngleY = tAngleY - cmAngleY;
  double eAngleZ = tAngleZ - cmAngleZ;

  // cal p
  double Kp = 1.0;
  double BalX = Kp * eAngleX;
  double BalY = Kp * eAngleY;
  double BalZ = Kp * eAngleZ;

  // get throttle
  if (SerialBT.available() > 0 ) {
    char readData = SerialBT.read();
    if (readData >= '0' && readData <= '9') {
      throttle = (readData - '0') * 100;
    }
  }

  // cal speed
  double speedFL = throttle + BalX - BalY + BalZ;  
  double speedFR = throttle - BalX - BalY - BalZ;  
  double speedBL = throttle + BalX + BalY - BalZ;
  double speedBR = throttle - BalX + BalY + BalZ;  

  // write speed
  ledcWrite(FAN_CHANNEL_FL, speedFL);
  ledcWrite(FAN_CHANNEL_FR, speedFR);
  ledcWrite(FAN_CHANNEL_BL, speedBL);
  ledcWrite(FAN_CHANNEL_BR, speedBR);

  static int cnt_loop;
  ++cnt_loop ;
  if (cnt_loop % 300 != 0) return;
  Serial.printf(" dt = %8.6f", dt);
  Serial.printf(" | FL = %6.1f", speedFL);
  Serial.printf(" | FR = %6.1f", speedFR);
  Serial.printf(" | BL = %6.1f", speedBL);
  Serial.printf(" | BR = %6.1f", speedBR);
  Serial.println();
}
