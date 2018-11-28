/* Gert, maart 2017; demo van 7-segments display op Multi Function Shield */
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

#define BEEP_PIN 3

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library

  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN,LOW); // bieper heel even aanzetten
  delay(25);  // 25 milliseconde
  digitalWrite(BEEP_PIN,HIGH); // bieper weer uitzetten
}

void loop() {
  MFS.write("Hi",0); // links uitlijnen
  delay(2000);
  MFS.write("Hi",1); // rechts uitlijnen
  delay(2000);
  MFS.write("HELP"); // kleine tekst demo
  delay(2000);
  MFS.write("ABCD"); // niet alle letters uit het alfabet zijn duidelijk weer te geven en alleen als hoofdletters.
  delay(2000);
  MFS.write(-273); // weergave negatief getal
  delay(2000);
  MFS.write(9); // cijfers worden standaard rechts uitgelijnd
  delay(2000);
  MFS.write('P',1); // weergave van letter 'P' op positie 1 (0=linker display, 3=rechter display)
  delay(2000);
  MFS.write(3.141f, 3);  // weergave met 3 cijfers achter de komma
  delay(2000);

  for(int counter = 2000; counter >= 0; counter--) { // count down timer
    MFS.write(counter);
    delay(5);
  }
  delay(100);
  
  MFS.write("    "); // scherm leegmaken
  for(int counter = 0; counter < 3; counter++) {  // night rider met puntjes
      for(int omhoog = 0; omhoog < 3; omhoog++){
        MFS.write('.',omhoog);
        delay(100);
        MFS.write(' ',omhoog);
      }
      for(int omlaag = 3; omlaag >= 0; omlaag--){
        MFS.write('.',omlaag);
        delay(100);
        MFS.write(' ',omlaag);
      }
  }

  MFS.write("End");
  MFS.blinkDisplay(DIGIT_ALL, ON); // display laten knipperen
  delay(4000);
  MFS.blinkDisplay(DIGIT_ALL, OFF); // display knipperen laten stoppen
}
