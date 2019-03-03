#define PotPin 5 //Potenciómetro


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  

  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                   // PWM power ON

  PIOC->PIO_PDR |= PIO_PDR_P3;                         // Set PWM pin to a peripheral
  PIOC->PIO_ABSR |= PIO_PC3B_PWMH0;                    // Set PWM pin peripheral type B for PWMH0 (Arduino pin 35)

  PWM->PWM_CLK = PWM_CLK_PREB(0) | PWM_CLK_DIVB(2);    // select Frequency for clock B: Mck/2 = 42 MHz
  PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKB;      // The period is left aligned, clock source as CLKB on channel 0
  PWM->PWM_CH_NUM[0].PWM_CPRD = 2100;                 // Set the PWM frequency 42 MHz/PWM_CPRD = 20 KHz
  PWM->PWM_CH_NUM[0].PWM_CDTY = 32768;                 // Set the PWM duty cycle = (CPRD/CDTY) * 100 %

  PWM->PWM_IER1 = PWM_IER1_CHID0;                      // Interrupt on PWM Channel 0 counter
  NVIC_EnableIRQ(PWM_IRQn);                            // Enable interrupt

  PWM->PWM_ENA = PWM_ENA_CHID0;                        // Enable PWM channel 0
}

void loop() {
  int val analogRead(PotPin);
  
  //PWM->PWM_CH_NUM[0].PWM_CPRDUPD = ....;              // update PWM frequency 
  PWM->PWM_CH_NUM[0].PWM_CDTYUPD = val/* hay que convertir este valor para que los rangos coincidan */;                // update PWM duty cycle 
}

void PWM_Handler() {
  static uint32_t Count;

  PWM->PWM_ISR1;      // Clear status register
  if (Count++ == 641)
  {
    PIOB->PIO_ODSR ^= PIO_ODSR_P27;
    Count = 0;
  }
}
