#include <dht.h>
//määritellään sensorin pinni
dht DHT;

#define DHT11_PIN 2

void setup(){
  Serial.begin(9600);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Lampotila = ");
  Serial.println(DHT.temperature); //printataan sarjamonitorille lämpötila
  Serial.print("Ilmankosteus = ");
  Serial.println(DHT.humidity); //printataan sarjamonitorille ilmankosteus
  delay(1000);
}
