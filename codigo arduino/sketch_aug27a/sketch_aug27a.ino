#include <WiFi.h>
#include "ThingSpeak.h"

const char *ssid = "GALAXY12";         // Nombre de tu red Wi-Fi
const char *pass = "rgm12345";         // Contraseña de tu red Wi-Fi
unsigned long myChannelNumber = 2637553; // Número de tu canal en ThingSpeak
const char *myWriteAPIKey = "S3XB6ZAYMCRZE6H1"; // Tu clave API de escritura en ThingSpeak

WiFiClient client;

#define sensorPin A0  // Pin analógico donde está conectado el sensor de humedad del suelo

void setup() {
  Serial.begin(115200); // Iniciar el puerto serie
  WiFi.begin(ssid, pass); // Conectar al Wi-Fi
  ThingSpeak.begin(client); // Inicializar ThingSpeak

  Serial.println();
  Serial.print("Conectando a ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);

  // Leer el valor analógico del sensor de humedad del suelo
  int analogValue = analogRead(sensorPin);
  // Convertir el valor analógico (0-4095) a un porcentaje (0-100%)
  int porcentaje = map(analogValue, 0, 4095, 100, 0);

  // Asegurar que el porcentaje está dentro del rango de 0 a 100%
  if (porcentaje < 0) porcentaje = 0;
  if (porcentaje > 100) porcentaje = 100;

  // Imprimir el porcentaje de humedad del suelo
  Serial.print("Humedad del suelo: ");
  Serial.print(porcentaje);
  Serial.println("%");

  // Enviar datos a ThingSpeak
  ThingSpeak.setField(1, porcentaje); // Enviar el porcentaje de humedad al campo 1
  
  int result = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (result == 200) {
    Serial.println("¡Datos enviados a ThingSpeak!");
  } else {
    Serial.print("Error al enviar datos. Código de respuesta: ");
    Serial.println(result);
  }

  delay(15000); // Esperar 15 segundos antes de la próxima actualización
}

