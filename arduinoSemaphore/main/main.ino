
const int PIN_R_2 = 15;
const int PIN_Y_2 = 2;
const int PIN_G_2 = 12;
const int PIN_R_1 = 25;
const int PIN_Y_1 = 23;
const int PIN_G_1 = 21;






void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_G_2,OUTPUT);
  pinMode(PIN_Y_2,OUTPUT);
  pinMode(PIN_R_2,OUTPUT);
  pinMode(PIN_R_1,OUTPUT);
  pinMode(PIN_Y_1,OUTPUT);
  pinMode(PIN_G_1,OUTPUT);
}

// Aquí configuramos lo que debe hacer cuando recibe un valor.

// Reconecta con MQTT broker
void loop() {

    
  semaphore1Loop();
  semaphore2Loop();
  
}


void semaphore1Loop(){

  digitalWrite(PIN_R_1,HIGH);
  delay(5000);
  digitalWrite(PIN_R_1,LOW);
  digitalWrite(PIN_Y_1,HIGH);
  delay(1500);
  digitalWrite(PIN_Y_1,LOW);
  digitalWrite(PIN_G_1,HIGH);
  delay(7000);
  digitalWrite(PIN_G_1,LOW);
}

void semaphore2Loop(){

  digitalWrite(PIN_R_2,HIGH);
  delay(5000);
  digitalWrite(PIN_R_2,LOW);
  digitalWrite(PIN_Y_2,HIGH);
  delay(1500);
  digitalWrite(PIN_Y_2,LOW);
  digitalWrite(PIN_G_2,HIGH);
  delay(7000);
  digitalWrite(PIN_G_2,LOW);
}
