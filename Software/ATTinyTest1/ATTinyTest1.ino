#include <util/delay.h>


int LED = 4;
int Button = 3;
volatile int State = 0; // must be declared static
bool Flash = LOW;

/*
   0: off
   1: low
   2: med
   3: high
   4: 2Hz blink at low intensity
*/

unsigned long t1 = 0; //current time
unsigned long t2 = 0; // previous time

void setup() {
  pinMode(LED, OUTPUT);
  
}

void loop() {
  Flash = !Flash;
  digitalWrite(LED, Flash);
  delay(100); 
}
