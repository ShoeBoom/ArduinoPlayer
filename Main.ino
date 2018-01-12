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

int receiver = 11;  // IR reciver

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);  // create instance of 'irrecv'
decode_results results;   // create instance of 'decode_results'
const int rs = 5, en = 4, d4 = 3, d5 = 2, d6 = 1, d7 = 1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int current = 0;

int IRdetect() {
  decode_results results;
  if (irrecv.decode(&results)) {
    int return_value = translateIR();
    irrecv.resume();
    return return_value;
  }
}

int translateIR() {
  switch (results.value) {
    case 0xFFA25D:
      // POWER
      return 12;
      break;
    case 0xFFE21D:
      // FUNC/STOP
      return 0;
      break;
    case 0xFF629D:
      // VOl +
      return 10;
      break;
    case 0xFF22DD:
      // FAST BACK
      if (current == 1) {
        return 9;
      } else {
        return --current;
      }
      break;
    case 0xFF02FD:
      // PAUSE
      return 0;
      break;
    case 0xFFC23D:
      // FAST FORWARD
      if (current == 9) {
        return 1;
      } else {
        return ++current;
      }
      break;
    case 0xFFE01F:
      // DOWN
      return 0;
      break;
    case 0xFFA857:
      // VOL-
      return 11;
      break;
    case 0xFF906F:
      // UP
      return 0;
      break;
    case 0xFF9867:
      // EQ
      return 0;
      break;
    case 0xFFB04F:
      // ST/REPT
      return 0;
      break;
    case 0xFF6897:
      // Serial.println("0");
      return 13;
      break;
    case 0xFF30CF:
      // 1
      return 1;
      break;
    case 0xFF18E7:
      // 2
      return 2;
      break;
    case 0xFF7A85:
      // 3
      return 3;
      break;
    case 0xFF10EF:
      // 4
      return 4;
      break;
    case 0xFF38C7:
      // 5
      return 5;
      break;
    case 0xFF5AA5:
      // 6
      return 6;
      break;
    case 0xFF42BD:
      // 7
      return 7;
      break;
    case 0xFF4AB5:
      // 8
      return 8;
      break;
    case 0xFF52AD:
      // 9
      return 9;
      break;
    case 0xFFFFFFFF:
      // REPEAT
      return 0;
      break;

    default:
      // other button
      return 0;
  }
}

string translateLCD(int buttonID) {
  return 'hi me';
}

void setup() {
  irrecv.enableIRIn();
  lcd.begin(16, 2);
  lcd.print("AYY LMAO");
}

void loop() {
  lcd.setCursor(0, 1);
  int buttonID = IRdetect();
  if (buttonID != 0) {
    lcd.print(buttonID);
  }
}