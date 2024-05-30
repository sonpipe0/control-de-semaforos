#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "mqtt_routes.h"
#include "traffic_light_manager.h"
#include "wifi_mqtt_manager.h"

String readPayload(byte* payload, unsigned int length) {
  String payloadStr = "";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  return payloadStr;
}

void parseJson(String message) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, message);

    int startHour = doc["startHour"];
    int endHour = doc["endHour"];

    // Get the current hour
    time_t now = time(nullptr);
    tm* currentTime = localtime(&now);
    int currentHour = currentTime->tm_hour;

    // Check if the current hour is within the active hours
    if (currentHour >= startHour && currentHour < endHour) {
        mode = NORMAL;
    } else {
        mode = OFF;
    }
}

void subscribeToTopics() {
  MQTT_CLIENT.subscribe("handler/obstruction");
  MQTT_CLIENT.subscribe("active/hours");
  MQTT_CLIENT.subscribe("shutdown");
  MQTT_CLIENT.subscribe("turn/on");
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Message arrived");
    Serial.print("Topic: ");
    Serial.println(topic);
  if (strcmp(topic, "handler/obstruction") == 0) {
    Serial.println("Obstruction detected");
    String message = readPayload(payload, length);
    if (strcmp(message.c_str(), "handle") == 0) {
        Serial.println("Obstruction detected");
        mode = OBSTRUCTED;
    }
    else{
        mode = NORMAL;
    }
} else if (strcmp(topic, "active/hours") == 0){
    String message = readPayload(payload, length);
    parseJson(message);
} else if (strcmp(topic, "shutdown") == 0) {
    String message = readPayload(payload, length);
    if (message == "shutdown") {
        mode = OFF;
    }
} else if (strcmp(topic, "turn/on") == 0) {
    String message = readPayload(payload, length);
    if (message == "turn on") {
        mode = NORMAL;
    }
} else {
    Serial.println("Unknown topic");
    }

}


