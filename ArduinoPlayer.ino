#include <IRremote.h>
#include <LiquidCrystal.h>
#include <NewTone.h>
#include <WString.h>
#include <elapsedMillis.h>
#include "SR04.h"
#include "pitches.h"

#define TRIG_PIN 5
#define ECHO_PIN 13

#define BUZZER_PIN A0

#define RECEIVER_PIN 6  // pin for the IR reciver

#define VISUALIZER_MAX_FREQUENCY \
  1000.00                 // frequency needed to turn on all leds
#define NUMBER_OF_LEDS 8  // total number of leds used for the visualizer

// pins for 74HC595
#define LATCH_PIN 3  // ST_CP [RCK] on 74HC595
#define CLOCK_PIN 4  // SH_CP [SCK] on 74HC595
#define DATA_PIN 2   // DS [S1] on 74HC595

// ULTRASONIC variable
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long distance;

IRrecv irrecv(RECEIVER_PIN);  // create instance of "irrecv"
decode_results results;       // create instance of "decode_results"

// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Music Player State variables
int current = 1;
bool power = true;

// Songs
int melody[9][56] = {
    // test song
    {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6},

    // test song reverse
    {NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5},

    // Mario Underworld
    {NOTE_C4,  NOTE_C5,  NOTE_A3,  NOTE_A4,  NOTE_AS3, NOTE_AS4, 0,
     0,        NOTE_C4,  NOTE_C5,  NOTE_A3,  NOTE_A4,  NOTE_AS3, NOTE_AS4,
     0,        0,        NOTE_F3,  NOTE_F4,  NOTE_D3,  NOTE_D4,  NOTE_DS3,
     NOTE_DS4, 0,        0,        NOTE_F3,  NOTE_F4,  NOTE_D3,  NOTE_D4,
     NOTE_DS3, NOTE_DS4, 0,        0,        NOTE_DS4, NOTE_CS4, NOTE_D4,
     NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_GS3, NOTE_G3,  NOTE_CS4, NOTE_C4,
     NOTE_FS4, NOTE_F4,  NOTE_E3,  NOTE_AS4, NOTE_A4,  NOTE_GS4, NOTE_DS4,
     NOTE_B3,  NOTE_AS3, NOTE_A3,  NOTE_GS3, 0,        0,        0},

    // Pirates of the Caribbean
    {NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5,
     0, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0, NOTE_A4, NOTE_G4, NOTE_A4, 0}};

// Power off the lcd
void powerOFF() { lcd.clear(); }

// takes input from the distance recived from the ultrasonic sensor and playes a
// frequency based on that
void playUltrasonic() {
  distance = sr04.Distance();  // get distance in cm
  int frequency;
  checkForChange();  // check if a button is pressed and then update the lcd
  if (distance < 50) {
    noNewTone(BUZZER_PIN);
    frequency = static_cast<int>(
        (distance / 50.00) *
        5000);  // get frequency as a percentage of (distance/max distance)
    NewTone(BUZZER_PIN, frequency, 100);
    visualizerPlayer(frequency);
  } else {
    visualizerPlayer(50);
  }
  customDelay(100);
}

byte leds;

void visualizerPlayer(int frequency) {
  leds = 0;
  int numberOfLedsOn =
      (frequency < VISUALIZER_MAX_FREQUENCY)
          ? static_cast<int>((frequency / VISUALIZER_MAX_FREQUENCY) *
                             NUMBER_OF_LEDS)
          : NUMBER_OF_LEDS;  // get nummber of leds to turn on from a percentage
                             // of frequency (frequency/max frequency)
  for (int i = 0; i < numberOfLedsOn; i++) {
    checkForChange();
    bitSet(leds, i);
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, leds);
    digitalWrite(LATCH_PIN, HIGH);
  }
}

// make state changes based on IR input
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
      break;
    case 0xFFA857:
      // VOL-
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

// get song name depending on the input int
String getSongName(int songNumber) {
  switch (songNumber) {
    case 0:
      return "Custom Song";
      break;
    case 1:
      return "test 1";
      break;
    case 2:
      return "test reverse";
      break;
    case 3:
      return "Mario Underworld";
      break;
    case 4:
      return "Pirates of the Caribbean";
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

// check for IR input
void IRdetect() {
  if (irrecv.decode(&results)) {
    changeRemote();
    irrecv.resume();
  }
}

// update LCD display
void updateLCD() {
  if (power) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(getSongName(current));
    delay(10);
  }
}

void customDelay(unsigned int interval) {  // Only use while you need to update
                                           // lcd and volme during delay
  elapsedMillis timeElapsed;
  while (timeElapsed < interval) {
    checkForChange();
  }
}

// check is a change has been made to the state
void checkForChange() {
  IRdetect();
  updateLCD();
}

// function to play the song based in input int
void playsong(int songNumber) {
  int currentNote = 0;
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    currentNote = melody[songNumber - 1][thisNote];
    checkForChange();

    if (melody[current - 1][thisNote] == 0) {
      customDelay(500);
    }
    visualizerPlayer(currentNote);
    NewTone(BUZZER_PIN, currentNote, 500);

    customDelay(500);
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("INITIALIZING...");
  irrecv.enableIRIn();
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  IRdetect();
  if (power) {
    updateLCD();
    if (current == 0) {
      playUltrasonic();
    } else {
      playsong(current);
    }
  }
}