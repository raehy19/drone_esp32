const int LED = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 1000; i = i + 50)
  {
    delay(i);
    digitalWrite(LED,HIGH);
    delay(i);
    digitalWrite(LED,LOW);
    Serial.println(i);
  }
}
