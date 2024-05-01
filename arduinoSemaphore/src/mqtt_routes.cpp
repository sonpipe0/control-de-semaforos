#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "mqtt_routes.h"
#include "traffic_light_manager.h"
#include "mqttConnection.cpp"

void subscribeToTopics() {
  MQTT_CLIENT.subscribe("handler/obstruction/OHDSDHSODHIDHODIHD");
  MQTT_CLIENT.subscribe("active/hours/NAOSINSDNOAIDSN");
  MQTT_CLIENT.subscribe("shutdown/NAOSINSDNOAIDSN");
  MQTT_CLIENT.subscribe("turn/on/NAOSINSDNOAIDSN");
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (topic == "handler/obstruction/OHDSDHSODHIDHODIHD") {
    String message = readPayload(payload, length);
    if (message == "handle") {
        mode = OBSTRUCTED;
    }
    else{
        mode = NORMAL;
    }
} else if (topic == "active/hours/NAOSINSDNOAIDSN") {
    String message = readPayload(payload, length);
    parseJson(message);
} else if (topic == "shutdown/NAOSINSDNOAIDSN") {
    String message = readPayload(payload, length);
    if (message == "shutdown") {
        mode = OFF;
    }
} else if (topic == "turn/on/NAOSINSDNOAIDSN") {
    String message = readPayload(payload, length);
    if (message == "turn on") {
        mode = NORMAL;
    }
} else {
    Serial.println("Unknown topic");
    }

}

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
