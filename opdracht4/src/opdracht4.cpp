#include <Arduino.h>
#include <libArduino_avr_uno.a>

void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.println("Hi");
	delay(1000);
}
