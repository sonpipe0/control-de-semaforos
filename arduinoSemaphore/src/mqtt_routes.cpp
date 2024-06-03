#define MQTT_MAX_PACKET_SIZE 1024  // Ajusta este valor según tus necesidades
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "mqtt_routes.h"
#include "wifi_mqtt_manager.h"
#include "config.h"

String readPayload(byte *payload, unsigned int length)
{
    String payloadStr = "";
    for (int i = 0; i < length; i++)
    {
        payloadStr += (char)payload[i];
    }
    return payloadStr;
}

int dayToInt(String day)
{
    if (day == "Monday")
    {
        return 0;
    }
    else if (day == "Tuesday")
    {
        return 1;
    }
    else if (day == "Wednesday")
    {
        return 2;
    }
    else if (day == "Thursday")
    {
        return 3;
    }
    else if (day == "Friday")
    {
        return 4;
    }
    else if (day == "Saturday")
    {
        return 5;
    }
    else if (day == "Sunday")
    {
        return 6;
    }
    return -1;
}

void setTrafficLightActiveHours(String day, String start, String end)
{
    int dayIndex = dayToInt(day);
    if (dayIndex == -1)
    {
        return;
    }
    activeHoursArray[dayIndex].day = day;
    activeHoursArray[dayIndex].startHour = start;
    activeHoursArray[dayIndex].endHour = end;
}

void parseJson(String message)
{
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, message);

    for (JsonObject elem : doc.as<JsonArray>()) {
        String day = elem["day"];
        String start = elem["open"];
        String end = elem["close"];
        setTrafficLightActiveHours(day, start, end);
    }
}

void changeSemaphoreStatus(String message)
{
    if (strcmp(message.c_str(), "NORMAL") == 0)
    {
        isInterrupted = false;
        xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
        mode = NORMAL;
        xTaskNotifyGive(normalTaskHandle);
        xSemaphoreGive(modeChangeSemaphore);
    }
    else if (strcmp(message.c_str(), "OBSTRUCTED") == 0)
    {
        isInterrupted = true;
        xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
        mode = OBSTRUCTED;
        xTaskNotifyGive(obstructedTaskHandle);
        xSemaphoreGive(modeChangeSemaphore);
    }
    else if (strcmp(message.c_str(), "OFF") == 0)
    {
        isInterrupted = true;
        xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY);
        mode = OFF;
        xTaskNotifyGive(offTaskHandle);
        xSemaphoreGive(modeChangeSemaphore);
    }
}

void subscribeToTopics()
{
    MQTT_CLIENT.subscribe("handler/obstruction");
    String topic = "semaphore/" + String(ESP.getEfuseMac()) + "/status";
    String changeActiveTimes = "semaphore/" + String(ESP.getEfuseMac()) + "/active/hours";
    MQTT_CLIENT.subscribe(changeActiveTimes.c_str());
    MQTT_CLIENT.subscribe(topic.c_str());
    MQTT_CLIENT.subscribe("active/hours");
    MQTT_CLIENT.subscribe("shutdown");
    MQTT_CLIENT.subscribe("turn/on");
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.println("Message arrived");
    Serial.print("Topic: ");
    Serial.println(topic);
    String modeTopic = "semaphore/" + String(ESP.getEfuseMac()) + "/status";
    String changeActiveTimes = "semaphore/" + String(ESP.getEfuseMac()) + "/active/hours";
    Serial.println(changeActiveTimes);
    if (strcmp(topic, "handler/obstruction") == 0)
    {
        Serial.println("Obstruction detected");
        String message = readPayload(payload, length);
        if (strcmp(message.c_str(), "handle") == 0)
        {
            Serial.println("Obstruction detected");
            mode = OBSTRUCTED;
        }
        else
        {
            mode = NORMAL;
        }
    }
    else if (strcmp(topic, modeTopic.c_str()) == 0)
    {
        String message = readPayload(payload, length);
        Serial.println(message);
        if (strcmp(message.c_str(), "NORMAL") == 0)
        {
            changeSemaphoreStatus(message);
        }
        else if (strcmp(message.c_str(), "OBSTRUCTED") == 0)
        {
            changeSemaphoreStatus(message);
        }
        else if (strcmp(message.c_str(), "OFF") == 0)
        {
            changeSemaphoreStatus(message);
        }
    }
    else if (strcmp(topic, changeActiveTimes.c_str()) == 0)
    {
        String message = readPayload(payload, length);
        parseJson(message);
        Serial.println("Active hours changed");
        Serial.print(message);
        //print new active hours
        for (int i = 0; i < 7; i++)
        {
            Serial.print("Day: ");
            Serial.print(activeHoursArray[i].day);
            Serial.print(" ");
            Serial.print("Start: ");
            Serial.print(activeHoursArray[i].startHour);
            Serial.print(" ");
            Serial.print("End: ");
            Serial.println(activeHoursArray[i].endHour);
        }
    }
    else{
        Serial.println("Unknown topic");
        Serial.println(topic);
    }
}
