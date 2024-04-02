#include <WiFi.h> // Para ESP32
WiFiClient WIFI_CLIENT;
#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

const char* ssid = "<ex>";
const char* password = "<passwd>";
#define PIN_RED1 = 32;
#define PIN_BLUE1 = 33;
#define PIN_GREEN1 = 34;



void setup() {
  Serial.begin(115200);
  delay(10);

  // Conectar con WiFi.
  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configuración de la respuesta.
  MQTT_CLIENT.setCallback(callback);
  pinMode(PIN_RED1,   OUTPUT);
  pinMode(PIN_GREEN1, OUTPUT);
  pinMode(PIN_BLUE1,  OUTPUT);
}

// Aquí configuramos lo que debe hacer cuando recibe un valor.
void callback(char* recibido, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido: ");
  Serial.print(recibido);
  Serial.print("   ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);

  }
  Serial.println();
}

void loop() {
  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }
  MQTT_CLIENT.loop(); // Chequea lo Subscrito.
}


// Reconecta con MQTT broker
void reconnect() {
     MQTT_CLIENT.setServer("broker.hivemq.com", 1883);

  MQTT_CLIENT.setClient(WIFI_CLIENT);

  // Intentando conectar con el broker.
  while (!MQTT_CLIENT.connected()) {
    Serial.println("Intentando conectar con MQTT.");
    MQTT_CLIENT.connect("XJXT061651656845165416");   // usar un nombre aleatorio
    //                      topic /  valor
    MQTT_CLIENT.subscribe("XJXT06/aleatorio");

    // Espera antes de volver a intentarlo.
    delay(3000);
  }

  Serial.println("Conectado a MQTT.");
}


void onGreen(){
  analogWrite(PIN_RED1,   0);
  analogWrite(PIN_GREEN1, 255);
  analogWrite(PIN_BLUE1,  0);

}

void offGreen(){
  analogWrite(PIN_RED1,   0);
  analogWrite(PIN_GREEN1, 0);
  analogWrite(PIN_BLUE1,  0);
}