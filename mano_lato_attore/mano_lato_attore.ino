#include <hidboot.h>
#include <usbhub.h>
#include <SPI.h>

#define flex_a A8
#define flex_b A9
#define flex_c A10
#define flex_d A11

#define btn A4

#define calOpenBtn 40
#define calCloseBtn 42

#define calOpenLed 41
#define calCloseLed 43

uint8_t aV, bV, cV, dV;

uint8_t flexs[4]    = {flex_a, flex_b, flex_c, flex_d};
int     t_min[4]    = {525, 565, 495, 529};
int     t_max[4]    = {720, 780, 690, 670};
int     reads[4]    = {0,0,0,0};
long    sumReads[4] = {0,0,0,0};
long    indReads[4] = {0,0,0,0};
char    names[4]    = {'a','b','c','d'};


class KbdRptParser : public KeyboardReportParser
{
  protected:
    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
};


void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  
  uint8_t c = OemToAscii(mod, key);

  char ts = (char)c;

  switch (ts) {

    case 'A':
      aV = 180;
      break;
    case 'S':
      bV = 180;
      break;
    case 'D':
      cV = 180;
      break;
    case 'F':
      dV = 180;
      break;
    case 'a':
      aV = 180;
      break;
    case 's':
      bV = 180;
      break;
    case 'd':
      cV = 180;
      break;
    case 'f':
      dV = 180;
      break;

    case 'H':
      aV = 90;
      break;
    case 'J':
      bV = 90;
      break;
    case 'K':
      cV = 90;
      break;
    case 'L':
      dV = 90;
      break;
    case 'h':
      aV = 90;
      break;
    case 'j':
      bV = 90;
      break;
    case 'k':
      cV = 90;
      break;
    case 'l':
      dV = 90;
      break;

    default:
      break;
    
  }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  
  uint8_t c = OemToAscii(mod, key);

  char ts = (char)c;

  switch (ts) {

    case 'A':
      aV = 0;
      break;
    case 'S':
      bV = 0;
      break;
    case 'D':
      cV = 0;
      break;
    case 'F':
      dV = 0;
      break;
    case 'a':
      aV = 0;
      break;
    case 's':
      bV = 0;
      break;
    case 'd':
      cV = 0;
      break;
    case 'f':
      dV = 0;
      break;

    case 'H':
      aV = 0;
      break;
    case 'J':
      bV = 0;
      break;
    case 'K':
      cV = 0;
      break;
    case 'L':
      dV = 0;
      break;
    case 'h':
      aV = 0;
      break;
    case 'j':
      bV = 0;
      break;
    case 'k':
      cV = 0;
      break;
    case 'l':
      dV = 0;
      break;

    default:
      break;
    
  }
}


USB Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
 
  pinMode(btn, INPUT_PULLUP);
  
  Serial.begin(9600);     //debug
  Serial3.begin(115200);  //RS485

  //Serial.println("a,b,c,d");

  pinMode(calOpenLed, OUTPUT);
  pinMode(calOpenBtn, INPUT_PULLUP);

  pinMode(calCloseLed, OUTPUT);
  pinMode(calCloseBtn, INPUT_PULLUP);

  digitalWrite(calOpenLed, LOW);
  digitalWrite(calCloseLed, LOW);
  delay(1000);

}

bool firstKBD = true;
bool firstFLX = true;

unsigned long tk = 0;

bool firstOpenOut = true;
bool firstCloseOut = true;

unsigned long tOpen = 0;
unsigned long tClose = 0;

void loop() {
 

  //OPEN HAND CALIBRATION
  if (!digitalRead(calOpenBtn)) {
    for (uint8_t i=0; i<4; i++) {
      sumReads[i] += analogRead(flexs[i]);
      indReads[i]++;
    }
    firstOpenOut = true;
  }
  else {
    if (firstOpenOut) {
      if (sumReads[0] > 0) {
        digitalWrite(calOpenLed,HIGH);
        for (uint8_t i=0; i<4; i++) {
          t_min[i] = sumReads[i]/indReads[i];
          Serial.print("MIN: ");
          Serial.print(names[i]);
          Serial.print(": ");
          Serial.print(sumReads[i]);
          Serial.print("/");
          Serial.print(indReads[i]);
          Serial.print(" = ");
          Serial.println(t_min[i]);
          Serial.println();
          sumReads[i] = 0;
          indReads[i] = 0;
        }     
        firstOpenOut = false;
        digitalWrite(calOpenLed,LOW);
      }
    }
  }
  
  //CLOSED HAND CALIBRATION
  if (!digitalRead(calCloseBtn)) {
    for (uint8_t i=0; i<4; i++) {
      sumReads[i] += analogRead(flexs[i]);
      indReads[i]++;
    }
    firstCloseOut = true;
  }
  else {
    if (firstCloseOut) {
      if (sumReads[0] > 0) {
        digitalWrite(calCloseLed,HIGH);
        for (uint8_t i=0; i<4; i++) {
          t_max[i] = sumReads[i]/indReads[i];
          Serial.print("MAX: ");
          Serial.print(names[i]);
          Serial.print(": ");
          Serial.print(sumReads[i]);
          Serial.print("/");
          Serial.print(indReads[i]);
          Serial.print(" = ");
          Serial.println(t_max[i]);
          Serial.println();
          sumReads[i] = 0;
          indReads[i] = 0;
        }     
        firstCloseOut = false;
        digitalWrite(calCloseLed,LOW);
      }
    }
  }
  
  

  if (!digitalRead(btn)) {
    
    //flexs
    if (firstFLX) {
      Serial.println("a,b,c,d");
      firstKBD = true;
      firstFLX = false;
      delay(250);
    }

    for (uint8_t i=0; i<4; i++) {
      reads[i] = map(analogRead(flexs[i]),t_min[i],t_max[i],2,180);
      Serial3.print(names[i]);
      Serial3.print(reads[i]);
    }
    
    delay(50);
    
  }
  else {
    //kbd
    if (firstKBD) {
      Serial.println("KBD");
      firstFLX = true;
      firstKBD = false;
      Usb.Init();
      HidKeyboard.SetReportParser(0, &Prs);
      delay(250);
    }

    Usb.Task();

    if ((millis() - tk) > 50) {
      tk = millis();
      Serial3.print('a');
      Serial3.print(aV);
      Serial3.print('b');
      Serial3.print(bV);
      Serial3.print('c');
      Serial3.print(cV);
      Serial3.print('d');
      Serial3.print(dV);
    }
    
  }
  
}
