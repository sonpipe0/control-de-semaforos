#include "wifi_mqtt_manager.h"
#include "mode_manager.h"
#include "readButton.h"
#include "config.h"
#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "screenManager.h"
#include <NTPClient.h>
#include <ESP32Time.h>
#include <ultrasonido.h>
#include <ArduinoJson.h>

// Define constants and global variables here
const char* ssid = "Fliafc PLC";
const char* password = "sanlorenzo2018";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
unsigned int semaphore = 0;
TrafficLightMode mode = NORMAL; 
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ar.pool.ntp.org", -3 * 3600, 60000);
ESP32Time rtc(0);

SemaphoreHandle_t modeChangeSemaphore;
TaskHandle_t normalTaskHandle = NULL;
TaskHandle_t obstructedTaskHandle = NULL;
TaskHandle_t pedestrianRequestTaskHandle = NULL;
TaskHandle_t offTaskHandle = NULL;

// Define the structure for traffic light parameters
struct SemaphoreTiming {
  int greenTime;
  int yellowTime;
  int redTime;
};

// Define the structure for MQTT message handling


void setTrafficLightMode(TrafficLightMode newMode) {
  mode = newMode;
}

TrafficLightParams trafficLightParams = {5, 10}; // Default parameters

void IRAM_ATTR handleButtonPress() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreTakeFromISR(modeChangeSemaphore, &xHigherPriorityTaskWoken);
  mode = PEDESTRIAN_REQUEST;

  // Notify the pedestrian request task to start
  vTaskNotifyGiveFromISR(pedestrianRequestTaskHandle, &xHigherPriorityTaskWoken);

  // Notify all other tasks to check the current mode and potentially stop
  vTaskNotifyGiveFromISR(normalTaskHandle, &xHigherPriorityTaskWoken);
  vTaskNotifyGiveFromISR(obstructedTaskHandle, &xHigherPriorityTaskWoken);
  vTaskNotifyGiveFromISR(offTaskHandle, &xHigherPriorityTaskWoken);
  
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
  xSemaphoreGiveFromISR(modeChangeSemaphore, &xHigherPriorityTaskWoken);
}

void registerESP() {
  // Register the ESP32 with the MQTT broker
  DynamicJsonDocument doc(1024);
  doc["name"] = ESP.getEfuseMac();
  String messageStr;
  serializeJson(doc, messageStr);
  MQTT_CLIENT.beginPublish("semaphore/create", messageStr.length(), true);
  MQTT_CLIENT.print(messageStr);
  MQTT_CLIENT.endPublish();
}


void setup() {
  Serial.begin(115200);

  registerESP();

  
  lcd.init();
  lcd.clear();         
  lcd.backlight(); 
  
 
  
  
  setupWifiAndMQTT(); // Connect to WiFi and MQTT

  timeClient.begin();
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

   rtc.setTime(timeClient.getEpochTime());

   //print the date and time saved in the RTC
    Serial.println(rtc.getDateTime(true));
    
  
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(boton, INPUT);


  attachInterrupt(digitalPinToInterrupt(boton1), handleButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton2), handleButtonPress, FALLING);


  modeChangeSemaphore = xSemaphoreCreateMutex();

  // Create the traffic light task with default parameters
  // read MQTT messages and update trafficLightParams accordingly
  xTaskCreatePinnedToCore(keepMqttConnected, "ReconnectTask", 6000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(readButtonTask, "ReadButtonTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(screenManagerTask, "ScreenTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(ultraSoundTask, "UltraSoundTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(runNormalModeTask, "NormalModeTask", 3000, (void*)&trafficLightParams, 1, &normalTaskHandle, 1);
  xTaskCreatePinnedToCore(runObstructedModeTask, "ObstructedModeTask", 3000, NULL, 1, &obstructedTaskHandle, 1);
  xTaskCreatePinnedToCore(runPedestrianRequestModeTask, "PedestrianRequestModeTask", 3000, (void*)&trafficLightParams, 1, &pedestrianRequestTaskHandle, 1);
  xTaskCreatePinnedToCore(runOffModeTask, "OffModeTask", 3000, NULL, 1, &offTaskHandle, 1);

  xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
  mode = NORMAL;
  xTaskNotifyGive(normalTaskHandle);
  xSemaphoreGive(modeChangeSemaphore);

  
}

void loop() {
  // Main loop functionality here
  // Check for button presses or other inputs and update trafficLightParams accordingly
};





// Define the xTaskTrafficLightTask function here
