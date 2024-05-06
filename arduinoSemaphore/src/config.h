#ifndef CONFIG_H
#define CONFIG_H

#include <LiquidCrystal_I2C.h>



extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const int mqtt_port;
extern unsigned int semaphore;

const int g = 2;
const int r = 15;
const int b = 0;
const int r1 =  32;
const int g1 = 33;
const int b1 = 25;
const int boton = 34;
extern LiquidCrystal_I2C lcd;  // set the LCD address to 0x3F for a 16 chars and 2 line display

#endif // CONFIG_H
