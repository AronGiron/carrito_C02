#include <DHT.h>           // Librería para el sensor DHT
#define DHTTYPE DHT22      // Tipo de sensor DHT que se está utilizando (DHT22)

const int DHTPin = 8;      // Pin digital donde está conectado el sensor DHT

DHT dht(DHTPin, DHTTYPE);  // Se crea un objeto del sensor DHT con el pin y tipo definidos

void setup() 
{
   Serial.begin(9600);           // Inicia la comunicación serial a 9600 baudios
   Serial.println("Probando DHT22!"); // Mensaje inicial

   dht.begin();                  // Inicializa el sensor DHT
}

void loop() 
{
   delay(2000);                  // Espera 2 segundos entre lecturas (recomendado para DHT22)

   float h = dht.readHumidity();     // Lee la humedad
   float t = dht.readTemperature();  // Lee la temperatura en grados Celsius

   // Verifica si hubo error en la lectura
   if (isnan(h) || isnan(t)) 
   {
      Serial.println("No se pudo leer el sensor DHT!");
      return;   // Sale del loop si hay error
   }

   // Imprime los valores leídos por el sensor
   Serial.print("Humedad: ");
   Serial.print(h);
   Serial.print(" %\t");
   Serial.print("Temperatura: ");
   Serial.print(t);
   Serial.println(" *C ");
}
