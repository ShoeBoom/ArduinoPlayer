/*
Detects is a signal is being sent.

parms:
irrecv: is the IRrecv
current: current song buttonID


IF no signal is dected or invalid button is pressed: return 0

IF 1 is pressed: return 1
IF 2 is pressed: return 2
...
IF 8 is pressed: return 8
IF 9 is pressed: return 9


IF Volume+ is pressed: return 10
IF Volume- is pressed: return 11
IF POWER is pressed: return 12
IF ZERO is pressed: return 13

IF next is pressed: return next song buttonID
IF current == 9:
return 1
else:
return ++current

IF previous is pressed: return previous song buttonID
IF current == 1:
return 9
else:
return --current
*/

#include <IRremote.h>
#include <LiquidCrystal.h>
#include <WString.h>
#define speakerOut 13
#define MAX_VOLUME 255

int receiver = 6;  // IR reciver
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);  // create instance of "irrecv"
decode_results results;   // create instance of "decode_results"
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int current = 1;
int volume = MAX_VOLUME;
bool power = true;
bool muted = false;
int savedVolume = MAX_VOLUME;

void playSong() {
  
}

double getVolumeForLCD() { return (volume / (double) MAX_VOLUME) * 100.00; }

void powerOFF() { lcd.clear(); }

void IRdetect() {
  if (irrecv.decode(&results)) {
    translateIR();
    irrecv.resume();
  }
}

void translateIR() {
  switch (results.value) {
    case 0xFFA25D:
      // POWER
      power = !power;
      powerOFF();
      break;
    case 0xFF22DD:
      // FAST BACK
      if (current == 1) {
        current = 9;
      } else {
        --current;
      }
      break;
    case 0xFFC23D:
      // FAST FORWARD
      if (current == 9) {
        current = 1;
      } else {
        ++current;
      }
      break;
    case 0xFF629D:
      // VOl +
      if (volume != 255) {
        volume += 10;
      }
      break;
    case 0xFFA857:
      // VOL-
      if (volume != 0) {
        volume -= 10;
      }
      break;
    case 0xFF6897:
      // ZERO
      current = 0;
      break;
    case 0xFF30CF:
      // 1
      current = 1;
      break;
    case 0xFF18E7:
      // 2
      current = 2;
      break;
    case 0xFF7A85:
      // 3
      current = 3;
      break;
    case 0xFF10EF:
      // 4
      current = 4;
      break;
    case 0xFF38C7:
      // 5
      current = 5;
      break;
    case 0xFF5AA5:
      // 6
      current = 6;
      break;
    case 0xFF42BD:
      // 7
      current = 7;
      break;
    case 0xFF4AB5:
      // 8
      current = 8;
      break;
    case 0xFF52AD:
      // 9
      current = 9;
      break;
    case 0xFFFFFFFF:
      // REPEAT
      break;
    case 0xFFE21D:
      // FUNC/STOP
      if (!muted){
        savedVolume = volume;
        volume = 0;
      }else{
        volume = savedVolume;
      }

      muted = !muted;
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

String translateSONG() {
  switch (current) {
    case 0:
      return "UNNAMED_SONG: electric boogaloo";
      break;
    case 1:
      return "UNNAMED_SONG_1";
      break;
    case 2:
      return "UNNAMED_SONG_2";
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

String translateLCD() { return "Volume: " + String(getVolumeForLCD()); }

void updateLCD() {
  if (power){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(translateSONG());
  lcd.setCursor(0, 1);
  lcd.print(translateLCD());
  delay(10);
  }
  
}

void setup() {
  irrecv.enableIRIn();
  lcd.begin(16, 2);
}

void loop() {
  IRdetect();
  updateLCD();
  playSong();
}
