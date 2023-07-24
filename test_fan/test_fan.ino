const int FAN_PIN_FL = 23;
const int FAN_PIN_FR = 19;
const int FAN_PIN_BL = 26;
const int FAN_PIN_BR = 18;

void setup() {
  // put your setup code here, to run once:
  pinMode(FAN_PIN_FL, OUTPUT);
  pinMode(FAN_PIN_FR, OUTPUT);
  pinMode(FAN_PIN_BL, OUTPUT);
  pinMode(FAN_PIN_BR, OUTPUT);
}

unsigned int spin_cnt = 40;

void loop() {
  // put your main code here, to run repeatedly:

  int spin_time  = 10;
  int delay_time  = 90;
  
  if (spin_cnt > 0) {
    --spin_cnt;

    // Front Left
    digitalWrite(FAN_PIN_FL, HIGH);
    // Front Right
    digitalWrite(FAN_PIN_FR, HIGH);
    // Back Left
    digitalWrite(FAN_PIN_BL, HIGH);
    // Back Right
    digitalWrite(FAN_PIN_BR, HIGH);

    // Spin
    delay(spin_time);

    // Front Left
    digitalWrite(FAN_PIN_FL, LOW);
    // Front Right
    digitalWrite(FAN_PIN_FR, LOW);
    // Back Left
    digitalWrite(FAN_PIN_BL, LOW);
    // Back Right
    digitalWrite(FAN_PIN_BR, LOW);
    
    // Delay
    delay(delay_time);
    
  }

}
