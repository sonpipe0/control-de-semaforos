#include "mode_manager.h"
#include <Arduino.h>
#include "config.h"
#include "traffic_light_manager.h"

extern TrafficLightMode mode;

void runNormalMode(int greenTime, int redTime) {
    yellow(r1,g1,b1);
    if(mode != NORMAL) return;
    vTaskDelay(pdMS_TO_TICKS(2000));
    if(mode != NORMAL) return;
    green(r1,g1,b1);
    if(mode != NORMAL) return;
    vTaskDelay(pdMS_TO_TICKS(greenTime*1000));
    if(mode != NORMAL) return;
    yellow(r1,g1,b1);
    if(mode != NORMAL) return;
    vTaskDelay(pdMS_TO_TICKS(2000));
    if(mode != NORMAL) return;
    red(r1,g1,b1);
    if(mode != NORMAL) return;
    vTaskDelay(pdMS_TO_TICKS(1000));
    if(mode != NORMAL) return;
    yellow(r,g,b);
    vTaskDelay(pdMS_TO_TICKS(2000));
    if(mode != NORMAL) return;
    green(r,g,b);
    if(mode != NORMAL) return;
    vTaskDelay(pdMS_TO_TICKS(redTime*1000-3000));
    if(mode != NORMAL) return;
    yellow(r,g,b);
    if(mode != NORMAL) return;
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    if(mode != NORMAL) return;
    red(r,g,b);
    if(mode != NORMAL) return;
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void runObstructedMode() {
        if(mode != OBSTRUCTED) return;
        yellow(r1,g1,b1);
        if(mode != OBSTRUCTED) return;
        vTaskDelay(pdMS_TO_TICKS(2000));
        if(mode != OBSTRUCTED) return;
        off(r1,g1,b1);
        yellow(r,g,b);
        if(mode != OBSTRUCTED) return;
        vTaskDelay(pdMS_TO_TICKS(2000));
        if(mode != OBSTRUCTED) return;
        off(r,g,b);
}

void runPedestrianRequestMode(int greenTime, int redTime, int semaphore) {
  Serial.println("Pedestrian Request Mode");
  if (semaphore == 1) {
    red(r1,g1,b1);
    vTaskDelay( pdMS_TO_TICKS(5000));
  } else {
    red(r,g,b);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void runOffMode() {
    off(r1, g1, b1);
    off(r, g, b);
}


void green(int r, int g , int b){
  analogWrite(r,0);
  analogWrite(g,255);
  analogWrite(b,0);
}

void red(int r, int g , int b){
  analogWrite(r,255);
  analogWrite(g,0);
  analogWrite(b,0);
}

void yellow(int r, int g, int b){
  analogWrite(r,0);
  analogWrite(g,0);
  analogWrite(b,255);
}

void off(int r, int g, int b){
  analogWrite(r,0);
  analogWrite(g,0);
  analogWrite(b,0);
}
