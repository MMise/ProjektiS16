if (inputString == "korkeus")
{
  float korkeus;
  float syotettyPaine;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Odotetaan tietoja.")
  Serial.println("Anna paine merenpinnan tasolla:")
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

