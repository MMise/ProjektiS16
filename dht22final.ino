#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  float kosteus = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Lampotila = ");
  Serial.println(temp);
  Serial.print("Ilmankosteus = ");
  Serial.println(kosteus);
  delay(2000);
}
