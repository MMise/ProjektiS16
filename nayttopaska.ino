#include <LiquidCrystal.h>
#include <dht.h>

#define DHT11_PIN 2

dht DHT;
LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );


 
void setup()
{
  lcd.begin(16, 2);
}
 
void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("TEMP = ");
  lcd.print(DHT.temperature);
  lcd.print("Hojojo"); //printataan sarjamonitorille lämpötila
  lcd.setCursor(0,1);
  lcd.print("HUM = ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(300);
}
