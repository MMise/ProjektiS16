void setup() {
  // put your setup code here, to run once:
  int analogPin = 2;
  pinMode(11, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int val = 0;
int aanenKorkeus = analogRead (A2);
if (digitalRead(11) == HIGH)
{
  val = rand()%6;
  if (val == 0)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    tone(10, 65535 / aanenKorkeus);
    delay(333);
    noTone(10);
  }

  else if (val == 1)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    tone(10, 65535 / aanenKorkeus);
    delay(333);
    noTone(10);
  }
    else if (val == 2)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    tone(10, 65535 / aanenKorkeus);
    delay(333);
    noTone(10);
  }
    else if (val == 3)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    tone(10, 65535 / aanenKorkeus);
    delay(333);
    noTone(10);
  }
    else if (val == 4)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    tone(10, 65535 / aanenKorkeus);
    delay(333);
    noTone(10);
  }
    else if (val == 5)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    tone(10, 65535 / aanenKorkeus);
    delay(333);
    noTone(10);
  }
  
}
}
