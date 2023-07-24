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
  // put your setup code here, to run once:

  ledcAttachPin(FAN_PIN_FL, FAN_CHANNEL_FL);
  ledcAttachPin(FAN_PIN_FR, FAN_CHANNEL_FR);
  ledcAttachPin(FAN_PIN_BL, FAN_CHANNEL_BL);
  ledcAttachPin(FAN_PIN_BR, FAN_CHANNEL_BR);

  ledcSetup(FAN_CHANNEL_FL, FAN_FREQUENCY, FAN_RESOLUTION);
  ledcSetup(FAN_CHANNEL_FR, FAN_FREQUENCY, FAN_RESOLUTION);
  ledcSetup(FAN_CHANNEL_BL, FAN_FREQUENCY, FAN_RESOLUTION);
  ledcSetup(FAN_CHANNEL_BR, FAN_FREQUENCY, FAN_RESOLUTION);

  ledcWrite(FAN_CHANNEL_FL, 100);
  ledcWrite(FAN_CHANNEL_FR, 100);
  ledcWrite(FAN_CHANNEL_BL, 100);
  ledcWrite(FAN_CHANNEL_BR, 100);

  delay(5000);
  
  ledcWrite(FAN_CHANNEL_FL, 0);
  ledcWrite(FAN_CHANNEL_FR, 0);
  ledcWrite(FAN_CHANNEL_BL, 0);
  ledcWrite(FAN_CHANNEL_BR, 0);
  
}


void loop() {
  // put your main code here, to run repeatedly:
  

}
