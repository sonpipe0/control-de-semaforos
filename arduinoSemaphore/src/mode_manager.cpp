#include "mode_manager.h"
#include <Arduino.h>
#include "config.h"

extern TrafficLightMode mode;
extern SemaphoreHandle_t modeChangeSemaphore;
extern TaskHandle_t normalTaskHandle;

void runNormalModeTask(void *pvParameters) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    TrafficLightParams *params = (TrafficLightParams *) pvParameters;
    int redTime = params->redTime;
    int greenTime = params->greenTime;
    
    
    while (mode == NORMAL) {
      yellow(r1, g1, b1);
      vTaskDelay(pdMS_TO_TICKS(2000));
      green(r1, g1, b1);
      vTaskDelay(pdMS_TO_TICKS(greenTime * 1000));
      yellow(r1, g1, b1);
      vTaskDelay(pdMS_TO_TICKS(2000));
      red(r1, g1, b1);
      vTaskDelay(pdMS_TO_TICKS(1000));
      
      yellow(r, g, b);
      vTaskDelay(pdMS_TO_TICKS(2000));
      green(r, g, b);
      vTaskDelay(pdMS_TO_TICKS(greenTime * 1000 - 3000));
      yellow(r, g, b);
      vTaskDelay(pdMS_TO_TICKS(2000));
      red(r, g, b);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

void runObstructedModeTask(void *pvParameters) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    while (mode == OBSTRUCTED) {
      yellow(r1, g1, b1);
      vTaskDelay(pdMS_TO_TICKS(2000));
      off(r1, g1, b1);
      yellow(r, g, b);
      vTaskDelay(pdMS_TO_TICKS(2000));
      off(r, g, b);
    }
  }
}

void runPedestrianRequestModeTask(void *pvParameters) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    TrafficLightParams *params = (TrafficLightParams *) pvParameters;
    int redTime = params->redTime;
    int greenTime = params->greenTime;
    
    if (mode == PEDESTRIAN_REQUEST) {
      if (semaphore == 1) {
        yellow(r1, g1, b1);
        vTaskDelay(pdMS_TO_TICKS(2000));
        red(r1, g1, b1);
        vTaskDelay(pdMS_TO_TICKS(redTime * 1000));
      } else {
        yellow(r, g, b);
        vTaskDelay(pdMS_TO_TICKS(2000));
        red(r, g, b);
        vTaskDelay(pdMS_TO_TICKS(redTime * 1000));
      }
    }
      xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
      mode = NORMAL;
      xTaskNotifyGive(normalTaskHandle);
      xSemaphoreGive(modeChangeSemaphore);

  }
}

void runOffModeTask(void *pvParameters) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    while (mode == OFF) {
      off(r1, g1, b1);
      off(r, g, b);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

void green(int r, int g, int b) {
  analogWrite(r, 0);
  analogWrite(g, 255);
  analogWrite(b, 0);
}

void red(int r, int g, int b) {
  analogWrite(r, 255);
  analogWrite(g, 0);
  analogWrite(b, 0);
}

void yellow(int r, int g, int b) {
  analogWrite(r, 0);
  analogWrite(g, 0);
  analogWrite(b, 255);
}

void off(int r, int g, int b) {
  analogWrite(r, 0);
  analogWrite(g, 0);
  analogWrite(b, 0);
}
