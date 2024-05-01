#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <Arduino.h>


// Function prototypes
void runNormalMode(int greenTime, int redTime);
void runObstructedMode();
void runPedestrianRequestMode(int greenTime, int redTime, int semaphore);
void runOffMode();

// LED control functions
void green(int r, int g, int b);
void red(int r, int g, int b);
void yellow(int r, int g, int b);
void off(int r, int g, int b);

#endif // MODE_MANAGER_H
