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

//                    y, x
int meteorok[][2] = {{0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}};
int hullam = 1;
int kor = 0;
int hullam_kor = 0;

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

  
  for (int meteor = 0; meteor < hullam; meteor++) {
    Serial.print(meteor);
    Serial.print(": Y=");
    Serial.print(meteorok[meteor][0]);
    Serial.print(": X=");
    Serial.println(meteorok[meteor][1]);
  }
  /*
  Serial.print("Lenyomva: ");
  Serial.println(lenyomva);
  Serial.print("x:");
  Serial.println(x_pozicio);
  Serial.print("y: ");
  Serial.println(y_pozicio);*/

  if (x_pozicio < 200 && x_Led != 0) {
    lc.setLed(0, y_Led, x_Led, false);
    x_Led -= 1;
  }
  if (x_pozicio > 823 && x_Led != 7) {
    lc.setLed(0, y_Led, x_Led, false);
    x_Led += 1;
  }

  if (lenyomva && y_pozicio > 825 && kor-le_loves_kor >= 25) {
    tone(30, 250, 50);
    delay(50);
    for (int oldal = 0; oldal <= 8; oldal++) {
      lc.setLed(0, 7, x_Led-oldal, true);
      lc.setLed(0, 7, x_Led+oldal, true);

      for (int meteor = 0; meteor < hullam; meteor++) { 

        if (7 == meteorok[meteor][0] && (x_Led-oldal == meteorok[meteor][1] || x_Led+oldal == meteorok[meteor][1])) {
        lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
        meteorok[meteor][0] = 0;
        meteorok[meteor][1] = random(8);
        tone(30, 100, 50);
        delay(50);
        }
      }

      delay(25);
    }
    lc.clearDisplay(0);
    le_loves_kor = kor;
  }

  else if (lenyomva && y_pozicio < 200 && kor-fel_loves_kor >= 25) {
    tone(30, 250, 50);
    delay(50);
    for (int magassag = 0; magassag <= 6; magassag++) {
      lc.setLed(0, 6 - magassag, x_Led, true);

      for (int meteor = 0; meteor < hullam; meteor++) {

        if (6-magassag == meteorok[meteor][0] && x_Led == meteorok[meteor][1]) {
        lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
        meteorok[meteor][0] = 0;
        meteorok[meteor][1] = random(8);
        tone(30, 100, 50);
        delay(50);
        }
      }

      delay(25);
    }
    lc.clearDisplay(0);
    fel_loves_kor=kor;
  }

  else if (digitalRead(gomb) == HIGH && kor-loves_kor >= 3) {
    tone(30, 250, 50);
    delay(50);
    bool talalt = false;
    for (int magassag = 0; magassag <= 6; magassag++) {
      lc.setLed(0, 6 - magassag, x_Led, true);

      for (int meteor = 0; meteor < hullam; meteor++) {

        if (6-magassag == meteorok[meteor][0] && x_Led == meteorok[meteor][1]) {
          lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
          meteorok[meteor][0] = 0;
          meteorok[meteor][1] = random(8);
          tone(30, 100, 50);
          delay(50);
          talalt = true;
          //break;
        }
      }

      if (talalt == true) {
        break;
      }

      delay(25);
    }
    lc.clearDisplay(0);
    loves_kor = kor;
  }

  if (kor % 4 == 0 && kor != 0) {
    for (int meteor = 0; meteor <= hullam-1; meteor++) {
      lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
      meteorok[meteor][0] +=  1;
    }
  }

  for (int meteor = 0; meteor < hullam; meteor++) {
    bool vege = false;

    if (meteorok[meteor][0] >= 8 ) {

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
      delay(100);
      }
      meteorok[0][0] = 0;
      meteorok[0][1] = random(8);
      /*meteorok[1][0] = 0;
      meteorok[1][1] = random(8);*/
      hullam = 1;
      kor = 0;
      hullam_kor = 0;
      loves_kor = 0;
      fel_loves_kor = 0;
      le_loves_kor = 0;
      vege = true;
    }
    if (vege) {
      lc.clearDisplay(0);
      break;
    }
  }

  if (kor - hullam_kor >= 50 && hullam <= 20) {
    meteorok[hullam][0] = 0;
    int random_szam = random(0,8);
    meteorok[hullam][1] = random_szam;/**/
    hullam += 1;
    hullam_kor = kor;
  }

  lc.setLed(0, y_Led, x_Led, true);

  for (int meteor = 0; meteor < hullam; meteor++) {
    lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], true);
  }
  //lc.setLed(0, meteorok[1][0], meteorok[1][1], true);
  delay(175);

  kor += 1;
}
