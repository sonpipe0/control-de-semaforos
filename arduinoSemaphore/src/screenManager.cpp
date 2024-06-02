#include "screenManager.h"
#include "config.h"




String modeToString(TrafficLightMode mode) {
  switch(mode) {
    case NORMAL:
      return "Normal";
    case OBSTRUCTED:
      return "Obstructed";
    case PEDESTRIAN_REQUEST:
      return "Pedestrian";
    case OFF:
      return "Off";
    default:
      return "Unknown";
  }
};

void screenManagerTask(void *pvParameters) {
  for (;;) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Active Mode: ");
    lcd.setCursor(0, 1);
    lcd.print(modeToString(mode));

    
    vTaskDelay(4000 / portTICK_PERIOD_MS);
  }
};


