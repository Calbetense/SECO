//#define ledPin  13
#define NMuestras 1199

const byte ENCODER_1 = 7;
const byte ENCODER_2 = 3;

//Encoder States
int val_1_last = 0;
int val_2_last = 0;
int val_1_now = 0;
int val_2_now = 0;

//Encoder changes' counter
int count = 0;

int sampling[NMuestras];

//Sampling index
int i = 0;

void setup() {
  /******** Interrupts ******/
  pinMode(ENCODER_1, INPUT);
  pinMode(ENCODER_2, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1), encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_2), encoder, CHANGE);

  /********** TEST ***********/
  //pinMode(ledPin, OUTPUT);
  
  /****** PWM ******/
  pwm_setup();
}

void loop() {
  // update PWM frequency with  PWM->PWM_CH_NUM[0].PWM_CPRDUPD = ....;
  // update PWM duty cycle with PWM->PWM_CH_NUM[0].PWM_CDTYUPD = ....;
}


void pwm_setup(){
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                   // PWM power ON

  PIOC->PIO_PDR |= PIO_PDR_P3|PIO_PDR_P5;              // Set PWM pin to a peripheral
  PIOC->PIO_ABSR |= PIO_PC3B_PWMH0 | PIO_PC5B_PWMH1;   // Set PWM pin peripheral type B for PWMH0 (Arduino pin 35) and PWMH1 (Arduino pin 37)


  PWM->PWM_CLK = PWM_CLK_PREB(0) | PWM_CLK_DIVB(2);    // select Frequency for clock B: Mck/2 = 42 MHz
  
  PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKB;      // The period is left aligned, clock source as CLKB on channel 0
  PWM->PWM_CH_NUM[0].PWM_CPRD = 2100;                  // Set the PWM frequency 42 MHz/PWM_CPRD = 2000 Hz
  PWM->PWM_CH_NUM[0].PWM_CDTY = 1050;                  // Set the PWM duty cycle = (CPRD/CDTY) * 100 %

  PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKB;      // The period is left aligned, clock source as CLKB on channel 0
  PWM->PWM_CH_NUM[1].PWM_CPRD = 2100;                  // Set the PWM frequency 42 MHz/PWM_CPRD = 2000 Hz
  PWM->PWM_CH_NUM[1].PWM_CDTY = 0;                     // Set the PWM duty cycle = (CPRD/CDTY) * 100 %

  PWM->PWM_ENA |= PWM_ENA_CHID1 | PWM_ENA_CHID0;       // Enable PWM channel 0 and channel 1
}

/************* Encoder ISR ****************/
void encoder(){
   /*********** TEST ************/
   /*
  cuenta++;
  if(cuenta > 5000){
    digitalWrite(ledPin,1);
  }else{
    digitalWrite(ledPin,0);
  }*/

  /*********** Encoder states ************/
  val_1_last = val_1_now;
  val_2_last = val_2_now;
  
  val_1_now = digitalRead(ENCODER_1);
  val_2_now = digitalRead(ENCODER_2);

  /************** States ****************/
  if(val_1_now != val_1_last){
    if((val_1_now != val_2_now)){
      count++;
    }else{
      count--;
    }
  }else if(val_2_now != val_2_last) { 
    if((val_1_now != val_2_now)){
      count--;
    }else{
      count++;
    }
  }
}

/************** 1 ms ISR ***************/
void every_ms(){
  sampling[i] = count;
  i++;
}

