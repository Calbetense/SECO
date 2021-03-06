#include <DueTimer.h>


//Controler
#define COUNTS_REVOLUTION 3592
#define ONE_TICK          COUNTS_REVOLUTION/60
#define K_P               1 //Probar con distintos valores

#define Normalized_DTCY   2100/12

// PINS
const byte ENCODER_1 = 7;
const byte ENCODER_2 = 3;

//Encoder States
int val_1_last = 0;
int val_2_last = 0;
int val_1_now = 0;
int val_2_now = 0;

int count = 0;

//Seconds
int temp = 0;
int rotate = 1;

void setup() {
  
  pwm_setup();

  encoder_setup();

  timer_setup();  

}

void loop() {
//NOT USED
}

void pwm_setup(){
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                   // PWM power ON

  PIOC->PIO_PDR |= PIO_PDR_P3|PIO_PDR_P5;              // Set PWM pin to a peripheral
  PIOC->PIO_ABSR |= PIO_PC3B_PWMH0 | PIO_PC5B_PWMH1;   // Set PWM pin peripheral type B for PWMH0 (Arduino pin 35) and PWMH1 (Arduino pin 37)


  PWM->PWM_CLK = PWM_CLK_PREB(0) | PWM_CLK_DIVB(2);    // select Frequency for clock B: Mck/2 = 42 MHz
  
  PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKB;      // The period is left aligned, clock source as CLKB on channel 0
  PWM->PWM_CH_NUM[0].PWM_CPRD = 2100;                  // Set the PWM frequency 42 MHz/PWM_CPRD = 2000 Hz
  PWM->PWM_CH_NUM[0].PWM_CDTY = 0;                     // Set the PWM duty cycle = (CPRD/CDTY) * 100 %

  PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKB;      // The period is left aligned, clock source as CLKB on channel 0
  PWM->PWM_CH_NUM[1].PWM_CPRD = 2100;                  // Set the PWM frequency 42 MHz/PWM_CPRD = 2000 Hz
  PWM->PWM_CH_NUM[1].PWM_CDTY = 0;                     // Set the PWM duty cycle = (CPRD/CDTY) * 100 %

  PWM->PWM_ENA |= PWM_ENA_CHID1 | PWM_ENA_CHID0;       // Enable PWM channel 0 and channel 1
}

void encoder_setup(){
  pinMode(ENCODER_1, INPUT);
  pinMode(ENCODER_2, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1), encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_2), encoder, CHANGE);
}

void timer_setup(){                                     // Calls every 1ms
  Timer1.attachInterrupt(every_ms);
  Timer1.start(1000);                                   
}


/************* Encoder ISR ****************/
void encoder(){
  //Encoder States
  val_1_last = val_1_now;
  val_2_last = val_2_now;
  
  val_1_now = digitalRead(ENCODER_1);
  val_2_now = digitalRead(ENCODER_2);

  //Count
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

/************* Controler ****************/
void every_ms(){
  if(temp < 600){      //Estos valores son erróneos, hay que hacer una medida de lo que tarda el resto del código y pararlo 1s - lo que tarde el código
    temp++;
  }else if(temp < 400){
    temp = 0;
    rotate = !rotate;
  }

  if(rotate){     // La medición de lo que tarda el código se puede ver con la función millis() o micros()
  //Position
  double angularPosition = 2*PI*count/COUNTS_REVOLUTION;

  // Error
  double error = ONE_TICK - angularPosition;

  // Input
  double v = error * K_P;

  // Voltage safety
  if(v < 10){
    v = 12;
  }else if(u < -12){
    v = -12;
  }

  set_speed(v);

  }
}

void set_speed(int spd){
  PWM->PWM_CH_NUM[1].PWM_CDTYUPD = spd*Normalized_DTCY;
}

