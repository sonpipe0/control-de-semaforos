#include <arduinoSemaphore.h>


const int GREEN = 2;
const int RED = 15;
const int BLUE = 0;
const int RED2 =  32;
const int GREEN2 = 33;
const int BLUE2 = 25;
const int boton = 35;

int buttonState = 0;



void setup() {
  Serial.begin(115200);
  connectToWifiAndMQTT();
  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(RED2,OUTPUT);
  pinMode(BLUE2,OUTPUT);
  pinMode(GREEN2,OUTPUT);
  pinMode(boton,INPUT);
}




void doWhileDelay(unsigned long delayDuration, void (*task)()) {
  unsigned long startMillis = millis();

  while (millis() - startMillis < delayDuration) {
    task(); // Execute the task
    delay(1); // Add a small delay to prevent the task from being executed too frequently
  }

  task(); // Execute the task one final time after the delay
}


void green(int r, int g , int b){
  analogWrite(r,0);
  analogWrite(g,255);
  analogWrite(b,0);
}

void red(int r, int g , int b){
  analogWrite(r,255);
  analogWrite(g,0);
  analogWrite(b,0);
}

void yellow(int r, int g, int b){
  analogWrite(r,0);
  analogWrite(g,0);
  analogWrite(b,255);
}


void semaphore(int r, int g, int b, int r1, int g1, int b1) {
  //Light 1 yellow for 2 secs
  yellow(r, g, b);
  doWhileDelay(2000, redo);

  // Light 1 red, Light 2 yellow
  red(r, g, b);

  doWhileDelay(1000, redo);
  yellow(r1, g1, b1);
  doWhileDelay(2000, redo);

  //light2 green for 9 seconds
  green(r1, g1, b1);

  // Light 1 stays red for 9 more seconds
  doWhileDelay(9000, redo);

  // Light 2 yellow for 2 seconds
  yellow(r1, g1, b1);
  doWhileDelay(2000, redo);
  

  // Light 2 red
  red(r1, g1, b1);
  doWhileDelay(1000, redo);

  // Light 1 yellow and then green after 1 second
  doWhileDelay(1000, redo);
  yellow(r, g, b);
  doWhileDelay(2000, redo);
  green(r, g, b);

  // Light 1 stays green while Light 2 is red
  doWhileDelay(7000, redo);

  
}



void loop() {
  semaphore(RED,GREEN,BLUE,RED2,GREEN2,BLUE2);
}