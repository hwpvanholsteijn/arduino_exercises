int t;

void setup(){
  Serial.begin(9600);
}
void loop(){
  Serial.print("Time: ");
  t = millis();

  Serial.println(t);    //prints time since program started
  delay(1000);             // wait a second so as not to send massive amounts of data
}
