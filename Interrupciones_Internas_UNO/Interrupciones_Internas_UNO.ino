
const int led_pin = PB5;

//Counter and compare values
const uint16_t tl_load = 0;
const uint16_t tl_comp = 31250;


void setup() {
  //Set led pin as an OUTPUT
  DDRB |= (1<<led_pin);

  //Reset TIMER1 Control REG A
  TCCR1A = 0;

  //Set prescaler of 256
  TCCR1B |= (1<<CS12);
  TCCR1B &= ~(1<<CS11);
  TCCR1B &= ~(1<<CS10);

  //Reset Tim1 and set compare value
  TCNT1 = tl_load;
  OCR1A = tl_comp; 

  //Enable Tim1 compare interrupt
  TIMSK1 = (1 << OCIE1A);

  //Enable global interrupts
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}

//Interrupt Handler
ISR(TIMER1_COMPA_vect){
  TCNT1 = tl_load;
  PORTB ^=(1<<led_pin);
}





