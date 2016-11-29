#include <math.h> 
 //määritetään pinnejä
 const int VCCPin = A0; //Lisätty uusi
 const int xPin   = A1; //Muutettu
 const int yPin   = A2; //Muutettu
 const int zPin   = A3; //Muutettu
 const int GNDPin = A4; //Lisätty uusi

// muuttujat
 int x = 0;
 int y = 0;
 int z = 0;
 double ax = 0.0;
 double ay = 0.0;
 double az = 0.0;
 double vektorinPituus = 0.0;
 double kallistusKulmaX = 0.0;
 double kallistusKulmaY = 0.0;
 double kallistusKulmaZ = 0.0;
 unsigned long aika = 0;

void setup() 
{
  //määritetään pinnit ja datan lähetysnopeus
 pinMode(A0, OUTPUT);
 pinMode(A4, OUTPUT);
 digitalWrite(14, HIGH);
 digitalWrite(18, LOW);
 Serial.begin(9600);
} 
 
void loop() 
{
  x = analogRead(xPin);
  y = analogRead(yPin);
  z = analogRead(zPin);
  //lasketaan kiihtyvyydet sensoriarvoista
  double ax = 0.1464 * x - 49.084;
  ax = constrain(ax, -9.81, 9.81);
  double ay = 0.1459 * y - 48.678;
  ay = constrain(ay, -9.81, 9.81);
  double az = 0.1453 * z - 48.785;
  az = constrain(az, -9.81, 9.81);
  
  vektorinPituus = sqrt((ax * ax)+(ay * ay)+(az * az));
  //lasketaan kallistuskulmat
  kallistusKulmaX = asin (ax/9.81) * (180/PI);
  kallistusKulmaY = asin (ay/9.81) * (180/PI);
  kallistusKulmaZ = asin (az/9.81) * (180/PI);
  //tulostetaan sarjamonitoriin
  Serial.print(" ax: ");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(" ay: ");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(" az: ");
  Serial.print(az);
  Serial.print("\t");
  Serial.print("A");
  Serial.print(vektorinPituus);
  Serial.print("\t");
  Serial.print("DegX: ");
  Serial.print(kallistusKulmaX);
  Serial.print("\t");
  Serial.print("DegY: ");
  Serial.print(kallistusKulmaY);
  Serial.print("\t");
  Serial.print("DegZ: ");
  Serial.print(kallistusKulmaZ);
  Serial.print("\n");
  // viive
  delay(100);  
}
