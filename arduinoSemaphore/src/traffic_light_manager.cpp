#include "traffic_light_manager.h"
#include "mode_manager.h"
#include "readButton.h"
#include "config.h"

struct TrafficLightParams {
  int redTime;
  int greenTime;
};

void trafficLightTask(void * parameter) {
  mode = NORMAL;
  TrafficLightParams* params = (TrafficLightParams*) parameter;
  
  for (;;) {
    switch(mode) {
      case NORMAL:
        runNormalMode(params->greenTime, params->redTime); // Pass the parameters to the function
        break;
      case OBSTRUCTED:
        runObstructedMode();
        break;
      case PEDESTRIAN_REQUEST:
        runPedestrianRequestMode(params->greenTime, params->redTime, semaphore); 
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
