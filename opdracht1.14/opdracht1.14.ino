/*
  For Loop Iteration

  Demonstrates the use of a for() loop.
  Lights multiple LEDs in sequence, then in reverse.

  The circuit:
  - LEDs from pins 2 through 7 to ground

  created 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ForLoop
*/

int timer = 100;           // The LOWer the number, the slower the timing.
const int D1 = 13;
const int D4 = 10;

void setup() {
  Serial.begin(9600);
  // use a for loop to initialize each pin as an output:
  for (int thisPin = D4; thisPin <= D1; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }
}

void loop() {
  // loop from the lowest pin to the LOWest:
  for (int thisPin = D4; thisPin <= D1; thisPin++) {
    // turn the pin on:
    digitalWrite(thisPin, LOW);
    delay(timer);
    // turn the pin off:
    digitalWrite(thisPin, HIGH);
    Serial.println(thisPin);
  }

  // loop from the lowest pin to the lowest:
  for (int thisPin = D1 - 1; thisPin >= D4; thisPin--) {
    // turn the pin on:
    digitalWrite(thisPin, LOW);
    delay(timer);
    // turn the pin off:
    digitalWrite(thisPin, HIGH);
    Serial.println(thisPin);
  }
}
