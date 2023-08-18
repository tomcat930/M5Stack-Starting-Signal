#include <M5Stack.h>

const int beep_volume = 3;

unsigned long current_time;
unsigned long start_time;
unsigned long count_time;
unsigned long buttun_push_time;

int count_mode = 0;

const int countdown_mode = 1;
const int countup_mode = 2;
const int countstop_mode = 3;

int countstop_time = 0;
int countstop_enable_time = 0;
int countstop_disable_time = 0;

int mm;
int ss;
int ms;

const int countdown_xpos = 128;
const int countdown_ypos = 120;

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
  M5.Lcd.setCursor(31, 110, 1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("COUNT START APP");
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
  countstop_time = 0;
  count_time = countstop_mode;
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
    count_mode = countdown_mode;
    count_time = 3;
    buttun_push_time = current_time;
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(2);
  }

  // timer stop
  if (M5.BtnA.wasPressed() && count_mode == countup_mode) {
    countstop_enable_time = current_time;
    count_mode = countstop_mode;
    btnAdisplayText("RESTART");
    btnCdisplayText("RESET");
  } else if (M5.BtnA.wasPressed() && count_mode == countstop_mode) {
    count_mode = countup_mode;
    countstop_disable_time = current_time;
    countstop_time =
        countstop_disable_time - countstop_enable_time + countstop_time;
    M5.Lcd.fillScreen(TFT_BLACK);
  }

  if (M5.BtnC.wasPressed() && count_mode == countup_mode) {
    resetCount();
  }

  if (M5.BtnC.wasPressed() && count_mode == countstop_mode) {
    resetCount();
  }

  switch (count_mode) {
    case countdown_mode:
      if (current_time == buttun_push_time + 1000) {
        M5.Lcd.fillCircle(70, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(countdown_xpos, countdown_ypos, 7);
        M5.Lcd.print(count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 2000) {
        M5.Lcd.fillCircle(160, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(countdown_xpos, countdown_ypos, 7);
        M5.Lcd.print(--count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 3000) {
        M5.Lcd.fillCircle(250, signal_ypos, signal_size, RED);
        M5.Lcd.setCursor(countdown_xpos, countdown_ypos, 7);
        M5.Lcd.print(--count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 4000) {
        start_time = current_time;
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(countdown_xpos, countdown_ypos, 7);
        M5.Lcd.print(--count_time);
        beep(400);

        M5.Lcd.fillScreen(TFT_BLACK);
        count_mode = countup_mode;
      }
      break;

    case countup_mode:
      count_time = current_time - start_time;
      count_time -= countstop_time;
      btnAdisplayText("STOP");
      btnCdisplayText("RESET");
      drawTimeDisplay(count_time);
      break;

    case countstop_mode:
      drawTimeDisplay(count_time);
      break;

    default:
      break;
  }
}
