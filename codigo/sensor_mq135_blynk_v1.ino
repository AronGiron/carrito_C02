#define BLYNK_TEMPLATE_ID "TMPL2VoTShQvH"
#define BLYNK_TEMPLATE_NAME "templete"
#define BLYNK_AUTH_TOKEN "oB8uxMIAPUjHqsiIt0xT6syMbvSNp7xh"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Carro 2.0";
char pass[] = "12345678";

#define MQ135_PIN 34
#define RL 10000.0
#define ADC_RESOLUTION 4095.0
#define VCC 3.3
#define AIR_PPM 400
#define SLOPE -0.42

float R0 = 10000.0;

BlynkTimer timer;

float calibrateR0() {
  int numReadings = 100;
  float rsAvg = 0;

  for (int i = 0; i < numReadings; i++) {
    int adc = analogRead(MQ135_PIN);
    float voltage = adc * VCC / ADC_RESOLUTION;
    float rs = ((VCC * RL) / voltage) - RL;
    rsAvg += rs;
    delay(50);
  }

  rsAvg /= numReadings;
  float r0 = rsAvg / pow(AIR_PPM, SLOPE);
  Serial.print("R0 calibrado: ");
  Serial.println(r0);
  return r0;
}

void sendSensor() {
  int adc = analogRead(MQ135_PIN);
  float voltage = adc * VCC / ADC_RESOLUTION;
  float rs = ((VCC * RL) / voltage) - RL;
  float ratio = rs / R0;
  float ppm = pow(ratio, (1.0 / SLOPE));

  Serial.print("ADC: "); Serial.print(adc);
  Serial.print(" | Rs: "); Serial.print(rs);
  Serial.print(" | Rs/R0: "); Serial.print(ratio);
  Serial.print(" | CO2 estimado: "); Serial.print(ppm);
  Serial.println(" ppm");

  Blynk.virtualWrite(V0, ppm);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando...");

  WiFi.begin(ssid, pass);
  Serial.print("Conectando a Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Blynk.config(BLYNK_AUTH_TOKEN);
  if (Blynk.connect()) {
    Serial.println("✅ Blynk conectado");
  } else {
    Serial.println("❌ Error al conectar a Blynk");
  }

  delay(5000);
  R0 = calibrateR0();
  timer.setInterval(3000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
