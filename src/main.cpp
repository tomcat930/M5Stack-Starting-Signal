#include <M5Stack.h>

const int beep_volume = 3;

unsigned long current_time;
unsigned long start_time;
unsigned long count_time;

int buttun_push_time;

int mode = 0;
int count = 0;

int mm;
int ss;
int ms;

void setup() {
  M5.begin();

  M5.Speaker.begin();
  M5.Speaker.setVolume(beep_volume);

  M5.Lcd.setTextColor(WHITE, BLACK);

  // title display setup
  M5.Lcd.setCursor(31, 110);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("COUNT START APP");

  // btnA display setup
  M5.Lcd.setCursor(36, 220);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("START");
}

int count_xpos = 128;
int count_ypos = 120;

int signal_xpos;
int signal_ypos = 60;
int signal_size = 30;

const int count_down = 1;
const int count_up = 2;

void loop() {
  M5.update();
  current_time = millis();

  if (M5.BtnA.wasPressed()) {
    mode = count_down;
    count = 3;
    buttun_push_time = current_time;
    M5.Lcd.fillScreen(TFT_BLACK);

    // count down display setup
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(2);
  }

  if (M5.BtnC.wasPressed()) {
    mode = 0;
    count = 3;
    M5.Lcd.fillScreen(TFT_BLACK);
  }

  switch (mode) {
    case count_down:
      if (current_time == buttun_push_time + 1000) {
        M5.Lcd.fillCircle(70, signal_ypos, signal_size, RED);

        M5.Lcd.setCursor(count_xpos, count_ypos, 7);
        M5.Lcd.print(count);

        M5.Speaker.beep();
        delay(100);
        M5.Speaker.mute();

      } else if (current_time == buttun_push_time + 2000) {
        M5.Lcd.fillCircle(160, signal_ypos, signal_size, RED);

        M5.Lcd.setCursor(count_xpos, count_ypos, 7);
        M5.Lcd.print(--count);

        M5.Speaker.beep();
        delay(100);
        M5.Speaker.mute();

      } else if (current_time == buttun_push_time + 3000) {
        M5.Lcd.fillCircle(250, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(count_xpos, count_ypos, 7);
        M5.Lcd.print(--count);

        M5.Speaker.beep();
        delay(100);
        M5.Speaker.mute();

      } else if (current_time == buttun_push_time + 4000) {
        M5.Lcd.fillScreen(TFT_BLACK);

        M5.Lcd.setCursor(count_xpos, count_ypos, 7);
        M5.Lcd.print(--count);

        start_time = current_time;

        M5.Speaker.beep();
        delay(400);
        M5.Speaker.mute();

        M5.Lcd.fillScreen(TFT_BLACK);
        mode = count_up;
        start_time = current_time;
      }
      break;

    case count_up:
      count_time = current_time - start_time;

      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(52, count_ypos, 7);
      M5.Lcd.setTextColor(RED, BLACK);

      // time calculation
      mm = (count_time / 60000);
      ss = (count_time / 1000) % 60;
      ms = (count_time % 1000) / 10;

      // time display
      if (mm < 10) {
        M5.Lcd.print("0" + String(mm));
      } else {
        M5.Lcd.print(mm);
      }
      M5.Lcd.print(":");
      if (ss < 10) {
        M5.Lcd.print("0" + String(ss));
      } else {
        M5.Lcd.print(ss);
      }
      M5.Lcd.print(".");
      M5.Lcd.print(ms);

      break;

    default:
      break;
  }
}
