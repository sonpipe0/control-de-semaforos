#include "wifi_mqtt_manager.h"
#include "mode_manager.h"
#include "readButton.h"
#include "config.h"
#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "screenManager.h"
#include <NTPClient.h>
#include <ESP32Time.h>
#include <ultrasonido.h>
#include <ArduinoJson.h>

// Define constants and global variables here
const char *ssid = "fernandezc";
const char *password = "sanlorenzo2018";
const char *mqtt_server = "18.214.77.237";
const int mqtt_port = 1883;
unsigned int semaphore = 0;
TrafficLightMode mode = NORMAL;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ar.pool.ntp.org", -3 * 3600, 60000);
ESP32Time rtc(0);

SemaphoreHandle_t modeChangeSemaphore;
TaskHandle_t normalTaskHandle = NULL;
TaskHandle_t obstructedTaskHandle = NULL;
TaskHandle_t pedestrianRequestTaskHandle = NULL;
TaskHandle_t offTaskHandle = NULL;
bool isInterrupted = false;

// Define the structure for traffic light parameters
struct SemaphoreTiming
{
  int greenTime;
  int yellowTime;
  int redTime;
};

// Define the structure for MQTT message handling

void setTrafficLightMode(TrafficLightMode newMode)
{
  mode = newMode;
}

TrafficLightParams trafficLightParams = {5, 10}; // Default parameters
activeHours activeHoursArray[7] = {
  {"Monday", "00:00:00", "23:59:59"},
  {"Tuesday", "00:00:00", "23:59:59"}, 
  {"Wednesday", "00:00:00", "23:59:59"}, 
  {"Thursday", "00:00:00", "23:59:59"}, 
  {"Friday", "00:00:00", "23:59:59"},
  {"Saturday", "00:00:00", "23:59:59"},
  {"Sunday", "00:00:00", "23:59:59"}};

void IRAM_ATTR handleButtonPress()
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreTakeFromISR(modeChangeSemaphore, &xHigherPriorityTaskWoken);
  mode = PEDESTRIAN_REQUEST;

  // Notify the pedestrian request task to start
  vTaskNotifyGiveFromISR(pedestrianRequestTaskHandle, &xHigherPriorityTaskWoken);

  // Notify all other tasks to check the current mode and potentially stop
  vTaskNotifyGiveFromISR(normalTaskHandle, &xHigherPriorityTaskWoken);
  vTaskNotifyGiveFromISR(obstructedTaskHandle, &xHigherPriorityTaskWoken);
  vTaskNotifyGiveFromISR(offTaskHandle, &xHigherPriorityTaskWoken);

  if (xHigherPriorityTaskWoken)
  {
    portYIELD_FROM_ISR();
  }
  xSemaphoreGiveFromISR(modeChangeSemaphore, &xHigherPriorityTaskWoken);
}

void registerESP()
{
  // Register the ESP32 with the MQTT broker
  DynamicJsonDocument doc(1024);
  doc["name"] = ESP.getEfuseMac();
  String messageStr;
  serializeJson(doc, messageStr);
  MQTT_CLIENT.beginPublish("semaphore/create", messageStr.length(), true);
  MQTT_CLIENT.print(messageStr);
  MQTT_CLIENT.endPublish();
}

String intToDay(int day) {
  switch (day) {
    case 0:
      return "Sunday";
    case 1:
      return "Monday";
    case 2:
      return "Tuesday";
    case 3:
      return "Wednesday";
    case 4:
      return "Thursday";
    case 5:
      return "Friday";
    case 6:
      return "Saturday";
    default:
      return "Invalid day";
  }
}

int timeToSeconds(const String &time) {
  int hours = time.substring(0, 2).toInt();
  int minutes = time.substring(3, 5).toInt();
  int seconds = time.substring(6, 8).toInt();
  return hours * 3600 + minutes * 60 + seconds;
}

void checkActiveHours(void * parameter) {
  for(;;) {
    // Get the current time from the RTC

    Serial.println(isInterrupted);

    if(isInterrupted) continue;
    
    int currentDay = rtc.getDayofWeek();

    Serial.println("currentDay: " + String(currentDay));

    String currentTime = rtc.getTime();

    String currentDayString = intToDay(currentDay);

    int currentDayInt = currentDay == 0 ? 6 : currentDay - 1; 

    // Check if the current time is within the active hours
    activeHours currentDayActiveHours = activeHoursArray[currentDayInt];

    int currentTimeSeconds = timeToSeconds(currentTime);
    int startHourSeconds = timeToSeconds(currentDayActiveHours.startHour);
    int endHourSeconds = timeToSeconds(currentDayActiveHours.endHour);
    
    Serial.println("currentDayString: " + currentDayString);
    //print start and end from today
    Serial.println("currentHour" + currentTime);
    Serial.println("startHour" + currentDayActiveHours.startHour);
    Serial.println("endHour" + currentDayActiveHours.endHour);
    Serial.println("currentTimeSeconds: " + String(currentTimeSeconds));
    Serial.println("startHourSeconds: " + String(startHourSeconds));
    Serial.println("endHourSeconds: " + String(endHourSeconds));
    Serial.println(currentDayString.c_str());
    Serial.println(currentDayActiveHours.day.c_str());
    if (strcmp(currentDayString.c_str(), currentDayActiveHours.day.c_str()) == 0  && 
        startHourSeconds <= currentTimeSeconds && 
        endHourSeconds >= currentTimeSeconds) {
          Serial.println("Semaphore is active");
        // The semaphore is active
        xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
        mode = NORMAL;
        xTaskNotifyGive(normalTaskHandle);
        xSemaphoreGive(modeChangeSemaphore);
    } else {
        Serial.println("Semaphore is off");
        // The semaphore is inactive
        xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
        mode = OFF;
        xTaskNotifyGive(offTaskHandle);
        xSemaphoreGive(modeChangeSemaphore);
    }

    // Delay for 15 seconds
    vTaskDelay(pdMS_TO_TICKS(15000));
  }
}


void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  setupWifiAndMQTT(); // Connect to WiFi and MQTT

  registerESP();

  timeClient.begin();
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }

  rtc.setTime(timeClient.getEpochTime());

  // print the date and time saved in the RTC
  Serial.println(rtc.getDateTime(true));

  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(boton, INPUT);

  attachInterrupt(digitalPinToInterrupt(boton1), handleButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(boton2), handleButtonPress, FALLING);

  modeChangeSemaphore = xSemaphoreCreateMutex();

  // Create the traffic light task with default parameters
  // read MQTT messages and update trafficLightParams accordingly
  xTaskCreatePinnedToCore(keepMqttConnected, "ReconnectTask", 6000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(readButtonTask, "ReadButtonTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(screenManagerTask, "ScreenTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(ultraSoundTask, "UltraSoundTask", 3000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(runNormalModeTask, "NormalModeTask", 3000, (void *)&trafficLightParams, 1, &normalTaskHandle, 1);
  xTaskCreatePinnedToCore(runObstructedModeTask, "ObstructedModeTask", 3000, NULL, 1, &obstructedTaskHandle, 1);
  xTaskCreatePinnedToCore(runPedestrianRequestModeTask, "PedestrianRequestModeTask", 3000, (void *)&trafficLightParams, 1, &pedestrianRequestTaskHandle, 1);
  xTaskCreatePinnedToCore(runOffModeTask, "OffModeTask", 3000, NULL, 1, &offTaskHandle, 1);
  xTaskCreatePinnedToCore(checkActiveHours, "CheckActiveHours", 3000, NULL, 1, NULL, 1);

  xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
  mode = NORMAL;
  xTaskNotifyGive(normalTaskHandle);
  xSemaphoreGive(modeChangeSemaphore);
}

void loop() {
  // Main loop functionality here
  // Check for button presses or other inputs and update trafficLightParams accordingly
};

// Define the xTaskTrafficLightTask function here
