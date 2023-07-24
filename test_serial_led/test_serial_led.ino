const int LED = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    char readData = Serial.read();

    Serial.print("read : ");
    Serial.println(readData);

    if (readData == 'o') {
      digitalWrite(LED,HIGH);
    }
    else if (readData == 'f') {
      digitalWrite(LED,LOW);
    }
    
  }

}
