//Includes
#include <avr/io.h> // Not entirely sure what this does
#include <avr/interrupt.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
#include <avr/sleep.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html



#define INTERRUPTPIN PCINT3 //this is PB3 per the schematic
#define DATADIRECTIONPIN DDB1 //Page 64 of data sheet
#define PORTPIN PB1 //Page 64
#define READPIN PINB1 //page 64
#define LEDPIN 4 //PB4

//set bit in io register
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit)) //OR
//clear bit in io register
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) //AND

int LED = 4;
int Button = 3;
bool Flash = LOW; 
int State = 0;
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
  cli();//disable interrupts during setup
  pinMode(LEDPIN, OUTPUT); //we can use standard arduino style for this as an example
  digitalWrite(LEDPIN, LOW); //set the LED to LOW
  PCMSK |= (1 << INTERRUPTPIN); //sbi(PCMSK,INTERRUPTPIN) also works but I think this is more clear // tell pin change mask to listen to pin2 /pb3 //SBI
  GIMSK |= (1 << PCIE);   // enable PCINT interrupt in the general interrupt mask //SBI

  DDRB &= ~(1 << DATADIRECTIONPIN); //cbi(DDRB, DATADIRECTIONPIN);//  set up as input  - pin2 clear bit  - set to zero
  PORTB |= (1 << PORTPIN); //cbi(PORTB, PORTPIN);// disable pull-up. hook up pulldown resistor. - set to zero
  sei(); //last line of setup - enable interrupts after setup


  //https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/
  // how to put attiny to sleep

}

void loop() {
  t1 = millis();
  switch (State) {
    case 0:
      analogWrite(LED, 0);
      ADCSRA &= ~_BV(ADEN);                   // ADC off
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);   
      sleep_enable();
      sleep_cpu();
      break;
    case 1:
      analogWrite(LED, 40);
      sleep_disable();
      ADCSRA |= _BV(ADEN);                    // ADC on
      break;
    case 2:
      analogWrite(LED, 120);
      break;
    case 3:
      analogWrite(LED, 255);
      break;
    case 4:
      if (t1 - t2 > 250) {
        Flash = !Flash;
        t2 = t1;
      }
      if (Flash) {
        analogWrite(LED, 255);
      }
      else {
        analogWrite(LED, 0);
      }
      break;
    default:
      analogWrite(LED, 0);
      break;
  }
}

//Pin Change Interrupt Request 0 (don't look at nested interrupts)
ISR(PCINT0_vect) {
  // This will trigger on both rising and falling by default
  // need to read the state of the pin to tell if rising
  if (digitalRead(Button)) {
    State = State + 1;
    if (State > 4) {
      State = 0;
    }
  }

}
