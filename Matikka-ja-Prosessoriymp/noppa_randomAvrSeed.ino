#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>
// The following addresses a problem in version 1.0.5 and earlier of the Arduino IDE
// that prevents randomSeed from working properly.
// https://github.com/arduino/Arduino/issues/575
#define randomSeed(s) srandom(s)

volatile uint32_t seed; // These two variables can be reused in your program after the
volatile int8_t nrot; // function CreateTrulyRandomSeed()executes in the setup()
                    // function.

void CreateTrulyRandomSeed()
{
seed = 0;
nrot = 32; // Must be at least 4, but more increased the uniformity of the produced
// seeds entropy.
// The following five lines of code turn on the watch dog timer interrupt to create
// the seed value
cli();
MCUSR = 0;
_WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (1<<WDE);
_WD_CONTROL_REG = (1<<WDIE);
sei();

while (nrot > 0); // wait here until seed is created

// The following five lines turn off the watch dog timer interrupt
cli();
MCUSR = 0;
_WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (0<<WDE);
_WD_CONTROL_REG = (0<< WDIE);
sei();
}

ISR(WDT_vect)
{
nrot--;
seed = seed << 8;
seed = seed ^ TCNT1L;
}

void setup()
{
pinMode(11, INPUT);
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
Serial.begin(9600);
}

void loop() {
CreateTrulyRandomSeed();
randomSeed(seed);
if (digitalRead(11) == HIGH)
{
  seed = seed %6;
  if (seed == 0)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }

  else if (seed == 1)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
    else if (seed == 2)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
    else if (seed == 3)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
    else if (seed == 4)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  }
    else if (seed == 5)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
  }
  
}
}
