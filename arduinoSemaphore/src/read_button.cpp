#include "readButton.h"
#include "traffic_light_manager.h"
#include <ArduinoJson.h>
#include "wifi_mqtt_manager.h"
#include <time.h>

const int boton = 35;
const int boton1 = 34;

unsigned int semaphore = 0;
void readButtonTask() {
  for(;;) {
    if(digitalRead(boton) == HIGH) {
        semaphore = 0;
        DynamicJsonDocument message = generatePedestrianRequestJSON();
        String messageStr;
        serializeJson(message, messageStr);
        MQTT_CLIENT.beginPublish("pedestrian/request", messageStr.length(), true);
        MQTT_CLIENT.print(messageStr);
        MQTT_CLIENT.endPublish();
        setTrafficLightMode(PEDESTRIAN_REQUEST);
    }
    if(digitalRead(boton1) == HIGH) {
        semaphore = 1;
        setTrafficLightMode(PEDESTRIAN_REQUEST);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

DynamicJsonDocument generatePedestrianRequestJSON() {
    DynamicJsonDocument doc(1024);
    doc["semaphore"] = semaphore;
    doc["esp32_id"] = ESP.getEfuseMac();

    // Get the current time
    time_t now = time(nullptr);

    // Format the time as a string
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Set the time field in the JSON document
    doc["time"] = timeStr;

    return doc;
};