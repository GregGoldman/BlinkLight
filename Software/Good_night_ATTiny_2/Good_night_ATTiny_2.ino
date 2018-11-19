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
/*
   0: off
   1: low
   2: med
   3: high
   4: 2Hz blink at high intensity
*/

unsigned long t1 = 0; //current time
unsigned long t2 = 0; // previous time

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

  ADCSRA &= ~_BV(ADEN);                   // ADC off


  //enable interrupts after setup
  sei();
}

void loop() {
  t1 = millis();
  switch (State) {
    case 0:
      analogWrite(LEDPIN, 0);

      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_cpu();
      break;
    case 1:
      analogWrite(LEDPIN, 40);
      sleep_disable();
      break;
    case 2:
      analogWrite(LEDPIN, 120);
      break;
    case 3:
      analogWrite(LEDPIN, 255);
      break;
    case 4:
      if (t1 - t2 > 250) {
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
}

//Pin Change Interrupt Request 0 (don't look at nested interrupts)
ISR(PCINT0_vect) {
  // This will trigger on both rising and falling by default
  // need to read the state of the pin to tell if rising
  if (digitalRead(BUTTONPIN)) {
    State = State + 1;
    if (State > 4) {
      State = 0;
    }
  }
}
