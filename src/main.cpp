#include <M5Stack.h>

const int beep_volume = 3;

unsigned long current_time;
unsigned long start_time;
unsigned long count_time;

int buttun_push_time;

int count_mode = 0;

const int count_down_mode = 1;
const int count_up_mode = 2;
const int count_stop_mode = 3;

int count_stop_time = 0;
int count_stop_start_time = 0;
int count_stop_end_time = 0;

int mm;
int ss;
int ms;

const int count_down_xpos = 128;
const int count_down_ypos = 120;

int signal_xpos;
int signal_ypos = 60;
int signal_size = 30;

void beep(int beepTime) {
  M5.Speaker.beep();
  delay(beepTime);
  M5.Speaker.mute();
}

void btnAdisplayText(String text) {
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextDatum(1);
  M5.Lcd.drawString(text, 68, 220);
}

void btnCdisplayText(String text) {
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextDatum(1);
  M5.Lcd.drawString(text, 253, 220);
}

void drawStartDisplay() {
  // title display setup
  M5.Lcd.setCursor(31, 110, 1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("COUNT START APP");
  // btnA display setup
  btnAdisplayText("START");
}

void drawTimeDisplay(unsigned long time) {
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(52, 80, 7);
  M5.Lcd.setTextColor(RED, BLACK);
  // time calculation
  mm = (time / 60000);
  ss = (time / 1000) % 60;
  ms = (time % 1000) / 10;
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
}

void resetCount() {
  count_mode = 0;
  count_stop_time = 0;
  count_time = count_stop_mode;
  M5.Lcd.fillScreen(TFT_BLACK);
  drawStartDisplay();
}

void setup() {
  M5.begin();
  M5.Speaker.begin();
  M5.Speaker.setVolume(beep_volume);
  drawStartDisplay();
}

void loop() {
  M5.update();
  current_time = millis();

  if (M5.BtnA.wasPressed() && count_mode == 0) {
    count_mode = count_down_mode;
    count_time = 3;
    buttun_push_time = current_time;
    M5.Lcd.fillScreen(TFT_BLACK);
    // count down display setup
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(2);
  }

  // timer stop
  if (M5.BtnA.wasPressed() && count_mode == count_up_mode) {
    count_stop_start_time = current_time;
    count_mode = count_stop_mode;
    btnAdisplayText("RESTART");
    btnCdisplayText("RESET");
  } else if (M5.BtnA.wasPressed() && count_mode == count_stop_mode) {
    count_mode = count_up_mode;
    count_stop_end_time = current_time;
    count_stop_time =
        count_stop_end_time - count_stop_start_time + count_stop_time;
    M5.Lcd.fillScreen(TFT_BLACK);
  }

  if (M5.BtnC.wasPressed() && count_mode == count_up_mode) {
    resetCount();
  }

  if (M5.BtnC.wasPressed() && count_mode == count_stop_mode) {
    resetCount();
  }

  switch (count_mode) {
    case count_down_mode:
      if (current_time == buttun_push_time + 1000) {
        M5.Lcd.fillCircle(70, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(count_down_xpos, count_down_ypos, 7);
        M5.Lcd.print(count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 2000) {
        M5.Lcd.fillCircle(160, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(count_down_xpos, count_down_ypos, 7);
        M5.Lcd.print(--count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 3000) {
        M5.Lcd.fillCircle(250, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(count_down_xpos, count_down_ypos, 7);
        M5.Lcd.print(--count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 4000) {
        start_time = current_time;
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(count_down_xpos, count_down_ypos, 7);
        M5.Lcd.print(--count_time);
        beep(400);

        M5.Lcd.fillScreen(TFT_BLACK);
        count_mode = count_up_mode;
      }
      break;

    case count_up_mode:
      count_time = current_time - start_time;
      count_time -= count_stop_time;
      btnAdisplayText("STOP");
      btnCdisplayText("RESET");
      drawTimeDisplay(count_time);
      break;

    case count_stop_mode:
      drawTimeDisplay(count_time);
      break;

    default:
      break;
  }
}
