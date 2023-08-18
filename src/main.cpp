#include <M5Stack.h>

const int beep_volume = 3;

unsigned long current_time;
unsigned long start_time;
unsigned long count_time;
unsigned long buttun_push_time;

int count_state = 0;

const int initial_state = 0;
const int countdown_state = 1;
const int countup_state = 2;
const int countstop_state = 3;

int countstop_time = 0;
int countstop_enable_time = 0;
int countstop_disable_time = 0;

int mm;
int ss;
int ms;

const int countdown_xpos = 128;
const int countdown_ypos = 120;

const int signal_xpos = 160;
const int signal_ypos = 60;
const int signal_size = 30;

void beep(int beepTime) {
  M5.Speaker.beep();
  delay(beepTime);
  M5.Speaker.mute();
}

void btnAdisplayText(String text) {
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(1);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(text, 68, 220);
}

void btnCdisplayText(String text) {
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(1);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(text, 253, 220);
}

void drawStartDisplay() {
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(1);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(3);
  M5.Lcd.drawString("TIME COUNTER APP", 160, 110);
  btnAdisplayText("START");
}

void drawCountdownDisplay(int time) {
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(1);
  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(String(time), 160, 120);
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
  count_state = initial_state;
  countstop_time = 0;
  count_time = countstop_state;
  M5.Lcd.fillScreen(TFT_BLACK);
  drawStartDisplay();
}

void setup() {
  M5.begin();
  M5.Speaker.begin();
  M5.Speaker.setVolume(beep_volume);
}

void loop() {
  M5.update();
  current_time = millis();

  if (M5.BtnA.wasPressed() && count_state == initial_state) {
    count_state = countdown_state;
    count_time = 3;
    buttun_push_time = current_time;
    M5.Lcd.fillScreen(TFT_BLACK);
  }

  // timer stop
  if (M5.BtnA.wasPressed() && count_state == countup_state) {
    countstop_enable_time = current_time;
    count_state = countstop_state;
    btnAdisplayText("RESTART");
    btnCdisplayText("RESET");
  } else if (M5.BtnA.wasPressed() && count_state == countstop_state) {
    count_state = countup_state;
    countstop_disable_time = current_time;
    countstop_time =
        countstop_disable_time - countstop_enable_time + countstop_time;
    M5.Lcd.fillScreen(TFT_BLACK);
  }

  if (M5.BtnC.wasPressed() && count_state == countup_state) {
    resetCount();
  }

  if (M5.BtnC.wasPressed() && count_state == countstop_state) {
    resetCount();
  }

  switch (count_state) {
    case 0:
      drawStartDisplay();
      break;
    case countdown_state:
      if (current_time == buttun_push_time + 1000) {
        M5.Lcd.fillCircle(70, signal_ypos, signal_size, RED);
        drawCountdownDisplay(count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 2000) {
        M5.Lcd.fillCircle(160, signal_ypos, signal_size, RED);
        drawCountdownDisplay(--count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 3000) {
        M5.Lcd.fillCircle(250, signal_ypos, signal_size, RED);
        drawCountdownDisplay(--count_time);
        beep(100);

      } else if (current_time == buttun_push_time + 4000) {
        start_time = current_time;
        M5.Lcd.fillScreen(TFT_BLACK);
        drawCountdownDisplay(--count_time);
        beep(400);

        M5.Lcd.fillScreen(TFT_BLACK);
        count_state = countup_state;
      }
      break;

    case countup_state:
      count_time = current_time - start_time;
      count_time -= countstop_time;
      btnAdisplayText("STOP");
      btnCdisplayText("RESET");
      drawTimeDisplay(count_time);
      break;

    case countstop_state:
      drawTimeDisplay(count_time);
      break;

    default:
      break;
  }
}
