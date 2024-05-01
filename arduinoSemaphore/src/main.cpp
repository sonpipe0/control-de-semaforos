#include "mqttConnection.cpp"
#include "wifi_mqtt_manager.h"
#include "traffic_light_manager.h"
#include "mode_manager.h"
#include "readButton.h"
#include <Arduino.h>

// Define constants and global variables here
const int g2 = 2;
const int r2 = 15;
const int b2 = 0;
const int r1 =  32;
const int g1 = 33;
const int b1 = 25;
const int boton = 35;

int buttonState = 0;

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
  connectToWifiAndMQTT(); // Connect to WiFi and MQTT
  pinMode(g2, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(boton, INPUT);

  // Create the traffic light task with default parameters
  // read MQTT messages and update trafficLightParams accordingly
  xTaskCreatePinnedToCore(redo, "ReconnectTask", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(readButtonTask, "ReadButtonTask", 1000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(trafficLightTask, "TrafficLightTask", 1000, &trafficLightParams, 1, NULL, 1);

  // Create other tasks here if needed
}

void loop() {
  // Main loop functionality here
  // Check for button presses or other inputs and update trafficLightParams accordingly
};

void trafficLightTask(void * parameter) {
  TrafficLightParams* params = (TrafficLightParams*) parameter;
  // Use params->redTime and params->greenTime in your task
  // ...
}


// Define the xTaskTrafficLightTask function here
