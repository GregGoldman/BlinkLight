/*
 * FinalFlashlightCode.ino
 * Greg Goldman and Jason Fischell
 * BME 590: Medical Instrumentation Design
 * Fall, 2018
 * 
 * This code operates a flashlight on an ATTiny85 processor at a clock speed of 8 MHz
 * It is capable of 5 modes: low, medium, high, flash at high, and off
 * When off, the processor is put to sleep, reducing its current draw from a few mA
 * to less than 50 uA.
 * 
 * It uses interrupts to record button inputs, and it has rudimentary software debounce 
 * built in.
 * 
 * A great challenge with the debounce was that the timer resets after going to sleep, so
 * it was hard to tell the time since the last press if the device had slept. This was solved
 * by implementing a delay of the bounce wait time before going to sleeep. 
 * 
 * All time variables are in microseconds because micros() works in the ISR, but millis doesn't
 * 
 * In an unsigned long, there are 2^32 bits, so I can count to about 4.295 billion microseconds,
 * or about 4295 seconds, which is about 2.98 days before the timer count can roll over
 * 
 * Related content can be found at https://github.com/GregGoldman/BlinkLight
 */

//Includes
#include <avr/io.h> 
#include <avr/interrupt.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
#include <avr/sleep.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html
//https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/
// how to put attiny to sleep

int LEDPIN = 4;
int BUTTONPIN = 3;
bool Flash = LOW;
volatile int State = 0;


volatile int pressCount = 0;
/*
   0: off
   1: low
   2: med
   3: high
   4: 2Hz blink at high intensity
*/

///online sources suggest to declare variables which go in the ISR as volatile
unsigned long bounceTime = 50000; // how long to wait to avoid bounce: 200 ms
volatile unsigned long now = 0; //current time
unsigned long lastFlashTime = 0; // previous time
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
  // only increment the State at the top of the loop so it can't change during 
  // the Switch Tree or other inconvenient places
  if (pressCount > 0) {
    State += 1;
    if (State > 4) {
      State = 0;
    }
    pressCount = 0;
  }
  
  switch (State) {
    case 0:
      analogWrite(LEDPIN, 0);
      // the longest accurate microsecond delay is 16383 according to arduino.cc
      // as such, use delay() instead
      // this is one of the rare cases we are OK with a delay.
      delay(bounceTime/1000);
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_cpu();
      break;
    case 1:
      sleep_disable(); // not entirely sure this line is needed
      analogWrite(LEDPIN, 20);
      break;
    case 2:
      analogWrite(LEDPIN, 100);
      break;
    case 3:
      analogWrite(LEDPIN, 255);
      break;
    case 4:
      now = micros();
      if (now - lastFlashTime > 250000) {
        Flash = !Flash;
        lastFlashTime = now;
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
  now = micros();
  if (digitalRead(BUTTONPIN)) {
    if (now - lastPressedTime > bounceTime || State == 0) {
      pressCount += 1;
    }
  }
  lastPressedTime = micros();
}


