#define MQTT_MAX_PACKET_SIZE 1024
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "wifi_mqtt_manager.h"
#include "mqtt_routes.h"

WiFiClient WIFI_CLIENT;
PubSubClient MQTT_CLIENT(WIFI_CLIENT);

void setupWifiAndMQTT() {
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  MQTT_CLIENT.setServer(mqtt_server, mqtt_port);
  MQTT_CLIENT.setCallback(callback);
  MQTT_CLIENT.setBufferSize(MQTT_MAX_PACKET_SIZE);
  reconnect();
}

void reconnect() {
  uint32_t chipId = ESP.getEfuseMac(); // Unique ID from chip
  char boardId[10];
  sprintf(boardId, "%08X", chipId);

  while (!MQTT_CLIENT.connected()) {
    Serial.println("Intentando conectar con MQTT.");
    if (MQTT_CLIENT.connect(boardId)) {
      subscribeToTopics(); // Call function to handle all subscriptions
    } else {
      Serial.println("Conexión fallida, intentando de nuevo en 3 segundos");
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
  Serial.println("Conectado a MQTT.");
}

void keepMqttConnected(void * parameter) {
  for(;;) {
    if (!MQTT_CLIENT.connected()) {
      reconnect();
    }
    MQTT_CLIENT.loop();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}