//LCD-NÄYTÖN ALUSTUS
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x3F, 20, 4);

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

//MUUTTUJAT KESKIARVOJEN LASKENTAAN LASKENTAAN
float tunninpaine;
float tunninlampo;
float tunninkosteus;
float paivanpaine;
float paivanlampo;
float paivankosteus;
float keskipaine;
float keskilampo; 
float keskikosteus;
float keskipaineH;
float keskilampoH; 
float keskikosteusH;


//LASKURI
int i; //tunnin keskiarvoille
int j; //päivän keskiarvoille

//MUUTTUJAT, JOIHIN TALLENNETAAN MAKSIMI- JA MINIMIARVOT

float maxTemp;
float minTemp;
float maxPress;
float minPress;
float maxHum;
float minHum;

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
 
  *second = bcdToDec(Wire.read());
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read());
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


//ALUSTETAAN SENSORIT JA LCD-NÄYTTÖ
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd.begin();
  dht.begin();
  bmp280.initialize();
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();
  
  // seuraavassa osassa asetetaan aika sulkujen sisään seuraavassa järjestyksessä, pilkulla erottaen:
  // sekunnit, minuutit, tunnit, päivämäärä, viikonpäivä (1 = SU, 2 = MA jne), kuukausi, vuosi (00-99)
  
  /*setDS1307time(55,02,11,5,1,12,16);*/ 
/*
Tämä pitää kommentoida pois onnistuneen ajan asetuksen jälkeen,
ja tämän jälkeen sketsi ladataan uudestaan Arduinon muistiin.
Muuten Arduino asettaa ajaksi aina käynnistyessään funktioon asetetun ajan.
*/
}

void loop()
{
  /*määritellään aikamuuttujat uudestaan erillään funktioista, jotta saadaan ohjelma
  tekemään juttuja tietyllä kellonajalla*/
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // Asettaa DS1307:n rekisteriosoittimen osoitteeseen 00h
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // Pyytää DS1307:lta 7 bittiä dataa alkaen rekisteristä 00h
 
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read());
  dayOfWeek = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
  
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
  
  float temperature22 = dht.readTemperature();
  float humidity = dht.readHumidity() +7;

  /*ANTURIEN LUKEMIEN KESKIARVO*/

  float temperature = (temperature280 + temperature22) / 2;
  
  //Tulostetaan loputkin tiedot näytölle:
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 2);
  lcd.print(hPa);
  lcd.print(" hPa");
  lcd.setCursor(0, 3);
  lcd.print(humidity);
  lcd.print(" %");
  delay(500);

  if (minute % 10 == 0 && second == 0)
  {
    tunninpaine = tunninpaine + hPa;
    tunninlampo = tunninlampo + temperature;
    tunninkosteus = tunninkosteus + humidity;
    i++;
    delay(1000); //odotellaan hieman, jotta lukemia ei tallennu yhdelle mittauskerralle useita
  
  
    if (i == 6)
    {
      keskipaineH = tunninpaine / 6;
      keskilampoH = tunninlampo / 6; 
      keskikosteusH = tunninkosteus / 6;
      tunninpaine = 0;
      tunninlampo = 0;
      tunninkosteus = 0;
      i = 0;
    }
  }

  if (minute % 59 == 0 && second == 0)
  {
    paivanpaine = paivanpaine + hPa;
    paivanlampo = paivanlampo + temperature;
    paivankosteus = paivankosteus + humidity;
    j++;
    delay(1000);
    
    //Maksimien ja minimien tallentamiset
    
    if (j == 0) //Asetetaan sensorilukemat maksimi- ja minimiarvoiksi ensimmäisellä mittauskerralla
      {
        maxTemp = temperature;
        minTemp = temperature;
        maxPress = hPa;
        minPress = hPa;
        maxHum = humidity;
        minHum = humidity;
      }
      
      if (temperature > maxTemp)
      {
        maxTemp = temperature;
      }
      if (temperature < minTemp)
      {
        minTemp = temperature;
      }
      if (hPa > maxPress)
      {
        maxPress = hPa;
      }
      if (hPa < minPress)
      {
        minPress = hPa;
      }
      if (humidity > maxHum)
      {
        maxHum = humidity;
      }
      if (humidity < minHum)
      {
        minHum = humidity;
      }
  
    if (j == 24)
    {
        keskipaine = paivanpaine / 24;
        keskilampo = paivanlampo / 24; 
        keskikosteus = paivankosteus / 24;
        paivanpaine = 0;
        paivanlampo = 0;
        paivankosteus = 0;
        j = 0;
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
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Odotetaan tietoja.");
      Serial.println("Anna paine merenpinnan tasolla:");
      Serial.flush(); //Tyhjennetään sarjamonitorin puskuri
      while(!Serial.available()); //Odotellaan syötettä
      syotettyPaine = Serial.parseFloat();
      Serial.println(syotettyPaine);
      korkeus = 44330 * (1.0 - pow(hPa/syotettyPaine, 0.1903));
      lcd.setCursor(0,0);
      lcd.print("Arvioitu korkeus");
      lcd.setCursor(0,1);
      lcd.print("merenpinnasta:");
      lcd.setCursor(0,2);
      lcd.print(korkeus);
      lcd.print(" metria.");
      delay(5000);
      lcd.clear();
    }
   
   //Maksimit ja minimit:

   if(inputString == "maxmin")
   {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("MAX:");
     lcd.setCursor(12,0);
     lcd.print("MIN:");
     lcd.setCursor(0,1);
     lcd.print("C:");
     lcd.print(maxTemp);
     lcd.setCursor(12,1);
     lcd.print(minTemp);
     lcd.setCursor(0,2);
     lcd.print("hPa:");
     lcd.print(maxPress);
     lcd.setCursor(12,2);
     lcd.print(minPress);
     lcd.setCursor(0,3);
     lcd.print("Hum:");
     lcd.print(maxHum);
     lcd.setCursor(12,3);
     lcd.print(minHum);
     delay(10000);
     lcd.clear();  
   }
   
   //Tämänhetkisen tilanteen tulostaminen sarjamonitorille:
   
    if(inputString == "tilanne")
    {
      Serial.print(temperature);
      Serial.println(" C");
      Serial.print(hPa);
      Serial.println(" hPa");
      Serial.print(humidity);
      Serial.println(" %");
      delay(10000);
    }
   
   //Kellonajan muuttaminen
   
     
    if(inputString == "aseta")
    {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Aseta aika");
      lcd.setCursor(0,2);
      lcd.print("sarjamonitorissa!");
      Wire.beginTransmission(DS1307_I2C_ADDRESS);
      Wire.write(0);
      Serial.println("Anna sekunnit:");
      Serial.flush();
      while(!Serial.available());
      Wire.write(decToBcd(second = Serial.parseInt())); // sekunnit
      Serial.println(second);
      Serial.println("Anna minuutit:");
      Serial.flush();
      while(!Serial.available());
      Wire.write(decToBcd(minute = Serial.parseInt())); // minuutit
      Serial.println(minute);
      Serial.println("Anna tunnit:");
      Serial.flush();
      while(!Serial.available());
      Wire.write(decToBcd(hour = Serial.parseInt())); // tunnit
      Serial.println(hour);
      Wire.endTransmission();
    }

    if(inputString = "tunti") 
    {
      lcd.clear();
      Serial.print("Arvoja laskettu summaan:");
      Serial.println(i);
      Serial.println("ARVOJEN SUMMA:");
      Serial.print("C.sum: ");
      Serial.println(tunninlampo);
      Serial.print("hPa.sum: ");
      Serial.println(tunninpaine);
      Serial.print("Hum.sum: ");
      Serial.println(tunninkosteus);
      Serial.println("KESKIARVOT:");
      Serial.print("C: ");
      Serial.println(keskilampoH);
      Serial.print("hPa: ");
      Serial.println(keskipaineH);
      Serial.print("Hum:");
      Serial.println(keskikosteusH);
      lcd.setCursor(0,1);
      lcd.print("KESKIARVOT:");
      lcd.setCursor(0,2);
      lcd.print(keskilampoH);
      lcd.print(" C");
      lcd.setCursor(0,3);
      lcd.print(keskipaineH);
      lcd.print(" hPa");
      lcd.setCursor(0,4);
      lcd.print(keskikosteusH);
      lcd.print(" %");
      delay(10000);
      lcd.clear();
    }

    if(inputString = "paiva") 
    {
      lcd.clear();
      Serial.print("Arvoja laskettu summaan:");
      Serial.println(j);
      Serial.println("ARVOJEN SUMMA:");
      Serial.print("C.sum: ");
      Serial.println(paivanlampo);
      Serial.print("hPa.sum: ");
      Serial.println(paivanpaine);
      Serial.print("Hum.sum: ");
      Serial.println(paivankosteus);
      Serial.println("KESKIARVOT:");
      Serial.print("C: ");
      Serial.println(keskilampo);
      Serial.print("hPa: ");
      Serial.println(keskipaine);
      Serial.print("Hum:");
      Serial.println(keskikosteus);
      lcd.setCursor(0,1);
      lcd.print("KESKIARVOT:");
      lcd.setCursor(0,2);
      lcd.print(keskilampo);
      lcd.print(" C");
      lcd.setCursor(0,3);
      lcd.print(keskipaine);
      lcd.print(" hPa");
      lcd.setCursor(0,4);
      lcd.print(keskikosteus);
      lcd.print(" %");
      delay(10000);
      lcd.clear();
    }
   
   //maksimien ja minimien nollaus
   
   if (inputString == "nollaus")
    {
      lcd.clear();
      /*  
          Tässä asetetaan sensorilukemat uusiksi maksimeiksi ja minimeiksi.
          Jos arvoiksi asetettaisiin 0 ja laskuri j > 0 eivät maksimit ja
          minimit tallentuisi oikein.
      */
      maxTemp = temperature;
      minTemp = temperature;
      maxPress = hPa;
      minPress = hPa;
      maxHum = humidity;
      minHum = humidity;
      lcd.setCursor(0,1);
      lcd.print("Nollaus valmis!");
      delay(2500);
      lcd.clear();
    }
   //top kek -tier komennot
   
   if(inputString == "fitta")
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Om hade jag en");
      lcd.setCursor(0,1);
      lcd.print("pistol skulle jag");
      lcd.setCursor(0,2);
      lcd.print("satta en kula precis");
      lcd.setCursor(9,3);
      lcd.print("har");
      delay(5000);
      lcd.clear();
    }
  
   //itsetuho
   
   if(inputString == "meltdown")
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Selfdestruct");
    lcd.setCursor(0,1);
    lcd.print("sequence initiated");
    lcd.setCursor(9,3);
    delay(1000); 
    for(int k = 5; i < 1; i--)
    {
       lcd.print("T-");
       lcd.print(k);
       delay(1000);
    }
    lcd.setCursor(0,0);
    lcd.print("Sequence failed.");
    lcd.setCursor(0,1);
    lcd.print("Nuclear payload");
    lcd.setCursor(0,2);
    lcd.print("not detected.");
    delay(5000);
    lcd.clear();
   }
   
    inputString = ""; //tyhjennetään muuttuja
  }
}
