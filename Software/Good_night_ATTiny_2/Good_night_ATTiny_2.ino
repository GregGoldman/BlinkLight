//Includes
#include <avr/io.h> // Not entirely sure what this does
#include <avr/interrupt.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
#include <avr/sleep.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html

//https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/
// how to put attiny to sleep

int LEDPIN = 4;
int BUTTONPIN = 3;
bool Flash = LOW;
volatile int State = 0;
volatile int lastState = 4;
/*
   0: off
   1: low
   2: med
   3: high
   4: 2Hz blink at high intensity
*/

unsigned long bounceTime = 200000;
volatile unsigned long t1 = 0; //current time
volatile unsigned long t2 = 0; // previous time
volatile unsigned long lastPressedTime = 0;

void setup() {
  // sets PB4 to output, all others to inputs
  DDRB  = 0b00010000;

  //disable pullup on PB3 and PB4
  PORTB = 0b11100111;

  // set all outputs to low first (default, likely unecessary)
  PINB = 0b00000000;

  //disable interrupts during setup
  cli();
  // choose PB 3 for your interrupt pin
  PCMSK = 0b00001000;
  // globally stage change interrupts to be enabled (still requires sei())
  GIMSK = 0b00100000;

  ADCSRA &= ~_BV(ADEN);  // ADC off

  //enable interrupts after setup
  sei();

}

void loop() {

  /*
     Because the timer resets whenever the attiny85 goes to sleep,
     I can't use the time difference to debounce from state 0

     The common case is that I'll press the button on State 4 (flash),
     and it will skip from zero (sleep) to 1 (low)

     It almost always only bounces once, which must be happening during
     the switch tree, so it jumps from State = 4 to State = 1;
     This jump from 4->0->1 must happen before another loop() completes.
     I know that the ATTiny85 is not getting to State = 0, going to sleep,
     and then waking back up on the bounce because I have a delayMicroseconds
     line inside of that case which should ignore that bounce.

     If I keep track of the last State called at the top of every loop,
     I'll know if the State has moved by more than 1 in the course of a
     single loop, which is the shortest amount of time for detection.
  */
  switch (State) {
    case 0:
      analogWrite(LEDPIN, 0);
      delayMicroseconds(bounceTime);
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_cpu();
      break;
    case 1:
      sleep_disable();
      if (lastState == 4){
        State = 0;
        break;
      }
      analogWrite(LEDPIN, 20);
      break;
    case 2:
      analogWrite(LEDPIN, 100);
      break;
    case 3:
      analogWrite(LEDPIN, 255);
      break;
    case 4:
      t1 = micros();
      if (t1 - t2 > 250000) {
        Flash = !Flash;
        t2 = t1;
      }
      if (Flash) {
        analogWrite(LEDPIN, 255);
      }
      else {
        analogWrite(LEDPIN, 0);
      }
      break;
    default:
      analogWrite(LEDPIN, 0);
      break;
  }
  lastState = State;
}

//Pin Change Interrupt Request 0 (don't look at nested interrupts)
ISR(PCINT0_vect) {
  // This will trigger on both rising and falling by default
  // need to read the state of the pin to tell if rising
  t1 = micros();
  if (digitalRead(BUTTONPIN)) {
    if (t1 - lastPressedTime > bounceTime || State == 0) {
      State = State + 1;
      if (State > 4) {
        State = 0;
      }
    }
  }
  lastPressedTime = micros();
}



void incrementState(){
  State+=1;
  if (State > 4){
    State = 0;
  }
}

