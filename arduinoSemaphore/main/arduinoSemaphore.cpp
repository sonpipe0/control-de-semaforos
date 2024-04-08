#include <WiFi.h> // Para ESP32
WiFiClient WIFI_CLIENT;
#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;
const int PIN_GREEN = 34;
const char* ssid = "UA-Alumnos";
const char* password = "41umn05WLC";
const int PIN_RED1 = 32;
const int PIN_BLUE1 = 33;

void callback(char* recibido, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido: ");
  Serial.print("   ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);

  }
  Serial.println();
}





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
  pinMode(PIN_GREEN,OUTPUT);
}

// Aquí configuramos lo que debe hacer cuando recibe un valor.

// Reconecta con MQTT broker
void reconnect() {
  MQTT_CLIENT.setServer("broker.hivemq.com", 1883);
  MQTT_CLIENT.setClient(WIFI_CLIENT);


  while (!MQTT_CLIENT.connected()) {
    Serial.println("Intentando conectar con MQTT.");

    if (MQTT_CLIENT.connect("XJXT061651656845165416")) { // Use a unique ID for each device
      MQTT_CLIENT.subscribe("XJXT06/aleatorio");
    } else {
      Serial.println("Conexión fallida, rc=" + String(MQTT_CLIENT.state()) + " intentando de nuevo en 3 segundos");
      // Wait before retrying
      delay(3000);
      
    }
  }

  Serial.println("Conectado a MQTT.");
}

void loop() {

    static int seconds = 0;
    String message = "hey how are you my friend " + String(seconds);
    MQTT_CLIENT.publish("XJXT06/aleatorio", message.c_str());


  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }
  MQTT_CLIENT.loop();
  digitalWrite(PIN_GREEN,HIGH);
  delay(3000);
  digitalWrite(PIN_GREEN,LOW);
  
  seconds += 3; // Chequea lo Subscrito.
}




