#include <OneWire.h>
#include <DallasTemperature.h>
 
// Asetetaan datajohto pinniin 2
#define ONE_WIRE_BUS 2
 
// Onewire kommunikoi sensorin kanssa
OneWire oneWire(ONE_WIRE_BUS);
 
// Referenssi Dallas temperatureen
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // sarjamonitori käyntiin
  Serial.begin(9600);

  // Sensori käyntiin
  sensors.begin();
}
 
 
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  sensors.requestTemperatures(); // Send the command to get temperatures

  Serial.print("Lampotila on: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("\n");
 
}
