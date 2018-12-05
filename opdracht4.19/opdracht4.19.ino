/*  20180923 Gert den Neijsel, Haagse Hogeschool
 *  16108051 Huub van Holsteijn, Haagse Hogeschool
    Dit programma demonstreert via de AVR/bitshift manier hoe de 7-segments LED displays
    van het Multi Function Shield kunnen worden aangestuurd.
    Als het programma werkt wordt het getal 3 weergegeven op het meest rechtse van de 4 displays.
    Voor meer informatie over de werking van het Multi Function Shield
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
  #define display0 0xF8 // 0b11111000 // LED display, meest rechter
  #define display1 0xF4 // 0b11110100 // LED display, 2e van rechts
  #define display2 0xF2 // 0b11110010 // LED display, 3e van rechts
  #define display3 0xF1 // 0b11110001 // LED display, meest linker

  //Dit zijn de cijfers 0 t/m 9 zoals weer te geven op de LED display
  uint8_t cijfer[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
*/

/*
  Opbouw 7-segments led display en segment namen (a t/m g en p van punt)
     --a
   f|  |b
     --g
   e|  |c
     --d
        .p

  Let op: een 1 betekent led segment uit, dus inverteren van de hele byte is nodig!

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

#define LATCH PD4 // Arduino pin 4
#define CLK PD7   // Arduino pin 7
#define DATA PB0  // Arduino pin 8

void setup() {
  /* "Data Direction Register" port B, C en D pinnen op de gewenste 'leesbare' AVR manier instellen,
    hiermee stel je in welke pinnen op PORTB als uitgang staan, niet benoemde pinnen zijn ingang:
  */
  DDRB = (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << DATA); // PB0 = DATA pin van 74HC595, andere pinnen zijn de leds.
  DDRC = 0; // "Data Direction Register C" = 0 zet alle pinnen op PORTC als input (is standaard al zo, dus hoeft eigenlijk niet).
  DDRD = (1 << LATCH) | (1 << CLK); // 'LATCH' (PD4) en 'CLK' (PD7) pinnen naar 74HC595 als uitgang instellen.
  PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2); // de 4 leds D1 t/m D4 uitzetten door pin hoog te maken.
}

void loop() {
  /* Let op: we gebruiken in dit programma 8 bits getallen, oftewel bytes.
    In de Arduino (=hobby) omgeving bestaat een datatype 'byte', maar die bestaat niet in de talen C en C++.
    We gebruiken daarom de voor C en C++ correcte benaming uint8_t; een 8-bits unsigned integer.
  */

  uint8_t segmenten = 0b10110000; // Dit stelt het cijfer 3 weer; zie uitleg in comments bovenaan
  uint8_t displaynr = 0b11111000; // meest rechter LED display; zie uitleg in comments bovenaan
  PORTD &= ~(1 << LATCH); // vertel de schuifregisters (74HC595 chips) dat er nieuwe data aankomt

  /* Eerst de data die het cijfer 3 voorstelt versturen. */
  schuifUit(segmenten);

  /* Nu de data die de aangeeft welk display wordt gebruikt versturen. */
  schuifUit(displaynr);

  PORTD |= (1 << LATCH);  // er is nieuwe data binnen, stuur display aan.

  while (1); // voor altijd in de wachtlus, niets meer doen
}


/* De functie schuifUit() moet je aanpassen waar aangegeven.
   Het vervangt de Arduino functie shiftOut() met een functie in AVR code (bitwise bewerkingen).
   Vul de missende 4 regels aan, blijf van de rest af. De rest van het programma werkt.
*/

void schuifUit(uint8_t waarde) {
  /* Alle 8 bits van 'waarde' langsgaan: */
  for (uint8_t n = 0; n < 8; n++) { 
    uint8_t machtvantwee = 0b10000000 >> n;
    
    /* Deze 'if' kijkt of het betreffende bitje in 'waarde' 1 is, zo ja dan is de voorwaarde TRUE.
      Extra uitleg: een 0 betekent boolean FALSE, alle andere waarden dan 0 betekenen boolean TRUE.
    */
    if (waarde & machtvantwee) {
      /* Maak de DATA pin hoog (set bit). Aanwijzing: de DATA pin valt onder PORTB */
      PORTB |= (1 << DATA);// vul deze regel aan  // DATA pin hoog maken (set bit)
    }
    else {
      /* Maak de DATA pin laag (clear bit). */
      PORTB &= ~(1 << DATA); ;// vul deze regel aan  // DATA pin laag maken (clear bit)
    }
    /* Hieronder komen twee regels.
       1ste: Maak de CLK pin hoog (set bit).
       2de:  Maak de CLK pin laag (clear bit).
       Aanwijzing: de CLK pin valt onder PORTD
    */
    PORTD |= (1 << CLK);    // CLK pin hoog maken
    PORTD &= ~(1 << CLK);   // CLK pin laag maken
  }
}
