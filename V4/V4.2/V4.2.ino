/*

A jatekban a lenyeg az, hogy meteoritokat kell leloni az egbol mielott azok elernek a talajt.
Elosszor a meteor felirat jelenik meg. Gombnyomassal tovabb lehet menni a menube.
Utana a menu jelenik meg ahol kivalaszthatod a nehezsegi szintet a gomb lenyomasaval.
Ha a gombot lenyomod es a joystickot fel iranyitod akkor visszalep a meteor felirathoz.
A menuben ha elersz valamelyik szelere a masikon folytatod (MAX->1, MAX<-1)

A jatek nehezsegi szinttol fuggetlenul 1 meteorral kezdodik es folyamatosan egyre tobb lesz.
Azt, hogy maximum mennyi meteorit jelenik meg a palyan a nehezsegi szint szabalyozza meg (nehezsegi szint = max meteorit).
MAX szintten ameddig meg nem halsz keletkeznek uj meteorok (max 20 (fizikailag keptelenseg annal tovabb vinni)).

A jatekban a gomb lenyomasaval tud loni ami az elotted levo oszlopban a hozzad legkozelebb levo
meteort (meteorokat ha egy mezon tobb is van) lovi ki.
Ketto szuper kepessege is van a karaktereknek:
      1. Szuperloves (felloves)
          - tobb meteort is ki tud loni az elotted levo oszlopban (nem all meg egynel hanem tovabb megy)
          - joystick fel + gomb lenyom
      2. Talajloves (leloves)
          - tobb meteort is ki tud loni melleted levo sorokban (nem all meg egynel hanem tovabb megy)
          - joystick le + gomb lenyom
Ezeket nem lehet folamatosan hasznalni mint a sima lovest (van rajta idokorlat) (a sima lovesen is van egy kicsi, hogy ne levehhesn folyamatosan nyomkodni).
A kijeljzon lathato a lelott meteorok szama.

Ha esetleg leesne az egyik meteorit lejatszodik a game over animacio es hang majd utana gomb nyomassal visszalehet terni a menube.
Ez utan ujrakezdodik az egesz.

*/

#include <LedControl.h>

const int X_pin = 0;
const int Y_pin = 1;
int x_pozicio; //Joystick x tengelyen valo elmozdulasa
int y_pozicio;

int DIN = 10;
int CS = 12;
int CLK = 11;
int x_Led; //karakter x tengely pozicioja
int y_Led;
LedControl lc = LedControl(DIN, CLK, CS, 0);

const int buzzer = 30;

const int gomb = 50;
bool lenyomva;

int nehezsegi_szint;

//                    y, x
int meteorok[][2] = {{0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}, {0, random(8)}};
// Meteorok helye

int hullam = 1; // hullam = mennyi meteor van a tablan
int kor = 0;    // mennyiszer futott a program
int hullam_kor = 0; // a hullam emelelese szempontjabol fontos (hullamemeles ota eltelt korok szama)
int pont[4] = {0, 0, 0, 0}; // az eltalalt meteorok szama

int loves_kor = 0;       // loves ota eltelt korok szama
int fel_loves_kor = 0;   // felloves ota eltelt korok szama
int le_loves_kor = 0;    // leloves ota eltelt korok szama


// 4 digit 7 segment display
int latch=5;  //74HC595  pin 9 STCP
int clock=6; //74HC595  pin 10 SHCP
int data=4;   //74HC595  pin 8 DS

const int digit1 = 51;
const int digit2 = 49;
const int digit3 = 47;
const int digit4 = 45;

const int digitPins[4] = {45, 47, 49, 51};

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};




void setup() {
  //  put your setup code here, to run once:
  Serial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, 3); // 2. taggal a fenyesseget lehet szabalyozni
  lc.clearDisplay(0);

  pinMode(buzzer, OUTPUT);

  pinMode(gomb, INPUT);

  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);

  // Digitek 1-4
  pinMode(45, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(51, OUTPUT);

  x_Led = 0;
  y_Led = 7;

  lc.setLed(0, y_Led, x_Led, true);


  // kiirja, hogy meteor
  while (digitalRead(gomb) == LOW){
    Meteor();
  }

  delay(1000);

  // a menubol kivalasztjuk a nehezsegi szintet
  nehezsegi_szint = menu();
  // ha a nehezsegi szint MAX akkor vegtelensegig megy (fizikai keptelenseg 20-nál tovabb)
  if (nehezsegi_szint == 10) {
    nehezsegi_szint = 20;
  }

  lc.clearDisplay(0);
  game_start();
}

void loop() {
  x_pozicio = analogRead(X_pin);
  y_pozicio = analogRead(Y_pin);
  lenyomva = digitalRead(gomb);


  // a joystick allasabol mozgatja a karaktert
  if (x_pozicio < 200) {
    lc.setLed(0, y_Led, x_Led, false);
    if (x_Led == 0) {
      x_Led = 7;
    }
    else {
      x_Led -= 1;
    }
  }
  if (x_pozicio > 823) {
    lc.setLed(0, y_Led, x_Led, false);
    if (x_Led == 7) {
      x_Led = 0;
    }
    else {
      x_Led += 1;
    }
  }

  // leloves
  if (lenyomva && y_pozicio > 825 && kor-le_loves_kor >= 30) {
    tone(30, 250, 50);
    delay(50);

    // noveli a szelesseget
    for (int oldal = 0; oldal <= 8; oldal++) {
      lc.setLed(0, 7, x_Led-oldal, true);
      lc.setLed(0, 7, x_Led+oldal, true);

      // lellenorzi minden meteornal, hogy eltalalta e
      for (int meteor = 0; meteor < hullam; meteor++) { 
        if (7 == meteorok[meteor][0] && (x_Led-oldal == meteorok[meteor][1] || x_Led+oldal == meteorok[meteor][1])) {
        // lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
        meteorok[meteor][0] = 0;
        meteorok[meteor][1] = random(8);

        // A pont novelese
        if (pont[3] == 9) {
          pont[3] = 0;
          if (pont[2] == 9) {
            pont[2] = 0;
            if (pont[1] == 9) {
              pont[1] = 0;
              pont[0] += 1;
            }
            else {
              pont[1] += 1;
            }
          }
          else {
            pont[2] += 1;
          }
        }
        else {
          pont[3] += 1;
        }

        tone(30, 100, 50);
        delay(50);
        }
      }

      delay(25);
    }
    lc.clearDisplay(0);
    le_loves_kor = kor;
  }

  // feloves
  else if (lenyomva && y_pozicio < 200 && kor-fel_loves_kor >= 30) {
    tone(30, 250, 50);
    delay(50);

    // noveli a magassagat
    for (int magassag = 0; magassag <= 7; magassag++) {
      lc.setLed(0, 6 - magassag, x_Led, true);

      // lellenorzi minden meteornal, hogy eltalalta e
      for (int meteor = 0; meteor < hullam; meteor++) {
        if (7-magassag == meteorok[meteor][0] && x_Led == meteorok[meteor][1]) {
        // lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
        meteorok[meteor][0] = 0;
        meteorok[meteor][1] = random(8);

        // A pont novelese
        if (pont[3] == 9) {
          pont[3] = 0;
          if (pont[2] == 9) {
            pont[2] = 0;
            if (pont[1] == 9) {
              pont[1] = 0;
              pont[0] += 1;
            }
            else {
              pont[1] += 1;
            }
          }
          else {
            pont[2] += 1;
          }
        }
        else {
          pont[3] += 1;
        }
        tone(30, 100, 50);
        delay(50);
        }
      }

      delay(25);
    }
    lc.clearDisplay(0);
    fel_loves_kor=kor;
  }

  // sima loves
  else if (digitalRead(gomb) == HIGH && kor-loves_kor >= 3) {
    tone(30, 250, 50);
    delay(50);
    bool talalt = false;

    // noveli a magassagat
    for (int magassag = 0; magassag <= 7; magassag++) {
      lc.setLed(0, 6 - magassag, x_Led, true);

      // lellenorzi minden meteornal, hogy eltalalta e
      for (int meteor = 0; meteor < hullam; meteor++) {
        if (7-magassag == meteorok[meteor][0] && x_Led == meteorok[meteor][1]) {
          lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
          meteorok[meteor][0] = 0;
          meteorok[meteor][1] = random(8);

          // A pont novelese
          if (pont[3] == 9) {
          pont[3] = 0;
          if (pont[2] == 9) {
            pont[2] = 0;
            if (pont[1] == 9) {
              pont[1] = 0;
              pont[0] += 1;
            }
            else {
              pont[1] += 1;
            }
          }
          else {
            pont[2] += 1;
          }
        }
        else {
          pont[3] += 1;
        }
        tone(30, 100, 50);
        delay(50);
        talalt = true;
          // break;
        }
      }

      if (talalt == true) {
        break;
      }
      
      delay(25);
      lc.setLed(0, 6 - magassag, x_Led, false);
    }
    lc.clearDisplay(0);
    loves_kor = kor;
  }


  // 4 koronkent lejjeb viszi a meteorokat
  if (kor % 4 == 0 && kor != 0) {
    for (int meteor = 0; meteor <= hullam-1; meteor++) {
      lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], false);
      meteorok[meteor][0] +=  1;
    }
  }


  // leelenorzi minden meteornal, hogy elerte e az aljat
  for (int meteor = 0; meteor < hullam; meteor++) {
    bool vege = false;

    // ha igen game overt csinal es utana ujrakezdi
    if (meteorok[meteor][0] >= 8 ) {

      game_over();
    
      // ameddig nem nyomja meg a gombot nem csinal semmit csak a pontszamot irja ki felul
      while (digitalRead(gomb) == LOW){
        for (int szamjegy = 0; szamjegy < 4; szamjegy++) {
          Display(pont[szamjegy], szamjegy);
          delay(1);
        }
      }

      lc.clearDisplay(0);
      game_start();
      nehezsegi_szint = menu();
      if (nehezsegi_szint == 10) {
      nehezsegi_szint = 20;
      }
      meteorok[0][0] = 0;
      meteorok[0][1] = random(8);
      hullam = 1;
      kor = 0;
      hullam_kor = 0;
      for (int szamjegy=0; szamjegy < 4; szamjegy++) {
        pont[szamjegy] = 0;
      }
      loves_kor = 0;
      fel_loves_kor = 0;
      le_loves_kor = 0;
      vege = true;
    }
    if (vege) {
      lc.clearDisplay(0);
      game_start();
      break;
    }
  }


  // ha eleg ideje megyunk megnoveli a hullamot
  if (kor - hullam_kor >= 50 && hullam < nehezsegi_szint) {
    meteorok[hullam][0] = 0;
    int random_szam = random(0,8);
    meteorok[hullam][1] = random_szam;
    hullam += 1;
    hullam_kor = kor;
  }

  // megjeleniti a karaktert es a meteorokat
  lc.setLed(0, y_Led, x_Led, true);
  for (int meteor = 0; meteor < hullam; meteor++) {
    lc.setLed(0, meteorok[meteor][0], meteorok[meteor][1], true);
  }

  // hogy a pontszamot ki tudja jelezni a korok utani delayt erre csereltem
  // tapasztalat alapjan mukodik igy
  for (int i = 0; i < 35; i++) {
    for (int szamjegy = 0; szamjegy < 4; szamjegy++) {
      Display(pont[szamjegy], szamjegy);
      delay(1);
    }
  }
  //delay(175);
  kor += 1;
}



// az osszes szam es a MAX megjelenitesere van egy function

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

int feliratMAX[8][21] {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0},
  {1,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0},
  {1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0},
  {1,0,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0},

};

int positionMAX = 0;
void MAX() {
  //lc.clearDisplay(0);
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, j, i, feliratMAX[j][(i + positionMAX) - abs((i+positionMAX)/21)*21]);
    }
  }
  delay(25);
  positionMAX += 1;
}


// Az elejen kiirja azt, hogy meteor
int feliratMe[8][41] {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0},
  {1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,  0,0,0,0,0,0},
  {1,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,  0,0,0,0,0,0},
  {1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,  0,0,0,0,0,0},
  {1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,  0,0,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,  0,0,0,0,0,0},
  {1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,  0,0,0,0,0,0},
  {1,0,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,  0,0,0,0,0,0},

};

int positionMe = 0;
void Meteor() {
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, j, i, feliratMe[j][(i + positionMe) - abs((i+positionMe)/41)*41]);
    }
  }
  delay(5);
  positionMe += 1;
}


// megjeleniti a kivant szamot
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


// a menu a nehezsegi szint kivalasztasara szolgal
int menu() {
  int eppen_szam = 1;

  // addig megy ameddig a gomb le nem nyomodik
  while (true){
    x_pozicio = analogRead(X_pin);
    y_pozicio = analogRead(Y_pin);
    lenyomva = digitalRead(gomb);

    // a menuben navigalas a joystickkal
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
    
    // ha a joystickot fel iranyitod es lenyomod a gombot visszamegy a meteor felirathoz
    if (lenyomva && y_pozicio < 200) {
      delay(500);
      while (digitalRead(gomb) == LOW){
        Meteor();
      }
      delay(500);
    }
    // viszont ha csak a gombot nyomja le akkor kilep a menubol
    else if (digitalRead(gomb) == HIGH) {
      break;
    }

    display(eppen_szam);
    delay(50);
  }


  return eppen_szam;
}


// a jatek elveszitesekor lejatszott dolgok
void game_over() {
  lc.setLed(0, y_Led, x_Led, false);

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
  tone(0, 50, 600);   
}


// a menu es a jatek kozotti animacio (es forditva is)
void game_start() {
  lc.setRow(0, 0, B10000000);
  delay(40);
  lc.setRow(0, 0, B11000000);
  lc.setRow(0, 1, B10000000);
  delay(40);
  lc.setRow(0, 0, B11100000);
  lc.setRow(0, 1, B11000000);
  lc.setRow(0, 2, B10000000);
  delay(40);
  lc.setRow(0, 0, B11110000);
  lc.setRow(0, 1, B11100000);
  lc.setRow(0, 2, B11000000);
  lc.setRow(0, 3, B10000000);
  delay(40);
  lc.setRow(0, 0, B11111000);
  lc.setRow(0, 1, B11110000);
  lc.setRow(0, 2, B11100000);
  lc.setRow(0, 3, B11000000);
  lc.setRow(0, 4, B10000000);
  delay(40);
  lc.setRow(0, 0, B11111100);
  lc.setRow(0, 1, B11111000);
  lc.setRow(0, 2, B11110000);
  lc.setRow(0, 3, B11100000);
  lc.setRow(0, 4, B11000000);
  lc.setRow(0, 5, B10000000);
  delay(40);
  lc.setRow(0, 0, B11111110);
  lc.setRow(0, 1, B11111100);
  lc.setRow(0, 2, B11111000);
  lc.setRow(0, 3, B11110000);
  lc.setRow(0, 4, B11100000);
  lc.setRow(0, 5, B11000000);
  lc.setRow(0, 6, B10000000);
  delay(40);
  lc.setRow(0, 0, B11111111);
  lc.setRow(0, 1, B11111110);
  lc.setRow(0, 2, B11111100);
  lc.setRow(0, 3, B11111000);
  lc.setRow(0, 4, B11110000);
  lc.setRow(0, 5, B11100000);
  lc.setRow(0, 6, B11000000);
  lc.setRow(0, 7, B10000000);
  delay(40);
  lc.setRow(0, 1, B11111111);
  lc.setRow(0, 2, B11111110);
  lc.setRow(0, 3, B11111100);
  lc.setRow(0, 4, B11111000);
  lc.setRow(0, 5, B11110000);
  lc.setRow(0, 6, B11100000);
  lc.setRow(0, 7, B11000000);
  delay(40);
  lc.setRow(0, 2, B11111111);
  lc.setRow(0, 3, B11111110);
  lc.setRow(0, 4, B11111100);
  lc.setRow(0, 5, B11111000);
  lc.setRow(0, 6, B11110000);
  lc.setRow(0, 7, B11100000);
  delay(40);
  lc.setRow(0, 3, B11111111);
  lc.setRow(0, 4, B11111110);
  lc.setRow(0, 5, B11111100);
  lc.setRow(0, 6, B11111000);
  lc.setRow(0, 7, B11110000);
  delay(40);
  lc.setRow(0, 4, B11111111);
  lc.setRow(0, 5, B11111110);
  lc.setRow(0, 6, B11111100);
  lc.setRow(0, 7, B11111000);
  delay(40);
  lc.setRow(0, 5, B11111111);
  lc.setRow(0, 6, B11111110);
  lc.setRow(0, 7, B11111100);
  delay(40);
  lc.setRow(0, 6, B11111111);
  lc.setRow(0, 7, B11111110);
  delay(40);
  lc.setRow(0, 7, B11111111);
  delay(40);

  lc.setRow(0, 0, B01111111);
  delay(40);
  lc.setRow(0, 0, B00111111);
  lc.setRow(0, 1, B01111111);
  delay(40);
  lc.setRow(0, 0, B00011111);
  lc.setRow(0, 1, B00111111);
  lc.setRow(0, 2, B01111111);
  delay(40);
  lc.setRow(0, 0, B00001111);
  lc.setRow(0, 1, B00011111);
  lc.setRow(0, 2, B00111111);
  lc.setRow(0, 3, B01111111);
  delay(40);
  lc.setRow(0, 0, B00000111);
  lc.setRow(0, 1, B00001111);
  lc.setRow(0, 2, B00011111);
  lc.setRow(0, 3, B00111111);
  lc.setRow(0, 4, B01111111);
  delay(40);
  lc.setRow(0, 0, B00000011);
  lc.setRow(0, 1, B00000111);
  lc.setRow(0, 2, B00001111);
  lc.setRow(0, 3, B00011111);
  lc.setRow(0, 4, B00111111);
  lc.setRow(0, 5, B01111111);
  delay(40);
  lc.setRow(0, 0, B00000001);
  lc.setRow(0, 1, B00000011);
  lc.setRow(0, 2, B00000111);
  lc.setRow(0, 3, B00001111);
  lc.setRow(0, 4, B00011111);
  lc.setRow(0, 5, B00111111);
  lc.setRow(0, 6, B01111111);
  delay(40);
  lc.setRow(0, 0, B00000000);
  lc.setRow(0, 1, B00000001);
  lc.setRow(0, 2, B00000011);
  lc.setRow(0, 3, B00000111);
  lc.setRow(0, 4, B00001111);
  lc.setRow(0, 5, B00011111);
  lc.setRow(0, 6, B00111111);
  lc.setRow(0, 7, B01111111);
  delay(40);
  lc.setRow(0, 1, B00000000);
  lc.setRow(0, 2, B00000001);
  lc.setRow(0, 3, B00000011);
  lc.setRow(0, 4, B00000111);
  lc.setRow(0, 5, B00001111);
  lc.setRow(0, 6, B00011111);
  lc.setRow(0, 7, B00111111);
  delay(40);
  lc.setRow(0, 2, B00000000);
  lc.setRow(0, 3, B00000001);
  lc.setRow(0, 4, B00000011);
  lc.setRow(0, 5, B00000111);
  lc.setRow(0, 6, B00001111);
  lc.setRow(0, 7, B00011111);
  delay(40);
  lc.setRow(0, 3, B00000000);
  lc.setRow(0, 4, B00000001);
  lc.setRow(0, 5, B00000011);
  lc.setRow(0, 6, B00000111);
  lc.setRow(0, 7, B00001111);
  delay(40);
  lc.setRow(0, 4, B00000000);
  lc.setRow(0, 5, B00000001);
  lc.setRow(0, 6, B00000011);
  lc.setRow(0, 7, B00000111);
  delay(40);
  lc.setRow(0, 5, B00000000);
  lc.setRow(0, 6, B00000001);
  lc.setRow(0, 7, B00000011);
  delay(40);
  lc.setRow(0, 6, B00000000);
  lc.setRow(0, 7, B00000001);
  delay(40);
  lc.setRow(0, 7, B00000000);
  delay(40);

}



// 4 digit 7 segment led kijelzesere szolgal
void Display(unsigned char num, int digitDisplay)
{

  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);

  digitalWrite(digitPins[digitDisplay], LOW);

  for(int k = 0; k < 4; k ++){
    if (k == digitDisplay)
    continue;
    else
    digitalWrite(digitPins[k], HIGH);
  }
  
}
