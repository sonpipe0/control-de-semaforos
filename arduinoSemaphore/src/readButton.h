#ifndef READ_BUTTON_H
#define READ_BUTTON_H

#include <Arduino.h>

unsigned int semaphore;

void readButtonTask(void *pvParameters);

#endif // READ_BUTTON_H