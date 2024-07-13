#include <LedControl.h>

const int X = 0;
const int Y = 1;
int x_pozicio;
int y_pozicio;

int DIN = 10;
int CS = 12;
int CLK = 11;
int x_Led;
int y_Led;
LedControl lc = LedControl(DIN, CLK, CS, 0);

const int buzzer = 30;

const int gomb = 50;
bool lenyomva;

//                  x, y
int meteor[2][2] = {{0, random(8)}, {0, random(8)}};
int kor = 0;

int loves_kor = 0;
int fel_loves_kor = 0;
int le_loves_kor = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);

  pinMode(buzzer, OUTPUT);

  pinMode(gomb, INPUT);

  x_Led = 0;
  y_Led = 7;

  lc.setLed(0, y_Led, x_Led, true);
}

void loop() {
  x_pozicio = analogRead(X);
  y_pozicio = analogRead(Y);
  lenyomva = digitalRead(gomb);

  // put your main code here, to run repeatedly:
  Serial.print("Lenyomva: ");
  Serial.println(lenyomva);
  Serial.print("x:");
  Serial.println(x_pozicio);
  Serial.print("y: ");
  Serial.println(y_pozicio);

  if (x_pozicio < 200 && x_Led != 0) {
    lc.setLed(0, y_Led, x_Led, false);
    x_Led -= 1;
  }
  if (x_pozicio > 823 && x_Led != 7) {
    lc.setLed(0, y_Led, x_Led, false);
    x_Led += 1;
  }

  if (lenyomva && y_pozicio > 825 && kor-le_loves_kor >= 30) {
    tone(30, 250, 50);
    delay(50);
    for (int oldal = 0; oldal <= 8; oldal++) {
      lc.setLed(0, 7, x_Led-oldal, true);
      lc.setLed(0, 7, x_Led+oldal, true);

      if (7 == meteor[0][0] && (x_Led-oldal == meteor[0][1] || x_Led+oldal == meteor[0][1])) {
        lc.setLed(0, meteor[0][0], meteor[0][1], false);
        meteor[0][0] = 0;
        meteor[0][1] = random(8);
        tone(30, 100, 50);
        delay(50);
      }
      if (7 == meteor[1][0] && (x_Led-oldal == meteor[1][1] || x_Led+oldal == meteor[1][1])) {
        lc.setLed(0, meteor[1][0], meteor[1][1], false);
        meteor[1][0] = 0;
        meteor[1][1] = random(8);
        tone(30, 100, 50);
        delay(50);
      }

      delay(25);
    }
    lc.clearDisplay(0);
    le_loves_kor = kor;
  }

  else if (lenyomva && y_pozicio < 200 && kor-fel_loves_kor >= 30) {
    tone(30, 250, 50);
    delay(50);
    for (int magassag = 0; magassag <= 6; magassag++) {
      lc.setLed(0, 6 - magassag, x_Led, true);

      if (6-magassag == meteor[0][0] && x_Led == meteor[0][1]) {
        lc.setLed(0, meteor[0][0], meteor[0][1], false);
        meteor[0][0] = 0;
        meteor[0][1] = random(8);
        tone(30, 100, 50);
        delay(50);
      }
      if (6-magassag == meteor[1][0] && x_Led == meteor[1][1]) {
        lc.setLed(0, meteor[1][0], meteor[1][1], false);
        meteor[1][0] = 0;
        meteor[1][1] = random(8);
        tone(30, 100, 50);
        delay(50);
      }

      delay(25);
    }
    lc.clearDisplay(0);
    fel_loves_kor=kor;
  }

  else if (digitalRead(gomb) == HIGH && kor-loves_kor >= 3) {
    tone(30, 250, 50);
    delay(50);
    for (int magassag = 0; magassag <= 6; magassag++) {
      lc.setLed(0, 6 - magassag, x_Led, true);

      if (6-magassag == meteor[0][0] && x_Led == meteor[0][1]) {
        lc.setLed(0, meteor[0][0], meteor[0][1], false);
        meteor[0][0] = 0;
        meteor[0][1] = random(8);
        tone(30, 100, 50);
        delay(50);
        break;
      }
      if (6-magassag == meteor[1][0] && x_Led == meteor[1][1]) {
        lc.setLed(0, meteor[1][0], meteor[1][1], false);
        meteor[1][0] = 0;
        meteor[1][1] = random(8);
        tone(30, 100, 50);
        delay(50);
        break;
      }

      delay(25);
    }
    lc.clearDisplay(0);
    loves_kor = kor;
  }

  else if (kor % 4 == 0 && kor != 0) {
    lc.setLed(0, meteor[0][0], meteor[0][1], false);
    lc.setLed(0, meteor[1][0], meteor[1][1], false);
    meteor[0][0] +=1;
    meteor[1][0] +=1;
  }
  if (meteor[0][0] >= 8 ) {
    for (int y = 0; y<=7; y++) {
      for (int x = 0; x<=7; x++) {
        lc.setLed(0, y, x, true);
        delay(25);
      }
      tone(30, 150, 50);
      delay(50);
    }
  tone(30, 150, 150);
  delay(150);
  tone(30, 175, 250);
  delay(250);
  tone(30, 200, 200);
  delay(200);
  delay(300);
  tone(30, 75, 400);
  delay(400);
  delay(200);
  tone(30, 50, 600);
  delay(600);
  //noTone(30);
  tone(0, 50, 600);
    while (digitalRead(gomb) == LOW){
      //
      delay(100);
    }
    meteor[0][0] = 0;
    meteor[0][1] = random(8);
    meteor[1][0] = 0;
    meteor[1][1] = random(8);
  
  }
  if (meteor[1][0] >= 8 ) {
    for (int y = 0; y<=7; y++) {
      for (int x = 0; x<=7; x++) {
        lc.setLed(0, y, x, true);
        delay(25);
      }
      tone(30, 150, 50);
      delay(50);
    }
    tone(30, 150, 150);
    delay(150);
    tone(30, 175, 250);
    delay(250);
    tone(30, 200, 200);
    delay(200);
    delay(300);
    tone(30, 75, 400);
    delay(400);
    delay(200);
    tone(30, 50, 600);
    delay(600);
    noTone(30);
    while (digitalRead(gomb) == LOW){
      //lenyomva = digitalRead(SW);
      delay(100);
    }
    meteor[0][0] = 0;
    meteor[0][1] = random(8);
    meteor[1][0] = 0;
    meteor[1][1] = random(8);
  }

  lc.setLed(0, y_Led, x_Led, true);
  lc.setLed(0, meteor[0][0], meteor[0][1], true);
  lc.setLed(0, meteor[1][0], meteor[1][1], true);
  delay(175);
  kor += 1;
}