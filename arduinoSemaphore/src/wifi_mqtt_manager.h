#ifndef WIFI_MQTT_MANAGER_H
#define WIFI_MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>

extern PubSubClient MQTT_CLIENT;
extern WiFiClient WIFI_CLIENT;

void setupWifiAndMQTT();
void reconnect();
void keepMqttConnected(void * parameter);

#endif