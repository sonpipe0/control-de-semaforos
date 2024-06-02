#include "readButton.h"
#include <ArduinoJson.h>
#include "wifi_mqtt_manager.h"
#include "config.h"




DynamicJsonDocument generatePedestrianRequestJSON() {
    DynamicJsonDocument doc(1024);
    doc["semaphore"] = semaphore;
    doc["esp32_id"] = ESP.getEfuseMac();

    // Get the current time
    
    int year = rtc.getYear();
    int month = rtc.getMonth();
    int day = rtc.getDay();
    int hour = rtc.getHour();
    int minute = rtc.getMinute();
    int second = rtc.getSecond();
    struct tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    timeinfo.tm_sec = second;
    char timeStr[20];
    strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
    Serial.println(timeStr);


    // Set the time field in the JSON document
    doc["time"] = timeStr;

    return doc;
};

void readButtonTask(void *pvParameters) {
  Serial.println("Read Button Task running...");
  int lastButtonState = HIGH;
  int lastButtonState2 = HIGH;
  for(;;) {
    int currentButtonState = digitalRead(boton1);
    // Serial.println(currentButtonState);
    if(currentButtonState == HIGH && lastButtonState == LOW) {
        semaphore = 0;
        sendMqtt();
        vTaskDelay(200 / portTICK_PERIOD_MS); // Debounce delay
    }
    int currentButtonState2 = digitalRead(boton2);
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
  MQTT_CLIENT.beginPublish("pedestrian/request", messageStr.length(), true);
  MQTT_CLIENT.print(messageStr);
  MQTT_CLIENT.endPublish();
  Serial.println(messageStr);
}
