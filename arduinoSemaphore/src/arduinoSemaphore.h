#include <WiFi.h>
#include <Arduino.h> 
#include <PubSubClient.h>

WiFiClient WIFI_CLIENT;
PubSubClient MQTT_CLIENT;
const char* ssid = "Fliafc PLC";
const char* password = "sanlorenzo2018";

void callback(char* recibido, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido: ");
  Serial.print("   ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);

  }
  Serial.println();
}





void connectToWifiAndMQTT() {

  // Conectar con WiFi.
  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configuración de la respuesta.
  MQTT_CLIENT.setCallback(callback);
}

// Aquí configuramos lo que debe hacer cuando recibe un valor.

// Reconecta con MQTT broker
void reconnect() {
  MQTT_CLIENT.setServer("broker.hivemq.com", 1883);
  MQTT_CLIENT.setClient(WIFI_CLIENT);


  while (!MQTT_CLIENT.connected()) {
    Serial.println("Intentando conectar con MQTT.");

    if (MQTT_CLIENT.connect("XJXT061651656845165416")) { // Use a unique ID for each device
      MQTT_CLIENT.subscribe("XJXT06/aleatorio");
    } else {
      Serial.println("Conexión fallida, rc=" + String(MQTT_CLIENT.state()) + " intentando de nuevo en 3 segundos");
      // Wait before retrying
      delay(3000);
      
    }
  }

  Serial.println("Conectado a MQTT.");
}

void redo() {
  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }
  MQTT_CLIENT.loop();
}




