/*
  MIC1 Week 6. Sensors and Actuators 2

  Author Lucas Vonk
         2189041
  Date   10/10/2026
*/

#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <chip_temperature.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
int previousButtonState = 0;

const int buttonPin = 21;

int hours = 0;
int minutes = 0;
int seconds = 0;

const int N_DATA = 3;
int data[N_DATA] = { 0, 1, 23 };

const int markerAddress = N_DATA * sizeof(int);
const byte savedMarker = 123;

unsigned long interval = 1000;

bool alarmActive = false;
bool previousAlarmMatch = false;
bool backlightOn = true;
unsigned long alarmStartMillis = 0;
unsigned long previousBlinkMillis = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  previousButtonState = digitalRead(buttonPin);

  for (unsigned int i = 0; i < N_DATA; i++) {
    unsigned int address = i * sizeof(int);
    EEPROM.get(address, data[i]);
  }

  if (EEPROM.read(markerAddress) != savedMarker ||
      data[0] < 0 || data[0] > 23 ||
      data[1] < 0 || data[1] > 59 ||
      data[2] < 0 || data[2] > 59) {
    data[0] = 0;
    data[1] = 1;
    data[2] = 23;
    saveAlarm();
  }

  lcd.init();
  lcd.backlight();
  showTime();
  showAlarm();

  previousMillis = millis();
}

void loop() {

  unsigned long currentMillis = millis();

  while (currentMillis - previousMillis >= interval) {
    previousMillis += interval;

    seconds++;
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }
    if (hours >= 24) {
      hours = 0;
    }

    showTime();
  }

  int buttonState = digitalRead(buttonPin);

  if (buttonState != previousButtonState) {
    delay(30);
    buttonState = digitalRead(buttonPin);

    if (buttonState != previousButtonState) {
      previousButtonState = buttonState;


      if (buttonState == LOW) {
        data[0] = hours;
        data[1] = minutes;
        data[2] = seconds;

        saveAlarm();
        showAlarm();
        Serial.println("Switch press detected: alarm saved in EEPROM");
      }
    }
  }

  bool alarmMatch = hours == data[0] &&
                    minutes == data[1] &&
                    seconds == data[2];

  if (alarmMatch && !previousAlarmMatch) {
    alarmActive = true;
    alarmStartMillis = currentMillis;
    previousBlinkMillis = currentMillis;
    backlightOn = true;
    lcd.backlight();
  }
  previousAlarmMatch = alarmMatch;

  if (alarmActive) {
    if (currentMillis - alarmStartMillis >= 10000) {
      alarmActive = false;
      backlightOn = true;
      lcd.backlight();
    } else if (currentMillis - previousBlinkMillis >= 250) {
      previousBlinkMillis += 250;
      backlightOn = !backlightOn;
      if (backlightOn) {
        lcd.backlight();
      } else {
        lcd.noBacklight();
      }
    }
  }
}

void saveAlarm() {
  for (unsigned int i = 0; i < N_DATA; i++) {
    unsigned int address = i * sizeof(int);
    EEPROM.put(address, data[i]);
  }
  EEPROM.update(markerAddress, savedMarker);
}

void showTime() {
  char timeText[9];
  snprintf(timeText, sizeof(timeText), "%02d:%02d:%02d",
           hours, minutes, seconds);

  lcd.setCursor(0, 0);
  lcd.print(timeText);
  Serial.println(timeText);

  int temperature = chip_temperature_read();
  char temperatureText[7];
  snprintf(temperatureText, sizeof(temperatureText), "%4d%cC",
           temperature, 0xDF);
  lcd.setCursor(10, 0);
  lcd.print(temperatureText);
}

void showAlarm() {
  char alarmText[9];
  snprintf(alarmText, sizeof(alarmText), "%02d:%02d:%02d",
           data[0], data[1], data[2]);
  lcd.setCursor(0, 1);
  lcd.print(alarmText);
}
