#include "wifi_mqtt_manager.h"
#include "traffic_light_manager.h"
#include "mode_manager.h"
#include "readButton.h"
#include "config.h"
#include <Arduino.h>

// Define constants and global variables here
const char* ssid = "Fliafc PLC";
const char* password = "sanlorenzo2018";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
unsigned int semaphore = 0;
TrafficLightMode mode = NORMAL;

// Define the structure for traffic light parameters
struct SemaphoreTiming {
  int greenTime;
  int yellowTime;
  int redTime;
};

// Define the structure for MQTT message handling
struct TrafficLightParams {
  int redTime;
  int greenTime;
  TrafficLightMode mode;
};

TrafficLightParams trafficLightParams = {5, 5, NORMAL}; // Default parameters

void setup() {
  Serial.begin(115200);
  setupWifiAndMQTT(); // Connect to WiFi and MQTT
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(boton, INPUT_PULLUP);

  // Create the traffic light task with default parameters
  // read MQTT messages and update trafficLightParams accordingly
  xTaskCreatePinnedToCore(keepMqttConnected, "ReconnectTask", 6000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(readButtonTask, "ReadButtonTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(trafficLightTask, "TrafficLightTask", 3000, (void*)&trafficLightParams, 1, NULL, 1);
  // Create other tasks here if needed
}

void loop() {
  // Main loop functionality here
  // Check for button presses or other inputs and update trafficLightParams accordingly
};




// Define the xTaskTrafficLightTask function here
