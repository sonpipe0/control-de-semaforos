#include "readButton.h"
#include "traffic_light_manager.h"
#include <ArduinoJson.h>
#include "wifi_mqtt_manager.h"
#include "config.h"
#include <time.h>

const int boton1 = 34;

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

void readButtonTask(void *pvParameters) {
  Serial.println("Read Button Task running...");
  int lastButtonState = HIGH;
  for(;;) {
    int currentButtonState = digitalRead(boton);
    if(currentButtonState == HIGH && lastButtonState == LOW) {
        Serial.println("Boton presionado");
        semaphore = 0;
        DynamicJsonDocument message = generatePedestrianRequestJSON();
        String messageStr;
        serializeJson(message, messageStr);
        MQTT_CLIENT.beginPublish("pedestrian/request", messageStr.length(), true);
        MQTT_CLIENT.print(messageStr);
        MQTT_CLIENT.endPublish();
        setTrafficLightMode(PEDESTRIAN_REQUEST);
        vTaskDelay(200 / portTICK_PERIOD_MS); // Debounce delay
    }
    lastButtonState = currentButtonState;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

