//BMP280-SENSORIN ALUSTUS
#include "i2c.h"
#include "i2c_BMP280.h"
BMP280 bmp280;

//DS1307-KELLOMODUULIN ALUSTUS
#define DS1307_I2C_ADDRESS 0x68
#include "Wire.h"
 
//DHT22-SENSORIN ALUSTUS
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//BLUETOOTHILLA LÄHETETTÄVIEN TEKSTIJONOJEN MUUTTUJAT
char junk;
String inputString = "";


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

//I2C-DATASIIRTOFUNKTIOIDEN MÄÄRITYS:
// Arduinolta kellolle:

void setDS1307time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // Asettaa DS1307:n ajan
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second)); // sekunnit
  Wire.write(decToBcd(minute)); // minuutit
  Wire.write(decToBcd(hour)); // tunnit
  Wire.write(decToBcd(dayOfWeek)); // viikonpäivät (1=Sunnuntai, 7=Lauantai)
  Wire.write(decToBcd(dayOfMonth)); // päivä (1 - 31)
  Wire.write(decToBcd(month)); // kuukausi
  Wire.write(decToBcd(year)); // vuosi (0 - 99)
  Wire.endTransmission();
}
 
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
 
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

//KELLONAJAN PRINTTAUS Serial-NÄYTÖLLE -FUNKTION MÄÄRITTELY:

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // hakee datan
  readDS1307time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // tulostaa Serial-näytön ensimmäiselle riville
  Serial.print(hour, DEC);
  // muuttaa datan binääristä kymmenjärjestelmään
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print(".");
  Serial.print(month, DEC);
  Serial.print(".");
  Serial.print(year, DEC);
  Serial.print(" ");
  switch(dayOfWeek){
  case 1:
    Serial.println("SU");
    break;
  case 2:
    Serial.println("MA");
    break;
  case 3:
    Serial.println("TI");
    break;
  case 4:
    Serial.println("KE");
    break;
  case 5:
    Serial.println("TO");
    break;
  case 6:
    Serial.println("PE");
    break;
  case 7:
    Serial.println("LA");
    break;
  }
}

float haeAika
  (int minuutti = 0,
  int sekunnit = 0,
  int tunnit = 0,
  int paiva = 0,
  int kuukausi = 0,
  int vuosi = 0)
{
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS1307time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
 if (sekunnit == 1)
 {
  return second, DEC;
 }
 else if (minuutti == 1)
 {
  return minute, DEC;
 }
 else if (tunnit == 1)
 {
  return hour, DEC;
 }
 else if (paiva == 1)
 {
  return dayOfMonth, DEC;
 }
 else if (kuukausi == 1)
 {
  return month, DEC;
 }
 else if (vuosi == 1)
 {
  return year, DEC;
 }
}

//ALUSTETAAN SENSORIT
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  dht.begin();
  bmp280.initialize();
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();
  
  // seuraavassa osassa asetetaan aika sulkujen sisään seuraavassa järjestyksessä, pilkulla erottaen:
  // sekunnit, minuutit, tunnit, päivämäärä, viikonpäivä (1 = SU, 2 = MA jne), kuukausi, vuosi (00-99)
  
  //setDS1307time(30,34,12,3,13,12,16);
/*
Tämä pitää kommentoida pois onnistuneen ajan asetuksen jälkeen,
ja tämän jälkeen sketsi ladataan uudestaan Arduinon muistiin.
Muuten Arduino asettaa ajaksi aina käynnistyessään funktioon asetetun ajan.
*/
}

void loop()
{
  
  displayTime(); // Aika sarjamonitorille
   
  /* TIEDOT BMP280:lta */
  bmp280.awaitMeasurement();
  float temperature280;
  bmp280.getTemperature(temperature280); 
  float pascal;
  bmp280.getPressure(pascal);
  float hPa = pascal / 100; //Pa -> hPa
  bmp280.triggerMeasurement();

  /*TIEDOT DHT22:lta */
  
  float temperature22 = dht.readTemperature();
  float humidity = dht.readHumidity() +7;

    /*ANTURIEN LUKEMIEN KESKIARVO*/

  float temperature = (temperature280 + temperature22) / 2;

  //Tulostetaan sarjamonitorille
  Serial.print(temperature22);
  Serial.println(" C");
  Serial.print(temperature280);
  Serial.println(" C");
  Serial.print(hPa);
  Serial.println(" hPa");
  Serial.print(humidity);
  Serial.println(" %");
  delay(250);
  
   
  float xminuutinvalein = (haeAika(0,1,0,0,0,0), DEC % 10);
  float tunninpaine[6] = {};
  float tunninlampo[6] = {};
  float tunninkosteus[6] = {};
  float keskipaine = (tunninpaine[0] + tunninpaine[1] + tunninpaine[2] + tunninpaine[3] + tunninpaine[4] + tunninpaine[5]) / 6;
  float keskilampo = (tunninlampo[0] + tunninlampo[1] + tunninlampo[2] + tunninlampo[3] + tunninlampo[4] + tunninlampo[5]) / 6; 
  float keskikosteus = (tunninkosteus[0] + tunninkosteus[1] + tunninkosteus[2] + tunninkosteus[3] + tunninkosteus[4] + tunninkosteus[5]) / 6;

  if (xminuutinvalein == 0)                            //Tallentaa joka kymmenes minuutti sensoreiden arvot talteen
    {
      if (haeAika(0,1,0,0,0,0) == 10)
      {
        tunninkosteus[0] = {humidity};
        tunninpaine[0] = {hPa};
        tunninlampo[0] = {temperature}; 
      }
      else if (haeAika(0,1,0,0,0,0) == 20)
      {
        tunninkosteus[1] = {humidity};
        tunninpaine[1] = {hPa};
        tunninlampo[1] = {temperature};
      }
      else if (haeAika(0,1,0,0,0,0) == 30)
      {
        tunninkosteus[2] = {humidity};
        tunninpaine[2] = {hPa};
        tunninlampo[2] = {temperature};
      }      
      else if (haeAika(0,1,0,0,0,0) == 40)
      {
        tunninkosteus[3] = {humidity};
        tunninpaine[3] = {hPa};
        tunninlampo[3] = {temperature};
      }      
      else if (haeAika(0,1,0,0,0,0) == 50)
      {
        tunninkosteus[4] = {humidity};
        tunninpaine[4] = {hPa};
        tunninlampo[4] = {temperature};
      }
      else if (haeAika(0,1,0,0,0,0) == 0)
      {
        tunninkosteus[5] = {humidity};
        tunninpaine[5] = {hPa};
        tunninlampo[5] = {temperature};
      }
}

  //Tarkistetaan onko Bluetooth-yhteyden yli tulossa tekstijonoja
  
  if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //luetaan tulevat merkit
      inputString += inChar;        //tehdään merkeistä merkkijono
    }
    Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // tyhjennetään sarjamonitorin puskuri

//SARJAMONITORILLE LÄHETETTÄVIÄ KÄSKYJÄ:
   
   //Arvioitu korkeus merenpinnasta:
    
    if (inputString == "korkeus")
    {
      float korkeus;
      float syotettyPaine;
      Serial.println("Anna paine merenpinnan tasolla:");
      Serial.flush(); //Tyhjennetään sarjamonitorin puskuri
      while(!Serial.available()); //Odotellaan syötettä
      syotettyPaine = Serial.parseFloat();
      Serial.println(syotettyPaine);
      korkeus = 44330 * (1.0 - pow(hPa/syotettyPaine, 0.1903));
      Serial.println("Arvioitu korkeus merenpinnasta");
      Serial.print(korkeus);
      Serial.println(" metria.");
      delay(5000);
    }

    if(inputString = "average") 
    {
      Serial.print("C: ");
      Serial.println(keskilampo);
      Serial.print("hPa: ");
      Serial.print(keskipaine);
      Serial.print("Hum:");
      Serial.print(keskikosteus);
      delay(5000);
    }

    if(inputString = "muuttujat")
    {
      Serial.print("Kosteus 1:");
      Serial.println(tunninkosteus[1]);
      Serial.print("Kosteus 2:");
      Serial.println(tunninkosteus[2]);
      Serial.print("Kosteus 3:");
      Serial.println(tunninkosteus[3]);
      Serial.print("Kosteus 4:");
      Serial.println(tunninkosteus[4]);
      Serial.print("Kosteus 5:");
      Serial.println(tunninkosteus[5]);
      Serial.print("Kosteus 6:");
      Serial.println(tunninkosteus[6]);
      Serial.print("\n \n");
      Serial.print("Lampotila 1:");
      Serial.println(tunninlampo[1]);
      Serial.print("Lampotila 2:");
      Serial.println(tunninlampo[2]);
      Serial.print("Lampotila 3:");
      Serial.println(tunninlampo[3]);
      Serial.print("Lampotila 4:");
      Serial.println(tunninlampo[4]);
      Serial.print("Lampotila 5:");
      Serial.println(tunninlampo[5]);
      Serial.print("Lampotila 6:");
      Serial.println(tunninlampo[6]);
      Serial.print("\n \n");
      Serial.print("Paine 1:");
      Serial.println(tunninpaine[1]);
      Serial.print("Paine 2:");
      Serial.println(tunninpaine[2]);
      Serial.print("Paine 3:");
      Serial.println(tunninpaine[3]);
      Serial.print("Paine 4:");
      Serial.println(tunninpaine[4]);
      Serial.print("Paine 5:");
      Serial.println(tunninpaine[5]);
      Serial.print("Paine 6:");
      Serial.println(tunninpaine[6]);
      delay(10000);
    }
       
    inputString = ""; //tyhjennetään muuttuja
  }
}
