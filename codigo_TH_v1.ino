#include <DHT.h>
#define DHTTYPE DHT22
 
const int DHTPin = 8;
 
DHT dht(DHTPin, DHTTYPE);
 
void setup() 
{
   Serial.begin(9600);
   Serial.println("Probando DHT22!");
 
   dht.begin();
}
 
void loop() 
{
   delay(2000);
 
   float h = dht.readHumidity();
   float t = dht.readTemperature();
 
   if (isnan(h) || isnan(t)) 
   {
      Serial.println("No se pudo leer el sensor DHT!");
      return;
   }
 
 
   Serial.print("Humedad: ");
   Serial.print(h);
   Serial.print(" %\t");
   Serial.print("Temperatura: ");
   Serial.print(t);
   Serial.println(" *C ");
}