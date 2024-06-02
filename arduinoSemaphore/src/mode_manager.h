#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <Arduino.h>


// Function prototypes
void runNormalModeTask(void *pvParameters);
void runObstructedModeTask(void *pvParameters);
void runPedestrianRequestModeTask(void *pvParameters);
void runOffModeTask(void *pvParameters);

// LED control functions
void green(int r, int g, int b);
void red(int r, int g, int b);
void yellow(int r, int g, int b);
void off(int r, int g, int b);

#endif // MODE_MANAGER_H
