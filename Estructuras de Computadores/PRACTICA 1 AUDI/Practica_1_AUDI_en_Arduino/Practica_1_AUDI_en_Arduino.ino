int pin = 0;

void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {

  digitalWrite(0, HIGH);
  digitalWrite(7, HIGH);
  delay(200);

  digitalWrite(1, HIGH);
  digitalWrite(6, HIGH);
  delay(200);

  digitalWrite(2, HIGH);
  digitalWrite(5, HIGH);
  delay(200);

  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(200);
  

  delay(1000);

  digitalWrite(0, LOW);
  digitalWrite(7, LOW);
  delay(200);

  digitalWrite(1, LOW);
  digitalWrite(6, LOW);
  delay(200);

  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
  delay(200);

  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  delay(200);

}
