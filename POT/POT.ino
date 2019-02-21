//Esto está probado con el arduino UNO
#define LedPin  6 //Aunque se use el analogWrite, hay que ponerlo en los pines digitales
#define PotPin  5 //Como es una entrada, este va a los pines analógicos

void setup() {
  pinMode(LedPin, OUTPUT);
  Serial.begin(9600); //Con esto podemos ver con el monitor serie el valor real del potenciómetro (0 - 1024)
}

void loop() {
  int val = analogRead(PotPin);
  analogWrite(LedPin, val/4);   //La diferencia entre el valor de lectura (1024) y escritura (255) es aproximádamente de 4

  Serial.println(val); 
  delay(5); //No sé si realmente es necesario, pero bueno, por costumbre
}
