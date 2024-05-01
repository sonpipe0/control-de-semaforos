#include "mode_manager.h"
#include <Arduino.h>
#include "config.h"

void runNormalMode(int greenTime, int redTime) {
    yellow(r1,g1,b1);
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    green(r1,g1,b1);
    vTaskDelay(greenTime*1000/ portTICK_PERIOD_MS);
    yellow(r1,g1,b1);
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    red(r1,g1,b1);
    vTaskDelay(1000/ portTICK_PERIOD_MS);
    yellow(r,g,b);
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    green(r,g,b);
    vTaskDelay((redTime*1000-3000)/portTICK_PERIOD_MS);
    yellow(r,g,b);
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    red(r,g,b);
    vTaskDelay(1000/ portTICK_PERIOD_MS);
}

void runObstructedMode() {
        yellow(r1,g1,b1);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        off(r1,g1,b1);
        yellow(r,g,b);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        off(r,g,b);
}

void runPedestrianRequestMode(int greenTime, int redTime,int semaphore) {
  Serial.println("Pedestrian Request Mode");
    if (semaphore == 1) {
      Serial.println("semaphore 1");
        vTaskDelay((greenTime*1000/ portTICK_PERIOD_MS)/2);
        yellow(r,g,b);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        red(r,g,b);
        vTaskDelay(4000/ portTICK_PERIOD_MS);
        yellow(r,g,b);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        green(r,g,b);
    } else {
        vTaskDelay((greenTime*1000/ portTICK_PERIOD_MS)/2);
        yellow(r1,g1,b1);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        red(r1,g1,b1);
        vTaskDelay(4000/ portTICK_PERIOD_MS);
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
