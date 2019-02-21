//Esto está probado con el arduino UNO
#define LedPin  6 //Aunque se use el analogWrite, hay que ponerlo en los pines digitales
#define PotPin  5 //Como es una entrada, este va a los pines analógicos

void setup() {
  pinMode(LedPin, OUTPUT);
  Serial.begin(9600);

 
}

void loop() {
  int val = analogRead(PotPin);
  analogWrite(LedPin, val/4);

  Serial.println(val); 
  delay(5);
}
