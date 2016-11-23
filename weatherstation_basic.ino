#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x3F, 20, 4); //LCD-näytön alustus
#include "i2c.h"
#include "i2c_BMP280.h"
#include <dht.h>
#define DS1307_I2C_ADDRESS 0x68 // HAE I2C-SKANNERILLA KELLON OSOITE!!!
#define DHT22_PIN 2
dht DHT;
BMP280 bmp280; //Painesensorin osoitteen määritys


//MUUNNOSFUNKTIOIDEN MÄÄRITYS:

// Kymmenjärjestelmästä binääriksi
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Binääristä kymmenjärjestelmään
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

//ALUSTETAAN SENSORIT JA LCD-NÄYTTÖ
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd.begin;
  // set the initial time here:
  // DS1307 seconds, minutes, hours, day, date, month, year
  // setDS1307time(30,42,21,4,26,11,14);        kommentoi pois, aseta arvot tähän?
      Serial.print("Probe BMP280: ");
    if (bmp280.initialize()) Serial.println("Sensor found"); //jätä vain bmp280.initialize() kunhan on testattu, että sensori löytyy
    else
    {
        Serial.println("Sensor missing");
        while (1) {}
    }

    // onetime-measure:
    bmp280.setEnabled(0);
    bmp280.triggerMeasurement();
}

//I2C-DATASIIRTOFUNKTIOIDEN MÄÄRITTELY:
// Arduinolta kellolle:
void setDS1307time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)              //vai tähän?
{
  // Asettaa DS1307:n ajan
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // sekunnit
  Wire.write(decToBcd(minute)); // minuutit
  Wire.write(decToBcd(hour)); // tunnit
  Wire.write(decToBcd(dayOfWeek)); // viikonpäivät (1=Sunnuntai, 7=Lauantai)
  Wire.write(decToBcd(dayOfMonth)); // päivä (1 - 31)
  Wire.write(decToBcd(month)); // kuukausi
  Wire.write(decToBcd(year)); // vuosi (0 - 99)
  Wire.endTransmission();
}

/*
 KOMMENTOI YLLÄOLEVA FUNKTIO POIS KUN SE ON LADATTU ENSIMMÄISEN KERRAN ARDUINOON. 
 TÄMÄN JÄLKEEN LATAA SKETSI UUDESTAAN, JOTTA KELLONAJAKSI EI ASETETA VANHOJA ARVOJA JOKA KERTA KUN LAITE KÄYNNISTYY
 */

//Kellolta Arduinolle:
void readDS1307time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // Asettaa DS1307:n rekisteriosoittimen osoitteeseen 00h
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // Pyytää DS1307:lta 7 bittiä dataa alkaen rekisteristä 00h
  /*
   Täytyykö datasheetistä hakea jokaisen rekisterin osoite?
   */
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

//KELLONAJAN PRINTTAUS LCD-NÄYTÖLLE -FUNKTION MÄÄRITTELY:

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // hakee datan
  readDS1307time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // tulostaa LCD-näytön ensimmäiselle riville
  lcd.setCursor(0,0);
  lcd.print(hour, DEC);
  // muuttaa datan binääristä kymmenjärjestelmään
  lcd.print(":");
  if (minute<10)
  {
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10)
  {
    lcd.print("0");
  }
  lcd.print(second, DEC);
  lcd.print(" ");
  lcd.print(dayOfMonth, DEC);
  lcd.print(".");
  lcd.print(month, DEC);
  lcd.print(".");
  lcd.print(year, DEC);
  lcd.print(" ");
  switch(dayOfWeek){
  case 1:
    lcd.print("SU");
    break;
  case 2:
    lcd.print("MA");
    break;
  case 3:
    lcd.print("TI");
    break;
  case 4:
    lcd.print("KE");
    break;
  case 5:
    lcd.print("TO");
    break;
  case 6:
    lcd.print("PE");
    break;
  case 7:
    lcd.print("LA");
    break;
  }
}


void loop()
{
  displayTime(); // Aika LCD-näytölle
  /* TIEDOT BMP280:lta */
  bmp280.awaitMeasurement();
  float temperature280;
  bmp280.getTemperature(temperature280); 
  float pascal;
  bmp280.getPressure(pascal);
  float hPa = pascal / 100; //Pa -> hPa
  bmp280.triggerMeasurement();

  /*TIEDOT DHT22:lta */
  int chk = DHT.read11(DHT22_PIN);
  float temperature22 = DHT.temperature;
  float humidity = DHT.humidity;

  /*ANTURIEN LUKEMIEN KESKIARVO*/
  
  float temperature = (temperature280 + temperature22) / 2;
  
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 2);
  lcd.print(hPa);
  lcd.print(" hPa");
  lcd.setCursor(0, 3);
  lcd.print(humidity);
  lcd.print(" %");
  delay(1000); // delay pienemmäksi? Alunperin tässä oli tarkoitus vain päivittää kellonaika joka sekunti
}
