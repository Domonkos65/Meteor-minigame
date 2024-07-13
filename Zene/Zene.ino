void setup() {
  // put your setup code here, to run once:
  pinMode(30, OUTPUT);
  //noTone(30);

}

void loop() {
  // put your main code here, to run repeatedly:
  // game over
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
  delay(5000);

  // atmenet
  tone(30, 200, 200);
  delay(200);
  delay(25);
  tone(30, 250, 100);
  delay(100);
  delay(25);
  tone(30, 275, 100);
  delay(100);
  delay(25);
  tone(30, 300, 100);
  delay(100);
  delay(50);
  tone(30, 450, 400);
  delay(400);
  delay(3000);
}
