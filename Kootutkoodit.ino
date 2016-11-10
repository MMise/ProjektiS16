#include <dht.h>
dht DHT;
#define DHT11_PIN 2/// Sensorin pinnin määritys

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

#include <DS1307RTC.h>
#include <Wire.h>
#include <Time.h>

Adafruit_BMP280 bme;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);   // LSD näytön pinnit
const int buttonPin = 2;     // the number of the pushbutton pin

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
  Serial.println(F("BMP280 test"));
  pinMode(buttonPin, INPUT);  // initialize the pushbutton pin as an input:
  
  if (!bme.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.print("Eka rivi, aika");
  lcd.print("Lampotila = ");
  lcd.print("Kolmas rivi, ilmankosteus");
  lcd.print("Neljäs rivi, ilmanpaine");
  int valinta = 1;
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) 
    {
        int valinta++; ///////////Pitää vielä miettiä
     }
  else if valinta == 1;
    {
      //tulosta kaikki anturiarvot
    }
  else if valinta == 2;
  {
    //tulostaan lisätiedot antureilta?? esim korkeustidot
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Aika: ");
  lcd.print(aikaLUL);///////////
  lcd.setCursor(1, 1);
  lcd.print("Lampotila= ");//12 merkkiä pitkä/16
  lcd.print(DHT.temperature);
  lcd.print(" *C");
  lcd.setCursor(2, 1);
  lcd.print("Kosteus= ");//9 merkkiä pitkä/16
  lcd.print(DHT.humidity);
  lcd.print(" %");
  lcd.setCursor(3, 1);
  lcd.print("Paine= ");//7 merkkiä pitkä/16
  lcd.print(bme.readPressure());
  lcd.print(" Pa");

  Serial.print(F("Lampotila = "));
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  Serial.print(F("Paina = "));
  Serial.print(bme.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bme.readAltitude(1013.25)); // pitää säätää paikallisen sääennusteen mukaan
  Serial.println(" m");
  
  Serial.println();
  ///////
  Serial.print("Lampotila = ");
  Serial.println(DHT.temperature); //printataan sarjamonitorille lämpötila
  Serial.print("Ilmankosteus = ");
  Serial.println(DHT.humidity); //printataan sarjamonitorille ilmankosteus
  delay(1000);
}
