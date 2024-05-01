#ifndef MQTT_ROUTES_H
#define MQTT_ROUTES_H

void subscribeToTopics();
void callback(char* topic, byte* payload, unsigned int length);

#endif // MQTT_ROUTES_H
