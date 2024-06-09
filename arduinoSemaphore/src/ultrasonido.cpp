#include <ultrasonido.h>
#include <wifi_mqtt_manager.h>
#include <ArduinoJson.h>


const int trigPin = 5;
const int echoPin = 18;
int distance = 0;
int numReadings = 100;

DynamicJsonDocument generateObstructionJSON() {
    DynamicJsonDocument doc(1024);
    doc["name"] = ESP.getEfuseMac();

    
    return doc;
};

void ultraSoundTask(void * parameter) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  while (true) {
    long readings[numReadings]; // the readings from the analog input
    long total = 0;          // the running total
    long average = 0;        // the average

    // take a number of readings and store them in the readings array
    for (int i = 0; i < numReadings; i++) {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      readings[i] = pulseIn(echoPin, HIGH) * 0.034 / 2;
      total += readings[i];
      delay(10);
    }

    // calculate the average of the readings
    average = total / numReadings;


    if(average < 50) {
        DynamicJsonDocument msg = generateObstructionJSON();
        String msgStr;
        serializeJson(msg, msgStr);
        MQTT_CLIENT.publish("semaphore/obstruction", msgStr.c_str(), false);
        Serial.println(msgStr);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}