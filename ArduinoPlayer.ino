// #include <Time.h>
// #include <TimeLib.h>

#include <IRremote.h>
#include <LiquidCrystal.h>
#include <NewTone.h>
#include <WString.h>
#include <elapsedMillis.h>
#include "SR04.h"
#include "pitches.h"

// TODO: set Ultrasonic pins
// #define TRIG_PIN 12
#define BUZZER_PIN 5
// #define ECHO_PIN 11

// #define MAX_FREQUENCY 4978
#define VISUILIZER_MAX_FREQUENCY 1000.00
#define NUMBER_OF_LEDS 8

// ULTRASONIC variable
// SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
// long distance;

// pins for 74HC595
int latchPin = 3;  // (11) ST_CP [RCK] on 74HC595
int clockPin = 4;  // (9) SH_CP [SCK] on 74HC595
int dataPin = 2;   // (12) DS [S1] on 74HC595

int receiver = 6;  // IR reciver

IRrecv irrecv(receiver);  // create instance of "irrecv"
decode_results results;   // create instance of "decode_results"
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int current = 1;
bool power = true;
// time_t t = now();

int melody[9][56] = {
    {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6},
    {NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5},
    {NOTE_C4,  NOTE_C5,  NOTE_A3,  NOTE_A4,  NOTE_AS3, NOTE_AS4, 0,
     0,        NOTE_C4,  NOTE_C5,  NOTE_A3,  NOTE_A4,  NOTE_AS3, NOTE_AS4,
     0,        0,        NOTE_F3,  NOTE_F4,  NOTE_D3,  NOTE_D4,  NOTE_DS3,
     NOTE_DS4, 0,        0,        NOTE_F3,  NOTE_F4,  NOTE_D3,  NOTE_D4,
     NOTE_DS3, NOTE_DS4, 0,        0,        NOTE_DS4, NOTE_CS4, NOTE_D4,
     NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_GS3, NOTE_G3,  NOTE_CS4, NOTE_C4,
     NOTE_FS4, NOTE_F4,  NOTE_E3,  NOTE_AS4, NOTE_A4,  NOTE_GS4, NOTE_DS4,
     NOTE_B3,  NOTE_AS3, NOTE_A3,  NOTE_GS3, 0,        0,        0},
    {NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5,
     0, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0, NOTE_A4, NOTE_G4, NOTE_A4, 0}};

void powerOFF() { lcd.clear(); }


// TODO: test and fix ultrasonic func working
// void playUltrasonic() {
//   distance = sr04.Distance();
//   int frequency;
//   while (distance < 10) {
//     noNewTone(BUZZER_PIN);
//     frequency = static_cast<int>((distance / 10.00) * 5000);
//     NewTone(BUZZER_PIN, frequency, 100);
//     visuilizerPlayer(frequency);
//   }
//   customDelay(100);
// }

byte leds;

void visuilizerPlayer(int frequency) {
  leds = 0;
  frequency = (frequency < VISUILIZER_MAX_FREQUENCY)
                  ? static_cast<int>((frequency / VISUILIZER_MAX_FREQUENCY) *
                                     NUMBER_OF_LEDS)
                  : NUMBER_OF_LEDS;
  for (int i = 0; i < frequency; i++) {
    bitSet(leds, i);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
    digitalWrite(latchPin, HIGH);
  }
}

void changeRemote() {
  switch (results.value) {
    case 0xFFA25D:
      // POWER
      power = !power;
      powerOFF();
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF22DD:
      // FAST BACK
      if (current == 1) {
        current = 9;
      } else {
        --current;
      }
      noNewTone(BUZZER_PIN);
      break;
    case 0xFFC23D:
      // FAST FORWARD
      if (current == 9) {
        current = 1;
      } else {
        ++current;
      }
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF629D:
      // VOl +
      // if (volume != 255) {
      //   volume += 10;
      // }
      break;
    case 0xFFA857:
      // VOL-
      // if (volume != 0) {
      //   volume -= 10;
      // }
      break;
    case 0xFF6897:
      // ZERO
      current = 0;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF30CF:
      // 1
      current = 1;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF18E7:
      // 2
      current = 2;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF7A85:
      // 3
      current = 3;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF10EF:
      // 4
      current = 4;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF38C7:
      // 5
      current = 5;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF5AA5:
      // 6
      current = 6;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF42BD:
      // 7
      current = 7;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF4AB5:
      // 8
      current = 8;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFF52AD:
      // 9
      current = 9;
      noNewTone(BUZZER_PIN);
      break;
    case 0xFFFFFFFF:
      // REPEAT
      break;
    case 0xFFE21D:
      // FUNC/STOP
      break;
    case 0xFF906F:
      // UP
      break;
    case 0xFF9867:
      // EQ
      break;
    case 0xFFB04F:
      // ST/REPT
      break;
    case 0xFFE01F:
      // DOWN
      break;
    case 0xFF02FD:
      // PAUSE
      break;
    default:
      // other button
      break;
  }
}

String getSongName() {
  switch (current) {
    case 0:
      return "Custom Song";
      break;
    case 1:
      return "UNNAMED_SONG_1";
      break;
    case 2:
      return "Mario Underworld";
      break;
    case 3:
      return "UNNAMED_SONG_3";
      break;
    case 4:
      return "UNNAMED_SONG_4";
      break;
    case 5:
      return "UNNAMED_SONG_5";
      break;
    case 6:
      return "UNNAMED_SONG_6";
      break;
    case 7:
      return "UNNAMED_SONG_7";
      break;
    case 8:
      return "UNNAMED_SONG_8";
      break;
    case 9:
      return "UNNAMED_SONG_9";
      break;
  }
}

void IRdetect() {
  if (irrecv.decode(&results)) {
    changeRemote();
    irrecv.resume();
  }
}

void updateLCD() {
  if (power) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(getSongName());
    delay(100);
  }
}

void customDelay(unsigned int interval) {  // may be off by 10 ms. Only use
                                           // while you need to update lcd and
                                           // volme during delay
  elapsedMillis timeElapsed;
  while (timeElapsed < interval) {
    IRdetect();
    updateLCD();
  }
}

void playsong() {
  int currentNote = 0;
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // pin8 output the voice, every scale is 0.5 sencond
    currentNote = melody[current - 1][thisNote];

    if (melody[current - 1][thisNote] == 0) {
      customDelay(500);
    }
    visuilizerPlayer(currentNote);
    NewTone(BUZZER_PIN, currentNote, 500);

    // Output the voice after several minutes
    customDelay(500);
  }
}


void setup() {
  lcd.begin(16, 2);
  lcd.print("INITIALIZING...");
  irrecv.enableIRIn();
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  IRdetect();
  if (power) {
    updateLCD();
    playsong();

    // TODO: make it switch to ultrasonic if current == 0
    // if (current == 0) {
    //   // playUltrasonic();
    // } else {
      
    // }
  }
}