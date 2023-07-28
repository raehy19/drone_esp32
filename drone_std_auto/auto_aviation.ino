TaskHandle_t TaskAutoAviHandle;

void auto_setup() {
  xTaskCreatePinnedToCore(
    TaskAutoAviMain,   /* Task function. */
    "TaskAutoAvi",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &TaskAutoAviHandle, /* Task handle */
    0);          /* pin task to core 0 */

  delay(2000); 
}

extern double tAngleX, tAngleY;
extern int throttle;
void TaskAutoAviMain( void * pvParameters ){
  
  pinMode(0, INPUT);
  
  while(true) {
    int pinState = digitalRead(0);
    if(pinState == LOW) break;
  }

  for(int thr = 0; thr < 475; thr++) { // 475*4 = 1900밀리 초간 상승
    throttle = thr;
    delay(4);
  }

  for(int thr = 475; thr > 300; thr--) { // (475-300)*12 = 2100밀리 초간 하강
    throttle = thr;
    delay(12);
  }

  for(int thr = 300; thr > 0; thr--) { // 300*5 = 1500밀리 초간 하강
    throttle = thr;
    delay(5);
  }

  throttle = 0;

  vTaskDelete( TaskAutoAviHandle );
  
}
