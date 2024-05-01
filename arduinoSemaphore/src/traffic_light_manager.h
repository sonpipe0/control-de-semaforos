#ifndef TRAFFIC_LIGHT_MANAGER_H
#define TRAFFIC_LIGHT_MANAGER_H

#include <Arduino.h>

enum TrafficLightState {
  RED,
  YELLOW,
  GREEN,
};

enum TrafficLightMode {
  NORMAL,
  OBSTRUCTED,
  PEDESTRIAN_REQUEST,
  OFF
};

extern TrafficLightMode mode;

void trafficLightTask(void * parameter);
void setTrafficLightMode(TrafficLightMode newMode);

#endif // TRAFFIC_LIGHT_MANAGER_H
