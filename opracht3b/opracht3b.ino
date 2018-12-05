/*  20180923 Gert den Neijsel, Haagse Hogeschool
 *  16108051 Huub van Holsteijn, Haagse Hogeschool
    Dit programma simuleert (nadat de voorwaarde in regel 84 is afgemaakt) een "licht op de trap" schakeling;
    alleen als een oneven aantal schakelaars is ingedrukt dan brandt led D4 en klinkt de pieper.
    Druk de voorwaarde in regel 84 uit in termen van S1, S2 en S3.
*/

/*
  In de tabel hieronder staan de pinnen die het Multi Function Shield (MFS) gebruikt:
  Arduino pin, AVR pin, functie
  A0           PC0      potmeter (let op: analoge waarde)
  A1           PC1      schakelaar S1
  A2           PC2      schakelaar S2
  A3           PC3      schakelaar S3
   3           PD3      pieper
   4           PD4      led display LATCH
   7           PD7      led display CLK
   8           PB0      led display DATA
  10           PB2      led D4
  11           PB3      led D3
  12           PB4      led D2
  13           PB5      led D1
  Voor een mooi overzicht van de pinnen op de Arduino UNO zie:
  https://www.allaboutcircuits.com/uploads/articles/Arduino_UNO_R3_Pinout.jpg
*/
#define S1_PIN A1
#define S2_PIN A2
#define S3_PIN A3
#define D1_PIN 13
#define D2_PIN 12
#define D3_PIN 11
#define D4_PIN 10
#define BUZZER_PIN 3
int S1 = 0, S2 = 0, S3 = 0;

void setup() {
  pinMode(S1_PIN, INPUT);
  pinMode(S2_PIN, INPUT);
  pinMode(S3_PIN, INPUT);
  pinMode(D1_PIN, OUTPUT);
  pinMode(D2_PIN, OUTPUT);
  pinMode(D3_PIN, OUTPUT);
  pinMode(D4_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Even de pieper aanzetten om te testen
  delay(100);
  digitalWrite(BUZZER_PIN, HIGH); // Pieper weer uit.
  digitalWrite(D1_PIN, HIGH); // leds uitzetten
  digitalWrite(D2_PIN, HIGH);
  digitalWrite(D3_PIN, HIGH);
  digitalWrite(D4_PIN, HIGH);
}

void loop() {
  /* Regels hieronder: met de ! 'inverteer' je de boolean waarde; true wordt false en omgekeerd,
     zie https://www.arduino.cc/reference/en/language/structure/boolean-operators/logicalnot/
     Als je het Multi Function Shield gebruikt (pull-up weerstanden), dan moet je inverteren (NOT).
  */
  S1 = !digitalRead(S1_PIN);
  S2 = !digitalRead(S2_PIN);
  S3 = !digitalRead(S3_PIN);
  // Leds aanzetten als je knopjes indrukt:
  if (S1) {
    digitalWrite(D1_PIN, LOW);
  } else {
    digitalWrite(D1_PIN, HIGH);
  }
  if (S2) {
    digitalWrite(D2_PIN, LOW);
  } else {
    digitalWrite(D2_PIN, HIGH);
  }
  if (S3) {
    digitalWrite(D3_PIN, LOW);
  } else {
    digitalWrite(D3_PIN, HIGH);
  }
  /* Hieronder: opmerking bij het gebruik van variabelen als boolean zie
     https://www.arduino.cc/reference/en/language/variables/constants/constants/
     In de taal C komt 'false' overeen met 0 (nul) en 'true' is alles wat niet nul is (elk positief of negatief getal).
  */
  // Hier zit je eigenlijke logica, vul deze regel aan. Vervang de 'false' door termen van S1, S2 en S3:
  
  bool sumOfProducts = (S1 ^ S2 ^ S3) || (S1 && S2 && S3);
  // short version :(S1 ^ S2 ^ S3) || (S1 && S2 && S3)  (xor) or (and)
  // long version  :(S1 && !S2 && !S3) || (!S1 && S2 && !S3) || (!S1 && !S2 && S3) || (S1 && S2 && S3) 
  
  if (sumOfProducts) {
    digitalWrite(BUZZER_PIN, LOW);
  }
  else {
    digitalWrite(BUZZER_PIN, HIGH);
  }

  if (sumOfProducts) {
  // if (!S1) {
  // if (S1 || S2) {
    digitalWrite(D4_PIN, LOW); // led D4 aan
  }
  else {
    digitalWrite(D4_PIN, HIGH);
  }

}
