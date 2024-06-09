#include "readButton.h"
#include <ArduinoJson.h>
#include "wifi_mqtt_manager.h"
#include "config.h"




DynamicJsonDocument generatePedestrianRequestJSON() {
    DynamicJsonDocument doc(1024);
    doc["name"] = ESP.getEfuseMac();

    
    return doc;
};

void readButtonTask(void *pvParameters) {
  Serial.println("Read Button Task running...");
  int lastButtonState = HIGH;
  int lastButtonState2 = HIGH;
  for(;;) {
    int currentButtonState = digitalRead(boton1);
    if(currentButtonState == HIGH && lastButtonState == LOW) {
        semaphore = 0;
        sendMqtt();
        vTaskDelay(200 / portTICK_PERIOD_MS); // Debounce delay
    }
    int currentButtonState2 = false;
    if(currentButtonState2 == HIGH && lastButtonState2 == LOW) {
        semaphore = 1;
        sendMqtt();
        vTaskDelay(200 / portTICK_PERIOD_MS); // Debounce delay
    }
    lastButtonState = currentButtonState;
    lastButtonState2 = currentButtonState2;
    vTaskDelay(pdMS_TO_TICKS(100));
  }

}

void sendMqtt()
{
  DynamicJsonDocument message = generatePedestrianRequestJSON();
  String messageStr;
  serializeJson(message, messageStr);
  MQTT_CLIENT.publish("semaphore/pedestrian", messageStr.c_str(), false);
  Serial.println(messageStr);
}
