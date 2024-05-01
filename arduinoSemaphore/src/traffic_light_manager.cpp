#include "traffic_light_manager.h"
#include "mode_manager.h"
#include "readButton.h"



void trafficLightTask(int redTime, int greenTime) {
  
  for (;;) {
    switch(mode) {
      case NORMAL:
        runNormalMode(greenTime, redTime); // Pass the parameters to the function
        break;
      case OBSTRUCTED:
        runObstructedMode();
        break;
      case PEDESTRIAN_REQUEST:
        runPedestrianRequestMode(greenTime, redTime, semaphore); 
        mode = NORMAL;
        break;
      case OFF:
        runOffMode();
        break;
    }
  }
}

void setTrafficLightMode(TrafficLightMode newMode) {
  mode = newMode;
}
