
const int PIN_GREEN = 33;
void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_GREEN,OUTPUT);
}

// Aquí configuramos lo que debe hacer cuando recibe un valor.

// Reconecta con MQTT broker
void loop() {

    
  digitalWrite(PIN_GREEN,HIGH);
  delay(3000);
  digitalWrite(PIN_GREEN,LOW);
  delay(3000);
  
}





