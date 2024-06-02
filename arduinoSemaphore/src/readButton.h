#ifndef READ_BUTTON_H
#define READ_BUTTON_H

#include <Arduino.h>

void readButtonTask(void *pvParameters);

void sendMqtt();

#endif // READ_BUTTON_H