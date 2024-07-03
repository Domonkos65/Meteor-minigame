#include <LedControl.h>

int DIN = 10;
int CS = 12;
int CLK = 11;
int x_Led;
int y_Led;
LedControl lc = LedControl(DIN, CLK, CS, 0);

const int X = 0;
const int Y = 1;
int x_pozicio;
int y_pozicio;

const int gomb = 50;
bool lenyomva;

int eppen_szam = 1;

void setup() {
  //Serial.begin();
  // put your setup code here, to run once:
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);

  pinMode(gomb, INPUT);

}

void loop() {
  x_pozicio = analogRead(X);
  y_pozicio = analogRead(Y);
  lenyomva = digitalRead(gomb);

  while (digitalRead(gomb) == LOW){
    x_pozicio = analogRead(X);
    y_pozicio = analogRead(Y);
    lenyomva = digitalRead(gomb);

    if (x_pozicio < 200) {
      if (eppen_szam == 1) {
        eppen_szam = 10;
      }
      else{
        eppen_szam -= 1;
      }
      delay(500);
    }

    if (x_pozicio > 813) {
      if (eppen_szam == 10) {
        eppen_szam = 1;
      }
      else{
        eppen_szam += 1;
      }
    delay(500);
    }
    display(eppen_szam);
    delay(50);
  }

  //Serial.println("VEGE")
}

void egy() {
  lc.setRow(0, 1, B00001000);
  lc.setRow(0, 2, B00011000);
  lc.setRow(0, 3, B00001000);
  lc.setRow(0, 4, B00001000);
  lc.setRow(0, 5, B00001000);
  lc.setRow(0, 6, B00001000);
  lc.setRow(0, 7, B00011100);
}

void ketto() {
  lc.setRow(0, 1, B00011100);
  lc.setRow(0, 2, B00100010);
  lc.setRow(0, 3, B00000010);
  lc.setRow(0, 4, B00000100);
  lc.setRow(0, 5, B00001000);
  lc.setRow(0, 6, B00010000);
  lc.setRow(0, 7, B00111110);
}

void harom() {
  lc.setRow(0, 1, B00111100);
  lc.setRow(0, 2, B00000010);
  lc.setRow(0, 3, B00000010);
  lc.setRow(0, 4, B00011100);
  lc.setRow(0, 5, B00000010);
  lc.setRow(0, 6, B00000010);
  lc.setRow(0, 7, B00111100);
}

void negy() {
  lc.setRow(0, 1, B00100010);
  lc.setRow(0, 2, B00100010);
  lc.setRow(0, 3, B00100010);
  lc.setRow(0, 4, B00111110);
  lc.setRow(0, 5, B00000010);
  lc.setRow(0, 6, B00000010);
  lc.setRow(0, 7, B00000010);
}

void ot() {
  lc.setRow(0, 1, B00111110);
  lc.setRow(0, 2, B00100000);
  lc.setRow(0, 3, B00100000);
  lc.setRow(0, 4, B00111100);
  lc.setRow(0, 5, B00000010);
  lc.setRow(0, 6, B00000010);
  lc.setRow(0, 7, B00111100);
}

void hat() {
  lc.setRow(0, 1, B00011100);
  lc.setRow(0, 2, B00100000);
  lc.setRow(0, 3, B00100000);
  lc.setRow(0, 4, B00111100);
  lc.setRow(0, 5, B00100010);
  lc.setRow(0, 6, B00100010);
  lc.setRow(0, 7, B00011100);
}

void het() {
  lc.setRow(0, 1, B00111110);
  lc.setRow(0, 2, B00000010);
  lc.setRow(0, 3, B00000010);
  lc.setRow(0, 4, B00000100);
  lc.setRow(0, 5, B00001000);
  lc.setRow(0, 6, B00001000);
  lc.setRow(0, 7, B00001000);
}

void nyolc() {
  lc.setRow(0, 1, B00011100);
  lc.setRow(0, 2, B00100010);
  lc.setRow(0, 3, B00100010);
  lc.setRow(0, 4, B00011100);
  lc.setRow(0, 5, B00100010);
  lc.setRow(0, 6, B00100010);
  lc.setRow(0, 7, B00011100);
}

void kilenc() {
  lc.setRow(0, 1, B00011100);
  lc.setRow(0, 2, B00100010);
  lc.setRow(0, 3, B00100010);
  lc.setRow(0, 4, B00011110);
  lc.setRow(0, 5, B00000010);
  lc.setRow(0, 6, B00100010);
  lc.setRow(0, 7, B00011100);
}

int felirat[8][21] {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0},
  {1,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0},
  {1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0},
  {1,0,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0},

};

int position = 0;
void MAX() {
  //lc.clearDisplay(0);
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, j, i, felirat[j][(i + position) - abs((i+position)/21)*21]);//* */
    }
  }
  delay(25);
  position += 1;
}


void menu() {
  
}

void display(int szam){
  switch(szam) {
    case 1:
      egy();
      break;
    case 2:
      ketto();
      break;
    case 3:
      harom();
      break;
    case 4:
      negy();
      break;
    case 5:
      ot();
      break;
    case 6:
      hat();
      break;
    case 7:
      het();
      break;
    case 8:
      nyolc();
      break;
    case 9:
      kilenc();
      break;
    case 10:
      MAX();
      break;
  }
}

