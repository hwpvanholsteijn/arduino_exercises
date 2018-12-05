/*  20180923 Gert den Neijsel, Haagse Hogeschool
    16108051 Huub van Holsteijn, Haagse Hogeschool
    Dit programma demonstreert via de Arduino manier hoe de 7-segments LED displays
    van het Multi Function Shield kunnen worden aangestuurd (nu alleen het rechter van de 4 displays).
    Voor meer informatie over de werking van het Multi Function Shield,
    zie het schema in de bijlage van de practicumhandleiding,
    of het pdf bestand in http://www.baaqii.com/promanage/manual/A1010.zip
    Zie ook de datasheet van de 74HC595
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

/*
  Opbouw 7-segments display en segment namen (a t/m g en p van punt)
     --a
   f|  |b
     --g
   e|  |c
     --d
        .p

  Let op: een 1 betekent segment uit, dus inverteren van de hele byte is nodig!

  segmenten: pgfe dcba
  0 = 0xC0 = 1100 0000
  1 = 0xF9 = 1111 1001
  2 = 0xA4 = 1010 0100
  3 = 0xB0 = 1011 0000
  4 = 0x99 = 1001 1001
  5 = 0x92 = 1001 0010
  6 = 0x82 = 1000 0010
  7 = 0xF8 = 1111 1000
  8 = 0x80 = 1000 0000
  9 = 0x90 = 1001 0000

  Voorbeeld berekening voor hoofdletter C:
  pgfe dcba
  0011 1001   alle bits inverteren; zie onder
  1100 0110 = 0xC6
*/

#define LATCH 4 // Arduino pin 4
#define CLK 7   // Arduino pin 7
#define DATA 8  // Arduino pin 8

byte segmenten[] = {
  0b11000000,
  0b11111001,
  0b10100100,
  0b10110000,
  0b10011001,
  0b10010010,
  0b10000010,
  0b11111000,
  0b10000000,
  0b10010000
};

void setup() {
  Serial.begin(9600);
  DDRB = (1 << PB0);
  DDRD = (1 << PD7) | (1 << PD4);
  /*
    for (int ledpin = 10; ledpin < 13; ledpin++) { // led pinnen op output zetten en leds uitzetten
      pinMode(ledpin, OUTPUT);                     // Welke docent is een slechte Arduino programmeur geweest ;)
      digitalWrite(ledpin, HIGH);
    }*/
}

void loop() {
  //byte segmenten = 0b10110000;                            // Dit stelt het cijfer 3 voor; zie uitleg in comments bovenaan
  byte displaynr = 0b11111000;                              // meest rechter LED display; zie uitleg in comments bovenaan
  for (int i = 0; i < sizeof(segmenten); i++) {             // For loop om alle nummers af te gaan <:o)
    PORTD &= ~(1 << LATCH);                                 // Zet de LATCH laag
    //digitalWrite(LATCH, LOW);                             // vertel de schuifregisters (74HC595 chips) dat er nieuwe data aankomt
    shiftOutAvr(DATA, CLK, MSBFIRST, segmenten[i]);         // schuif in één keer 8 bits data in; het weer te geven cijfer
    shiftOutAvr(DATA, CLK, MSBFIRST, displaynr);            // schuif in één keer 8 bits data in; het display nummer
    PORTD |= (1 << LATCH);                                  // Zet de LATCH Hoog
    //digitalWrite(LATCH, HIGH);                            // er is nieuwe data binnen, stuur display aan.
    delay(1000);
  }
  while (1); // voor altijd in de wachtlus, niets meer doen
}

/*
   self created shiftOut Method, referenced by https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout/
   supported by the basics from: http://playground.arduino.cc/Code/BitMath
   explanation: http://playground.arduino.cc/Code/BitMath
   Thanks to PaulS for helping on: http://forum.arduino.cc/index.php?topic=45529.0 writing_shift.c
*/
void shiftOutAvr(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value) {
  
  uint8_t i;                               //uint8_t is an unsigned integer with the length 8 bits //according to: https://forum.arduino.cc/index.php?topic=41590.0
  for (i = 0; i < 8; i++) {
    if (bitOrder == LSBFIRST) {
      PORTB = !!(value & (1 << i));        //digitalWrite(dataPin, !!(value & (1 << i)));  // schrijft de data naar poort B
    }
    else {
      PORTB = !!(value & (1 << (7 - i)));  //digitalWrite(dataPin, !!(value & (1 << (7 - i))));  // schrijft de data naar poort B
    }
    PORTD |= (1 << clockPin);              //Zet de Clockpin hoog
    PORTD &= ~(1 << clockPin);             //Zet de Clockpin laag

  }

}
/*
  void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
  {
     uint8_t i;

     for (i = 0; i < 8; i++)  {
           if (bitOrder == LSBFIRST)
                 digitalWrite(dataPin, !!(val & (1 << i)));
           else
                 digitalWrite(dataPin, !!(val & (1 << (7 - i))));

                   //serial for debug
                  Serial.print(i);
      Serial.print(":");
      Serial.print(dataPin);
      Serial.print(":");
      Serial.println(!!(value & (1 << (7 - i))));
      Serial.println(digitalRead(8))

           digitalWrite(clockPin, HIGH);
           digitalWrite(clockPin, LOW);
     }
  }
*/
