/*  20190923 Gert den Neijsel, Haagse Hogeschool
    Dit programma demonstreert op 2 manieren hoe pinnen uitgelezen en aangestuurd kunnen worden.
    Op de AVR manier: met AVR registers en bitwise operators (bitshift <<, not ~, and &, or |)
    Op de Arduino manier: staat onderaan als comments. 
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

void setup() {
  Serial.begin(9600);
  // "Data Direction Register B" instellen om pinnen op PORTB op output te zetten, niet genoemde pinnen staan op input:
  DDRB = (1 << PB4) | (1 << PB0);
  // Alle pinnen van PORTC OP input:
  DDRC = 0;
  // Pinnen waar led display op zit als uitgang instellen om "rommel" op display te voorkomen (PB0, PD4, PD7):
  DDRD = (1 << PD7) | (1 << PD4) | (1 << PD3);
  PORTD = (1 << PD7) | (1 << PD4); // alleen genoemde bits op 1 zetten om display uit te zetten.
}

void loop() {
  Serial.println(PB5);
  if ((~PINC & (1 << PC2)) || ( ~PINC & (1 << PC1))) {//Is schakelaar S2 ingedrukt?
    PORTB &= ~(1 << PB4);  //Zet de LED D1 aan
    PORTD &= ~(1 << PD3);  //Zet de pieper aan
  }
  else {
    PORTB |= (1 << PB4);   //Zet de LED D1 uit
    PORTD |= (1 << PD3);   //Zet de pieper uit
  }
}

/*
void setup() { // Arduino code. Voor de eenvoud pinnummers direct in de code gezet.
  // Pinnen op OUTPUT zetten. Pinnen die niet vermeld zijn staan automatisch op INPUT
  pinMode(13,OUTPUT); // led D1 / PB5
  pinMode(8,OUTPUT); // PB0 led display DATA
  pinMode(7,OUTPUT); // PD7 led display CLK
  pinMode(4,OUTPUT); // PD4 led display LATCH
  pinMode(3,OUTPUT); // PD3 pieper
  digitalWrite(7, HIGH); // Deze regel en de 2 hieronder: display uitzetten.
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
}

void loop() {
  if (!digitalRead(A1)) {//Is schakelaar S1 ingedrukt? (let op de ! = NOT; denk aan pull-up weerstand)
    digitalWrite(13,LOW);  //Zet de LED D1 aan
    digitalWrite(3,LOW);  //Zet de pieper aan
  }
  else {
    digitalWrite(13,HIGH);   //Zet de LED D1 uit
    digitalWrite(3,HIGH);   //Zet de pieper uit
  }
}
*/
