#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <Arduino.h>


const int g = 2;
const int r = 15;
const int b = 0;
const int r1 =  32;
const int g1 = 33;
const int b1 = 25;
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
