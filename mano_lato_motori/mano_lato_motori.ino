#include <Servo.h>

Servo aM,bM,cM,dM;

void setup() {
  // put your setup code here, to run once:
  Serial3.begin(115200);
  Serial.begin(9600);

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  aM.attach(6);
  bM.attach(7);
  cM.attach(9);
  dM.attach(10);
}

char inc;
int aV,bV,cV,dV;

unsigned long tk = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial3.available()) {
    inc = Serial3.read();

    switch (inc) {
      case 'a':
        aV = Serial3.parseInt();
        break;
      case 'b':
        bV = Serial3.parseInt();
        break;
      case 'c':
        cV = Serial3.parseInt();
        break;
      case 'd':
        dV = Serial3.parseInt();
        break;
      default:
        break;
    }
    
  }

  if (aV < 2) {
    aV = 2;
  }
  else if (aV >= 180) {
    aV = 180;
  }

  if (bV < 2) {
    bV = 2;
  }
  else if (bV >= 180) {
    bV = 180;
  }

  if (cV < 2) {
    cV = 2;
  }
  else if (cV >= 180) {
    cV = 180;
  }

  if (dV < 2) {
    dV = 2;
  }
  else if (dV >= 180) {
    dV = 180;
  }

//  if ((millis() - tk) > 500) {
//    tk = millis();
//    Serial.print(aV);
//    Serial.print("-");
//    Serial.print(bV);
//    Serial.print("-");
//    Serial.print(cV);
//    Serial.print("-");
//    Serial.println(dV);
//  }

  if ((millis() - tk) > 30) {
    tk = millis();
    aM.write(aV);
    bM.write(bV);
    cM.write(cV);
    dM.write(dV);
  }

  
}
