#define PinPot  EL QUE SEA
#define LED  EL LED QUE SEA

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED as an output.
  pinMode(LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  int pot = analogRead(PinPot);
  valt = map(val, 1, 1024, 0, 255);
  analogWrite(LED, val);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
}
