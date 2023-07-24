const int LED = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
//  // 10hz
//  digitalWrite(LED,HIGH);
//  delay(50);
//  digitalWrite(LED,LOW);
//  delay(50);

//  // 100hz
//  digitalWrite(LED,HIGH);
//  delay(5);
//  digitalWrite(LED,LOW);
//  delay(5);

  // 100hz, low brightness
  digitalWrite(LED,HIGH);
  delay(1);
  digitalWrite(LED,LOW);
  delay(9);

  delay(100);
  
  // 100hz, high brightness
  digitalWrite(LED,HIGH);
  delay(9);
  digitalWrite(LED,LOW);
  delay(1);

  
  delay(100);
}
