#include <Servo.h>
#include "RunningAverage.h"

RunningAverage aRA(7);
RunningAverage bRA(7);
RunningAverage cRA(7);
RunningAverage dRA(7);

#define limInf 2
#define limSup 180

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

  aRA.clear();
  bRA.clear();
  cRA.clear();
  dRA.clear();
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

  if (aV < limInf) {
    aV = limInf;
  }
  else if (aV >= limSup) {
    aV = limSup;
  }

  if (bV < limInf) {
    bV = limInf;
  }
  else if (bV >= limSup) {
    bV = limSup;
  }

  if (cV < limInf) {
    cV = limInf;
  }
  else if (cV >= limSup) {
    cV = limSup;
  }

  if (dV < limInf) {
    dV = limInf;
  }
  else if (dV >= limSup) {
    dV = limSup;
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

    aRA.addValue(map(aV,2,180,2,60));
    bRA.addValue(map(bV,2,180,2,55));
    cRA.addValue(map(cV,2,180,180,125));
    dRA.addValue(map(dV,2,180,180,132));
    
  }

  aM.write(aRA.getAverage());
  bM.write(bRA.getAverage());
  cM.write(cRA.getAverage());
  dM.write(dRA.getAverage());

}
