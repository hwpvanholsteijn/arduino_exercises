/*Huub van Holsteijn 27 november 2018*/
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

const int D1 = 13;
const int D4 = 10;

int sizeButtons = 3;
const int BUTTONS[] = {A1, A2, A3};
int lastButtonState[] = {HIGH, HIGH, HIGH};
bool buttonState = LOW;

int hexIndex = 0;
int binaryIndex = 0;

const char HEXDIGITS[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D', 'E','F'};

int sizeLedPins = 4;
int ledPins[] = {
 10, 11, 12, 13
}; 

void setup() {
  Serial.begin(9600);

  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
  
  for (int i = 0; i < sizeLedPins; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  countClick();
}


void countClick(){
  for(int i = 0; i < sizeButtons; i++){
    buttonState = digitalRead(BUTTONS[i]);
    if (buttonState != lastButtonState[i]) {
      if (buttonState == HIGH) {
        runButtonFunction(BUTTONS[i]);
      }
      delay(50);                                     //Small delay for debouncing
    }
    lastButtonState[i] = buttonState;
  }
}

void runButtonFunction(int button){
  switch(button){
    case A1:
      nextBinary();
      decimalToBinary(binaryIndex);
    break;
    case A2:
      nextHex();
      displayHex(hexIndex);
      break;
    case A3:
      prevHex();
      displayHex(hexIndex);
      break;
  }
}

void nextBinary(){
  Serial.println("next binary");
  if(binaryIndex <= 15){
    binaryIndex++;
  }else{
    binaryIndex = 0;
  }
}

void decimalToBinary(int n){
    int temp;
    for(int i = 0; i <= 3; i++){
      temp = n >> i;                    // Binary Right Shift Operator. The left is an operator, the iterator (i) will be the number of bits that will be moved to the right on the bits of the operator.
      Serial.println(temp);
      boolean bitd = temp & 1;          // binair getal en operator kopieren een bit naar het resultaat, wanneer het bit bestaat 
      //Binary AND Operator copies a bit to the result if it exists in both operands. 
      digitalWrite(i + 10, !bitd);
    }
    Serial.println();
}
/*
 * Sources: https://www.programmingsimplified.com/c/source-code/c-program-convert-decimal-to-binary Subtitle: C program to convert decimal to binary
 * Led en display aansturen, Network en System Engineering reader, The Hague University
 */


void nextHex(){
 Serial.println("next Hex");
  if(hexIndex < 15){
    hexIndex++;
  }else{
    hexIndex = 0;
  }
}

void prevHex(){
 Serial.println("previous Hex");
  if(hexIndex > 0){
    hexIndex--;
  }else{
    hexIndex = 15;
  }
}


void displayHex(int n){
  MFS.write(HEXDIGITS[n],0);
}
