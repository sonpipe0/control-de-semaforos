#ifndef CONFIG_H
#define CONFIG_H

#include <LiquidCrystal_I2C.h>
#include <ESP32Time.h>


extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const int mqtt_port;
extern unsigned int semaphore;
extern bool isInterrupted;
const int boton1 = 23;
const int boton2 = 34;


enum TrafficLightMode {
  NORMAL,
  OBSTRUCTED,
  PEDESTRIAN_REQUEST,
  OFF
};

struct TrafficLightParams {
  int redTime;
  int greenTime;
};

struct activeHours {
  String day;
  String startHour;
  String endHour;
};

extern activeHours activeHoursArray[7];

extern TrafficLightParams trafficLightParams;

void setTrafficLightMode(TrafficLightMode newMode);

extern TrafficLightMode mode;

extern ESP32Time rtc;

const int g = 2;
const int r = 15;
const int b = 0;
const int r1 =  32;
const int g1 = 33;
const int b1 = 25;
const int boton = 34;
extern LiquidCrystal_I2C lcd;  // set the LCD address to 0x3F for a 16 chars and 2 line display

extern SemaphoreHandle_t modeChangeSemaphore;
extern TaskHandle_t normalTaskHandle;
extern TaskHandle_t obstructedTaskHandle;
extern TaskHandle_t pedestrianRequestTaskHandle;
extern TaskHandle_t offTaskHandle;

#endif // CONFIG_H
