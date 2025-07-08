// Identificadores del proyecto Blynk
#define BLYNK_TEMPLATE_ID "TMPL2VoTShQvH"
#define BLYNK_TEMPLATE_NAME "templete"
#define BLYNK_AUTH_TOKEN "oB8uxMIAPUjHqsiIt0xT6syMbvSNp7xh"

#include <WiFi.h>                // Librería para conexión Wi-Fi
#include <BlynkSimpleEsp32.h>   // Librería de Blynk para ESP32

// Credenciales de la red Wi-Fi
char ssid[] = "Carro 2.0";
char pass[] = "12345678";

// Definición del pin del sensor MQ-135 y constantes del sensor
#define MQ135_PIN 34            // Pin analógico donde está conectado el sensor MQ-135
#define RL 10000.0              // Resistencia de carga en ohmios
#define ADC_RESOLUTION 4095.0   // Resolución del ADC del ESP32 (12 bits)
#define VCC 3.3                 // Voltaje de alimentación del sensor
#define AIR_PPM 400             // Concentración estándar de CO2 en aire limpio (ppm)
#define SLOPE -0.42             // Pendiente estimada de la curva del sensor

float R0 = 10000.0;             // Valor inicial de R0 (se calibrará)

BlynkTimer timer;               // Temporizador de Blynk para tareas periódicas

// Función para calibrar R0 (se recomienda hacerlo en aire limpio)
float calibrateR0() {
  int numReadings = 100;
  float rsAvg = 0;

  // Tomar múltiples lecturas para promediar Rs (resistencia del sensor)
  for (int i = 0; i < numReadings; i++) {
    int adc = analogRead(MQ135_PIN);
    float voltage = adc * VCC / ADC_RESOLUTION;
    float rs = ((VCC * RL) / voltage) - RL;
    rsAvg += rs;
    delay(50);
  }

  rsAvg /= numReadings;
  float r0 = rsAvg / pow(AIR_PPM, SLOPE);  // Cálculo del valor de R0
  Serial.print("R0 calibrado: ");
  Serial.println(r0);
  return r0;
}

// Función para leer el sensor y enviar datos a la app Blynk
void sendSensor() {
  int adc = analogRead(MQ135_PIN);                  // Leer valor analógico del sensor
  float voltage = adc * VCC / ADC_RESOLUTION;       // Convertir a voltaje
  float rs = ((VCC * RL) / voltage) - RL;            // Calcular Rs (resistencia del sensor)
  float ratio = rs / R0;                             // Rs dividido por R0
  float ppm = pow(ratio, (1.0 / SLOPE));             // Calcular concentración de CO2 estimada

  // Imprimir resultados en el monitor serial
  Serial.print("ADC: "); Serial.print(adc);
  Serial.print(" | Rs: "); Serial.print(rs);
  Serial.print(" | Rs/R0: "); Serial.print(ratio);
  Serial.print(" | CO2 estimado: "); Serial.print(ppm);
  Serial.println(" ppm");

  Blynk.virtualWrite(V0, ppm); // Enviar valor de CO2 a la app Blynk en el pin virtual V0
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando...");

  // Conexión Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Conectando a Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configuración de Blynk con el token de autenticación
  Blynk.config(BLYNK_AUTH_TOKEN);
  if (Blynk.connect()) {
    Serial.println("✅ Blynk conectado");
  } else {
    Serial.println("❌ Error al conectar a Blynk");
  }

  delay(5000);              // Esperar antes de calibrar
  R0 = calibrateR0();       // Calibrar el sensor
  timer.setInterval(3000L, sendSensor);  // Ejecutar sendSensor() cada 3 segundos
}

void loop() {
  Blynk.run();   // Ejecutar el loop de Blynk
  timer.run();   // Ejecutar funciones programadas con el temporizador
}
