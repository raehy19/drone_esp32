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

int data_sampling_en = 0; // 데이터 수집 활성화 변수
int data_streaming_en = 0; // 수집된 데이터 출력 변수

void TaskAutoAviMain(void *pvParameters) {

  delay(1000);

  pinMode(0, INPUT);

  while (true) {
    int pinState = digitalRead(0);
    if (pinState == LOW) break;
  }

  data_sampling_en = 1;

  for (int thr = 30; thr < 530; thr++) { // 500 * 4 = 2000밀리초 상승
    throttle = thr;
    delay(4);
  }

  throttle = 510;
  delay(1000);

  throttle = 490;
  delay(1000);

  for (int thr = 475; thr > 300; thr--) { // (475-300)*12 = 2100밀리초 하강
    throttle = thr;
    delay(12);
  }

  for (int thr = 300; thr > 0; thr--) { // 300*5 = 1500밀리초 하강
    throttle = thr;
    delay(5);
  }

  throttle = 0;

  data_sampling_en = 0;
  data_streaming_en = 1;

  vTaskDelete(TaskAutoAviHandle);

}
