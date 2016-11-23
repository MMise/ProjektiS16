# ProjektiS16
School project

Tänne kursseilla tehtyjä Arduino-koodeja ja muita tiedostoja, joissa ei ole henkilökohtaisia tietoja. Projektisuunnitelmat yms. OneDriven kautta.

2.11.2016
-v0.01b release

2.11.2016
Lisäsin kaksi prosessoriympäristöt-kurssilla väännettyä Arduino-koodia. -VV

3.11.2016
Lisätty DHT11-sensorin testauksessa käytetty koodi. Meidän sensorimme on mallia DHT22, mutta tämän koodinpätkän pitäisi toimia myös sillä. Koodi ja kirjasto peräisin osoitteesta: https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib, funktiot koodannut Rob Tillaart. -VV

3.11.2016
-Lisätty lampotila_v_0_69.ino DS18B20-sensorille.
(Ei käytössä lopullisessa tuotteessa)

4.11.2016
Lisätty BMP280-barometrin koodi, ei vielä testattu. Kirjasto ja koodi peräisin osoitteesta https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout/wiring-and-test -VV
HOX!! Ei saatu toimimaan, joten tätä ei käytetty lopullisessa tuotteessa.

4.11.2016
-OLED näyttö vaatii Adafruit SSD1306 ja GFX-kirjastot (löytyy arduinon library managerista)
(Näyttö liian pieni. Käytetään LCD-näyttöä, jonka kirjastot tulevat mukana Arduino IDE:ssä)

8.11.2016
Lisätty Prosessoriympäristöt-kurssilla tehty noppakoodi, jossa käytetään AVR timeriä antamaan satunnainen seed. CreateTrulyRandomSeed-funktio löydetty osoitteesta https://sites.google.com/site/astudyofentropy/project-definition/timer-jitter-entropy-sources/entropy-library/arduino-random-seed . Lisätty myös aikasemmasta noppa-sketsistä sovellettu sketsi, jossa summeri päästää äänen aina kun "noppaa" heitetään. Äänenkorkeutta voi muuttaa pyörittämällä säätövastusta. -VV

17.11.2016
Lisätty weatherstation_basic.ino. Wire.h-kirjasto tulee Arduino IDEn mukana. Koodin toimivuutta ei vielä testattu käytännössä. Tässä vaiheessa koodista löytyy toiminnallisuudet seuraaville sensoreille:

  -DHT22. Sovellettu Mikko Jokisen työstämän DHT11_lcd.ino-pohjalta. Sketsissä käytetyt funktiot ja kirjasto: kts. 3.11.2016
  
  -BMP280. Sovellettu Mikko Malilan työstämän ViolettiBPM260Koodi.ino-sketsin pohjalta. Sketsissä käytetty i2c.h-kirjasto ja
  funktiot löydetty Arduinon library managerin avulla.
  
  -DS1307. Tämän sensorin koodi toimii koko koodin runkona. Funktiot ja itse koodi peräisin osoitteesta http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/, tekijä John Boxall. Ville-Veikko Komulainen soveltanut koodia LCD-näyttöä ja suomalaista ajanesitystapaa varten. -VV
  
23.11.2016
Päivitetty weatherstation_basic-koodia vastaamaan I2C-sarjaväylällä kommunikoivaa LCD-näyttöä varten. LCD-näyttöä varten tarvittava kirjasto peräisin osoitteesta https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library, tekijä Frank de Brabander. -VV
