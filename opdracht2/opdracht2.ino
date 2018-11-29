// Huub van Holsteijn, The Hague University 28 november 2018
//References: https://www.arduino.cc/reference/en/#functions
#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>


char HEXDIGITS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'F'};

const int BUTTON = A1;
bool buttonState;
bool lastButtonState = HIGH;

unsigned long beginTime;

//int guessingTime = 2000;

int score = 0;

const int GAMETIME = 2000;

int buttonPushes = 0;

int randomNum = 1;

//Sets up all the functional methods and sets an random.
void setup() {
  Serial.begin(9600);
  Timer1.initialize();
  MFS.initialize(&Timer1); // initialize multi-function shield library
  //randomSeed(analogRead(A5)); //Most random number on the multi-function shield, cause pins is unused.
  randomSeed(analogRead(A5)); // Pseudorandom
  /*Instances of the game*/
  MFS.write("Play");
  delay(2000);
  MFS.write("");
  randomNum = random(1, 15);
  MFS.write(HEXDIGITS[randomNum], 3);
  //MFS.write(randomNum);

  Serial.println("Start");
  beginTime = millis();
}

//loops sequence.
void loop() {
  if (!isTimeUp()) {
    if (buttonClick()) {
      runButtonFunction();
    }
  } else {
    if (randomGuessed()) {
      //displayTime();
      delay(2000);
      scoreIncrease();
      gameReset();
    } else {
      displayError();
      delay(2000);
      showGameScore();
      gameReset();
      instanceNewGame();
    }
  }
}

//Increases games score.
void scoreIncrease() {
  score++;
}

//Shows play display and begins new game with a new score.
void instanceNewGame() {
  score = 0;
  MFS.write("Play");
  delay(2000);
  displayReset();
}

//resets the game settings randomNumber and push counter.
void gameReset() {
  randomNum = getRandom();
  Serial.println(randomNum);
  buttonPushes = 0;
  displayReset();
}

//Display number and resets millis.
void displayReset() {
  displayHex(randomNum);
  beginTime = millis();
}

//Show games score animation with countup.
//source: blink exmaples
void showGameScore() {
  for (int i = 0; i <= score; i++) {
    delay(35);
    MFS.write(i);
  }
/*
  int BLINKTIME = 500;
  //blink score
  for (int i = 0; i < 4; i++) {
    delay(BLINKTIME);
    MFS.write("");
    delay(BLINKTIME);
    MFS.write(score);
  }
  delay(1500);*/
}

//Get random integer from 1 till 15.
int getRandom() {
  return random(1, 15);
}

//Display
//Source: https://www.programmingsimplified.com/c/source-code/c-program-convert-decimal-to-binary
// Opdracht 1b reader
void displayBinary(int decimalNumber) {
  int temp;
  for (int i = 0; i <= 3; i++) {
    temp = decimalNumber >> i;        // Binary Right Shift Operator. The left is an operator, the iterator (i) will be the number of bits that will be moved to the right on the bits of the operator.
    //Serial.println(temp);
    boolean bitd = temp & 1;          // binair getal en operator kopieren een bit naar het resultaat, wanneer het bit bestaat
    digitalWrite(i + 10, !bitd);
  }
  //Serial.println();
}

//Button A1 Click
//Source: button State example
bool buttonClick() {
  buttonState = digitalRead(BUTTON);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      lastButtonState = buttonState;
      return true;
    }
    delay(50);                                     //Small delay for debouncing
  }
  lastButtonState = buttonState;
  return false;
}

//Function when some pushes button A1, during numberguesser.
void runButtonFunction() {
  if (buttonPushes < 15) {
    buttonPushes++;
  } else {
    buttonPushes = 1;
  }
  //setGuessingTime();
  Serial.print("Knop is ");
  Serial.print(buttonPushes);
  Serial.println(" keer ingedrukt");
  displayBinary(buttonPushes);
  beginTime = millis();

}

//Display Hexadecimaldigit
void displayHex(int d) {
  MFS.write("");                  //Clear display;
  MFS.write(getHexDigit(d), 3);
}

//Leds an error message
void displayError() {
  MFS.write("Err");
}

//returns passed time from the start of a new game sequence.
unsigned long passedTime() {
  return millis() - beginTime;
}

//When passed time is greater than your max game time (2000ms) then it returns true
bool isTimeUp() {
  return (passedTime() >= GAMETIME);
}

//Returns true the randomnumber equals the buttonpushes
bool randomGuessed() {
  if (buttonPushes == randomNum) {
    return true;
  }
  return false;
}

//Returns a character of a hexadecimal until 15.
char getHexDigit(int d) {
  return HEXDIGITS[d];
}

/*
  void setGuessingTime(){
  guessingTime = passedTime();
  }

  void displayTime(){
  MFS.write(guessingTime);
  }

*/
