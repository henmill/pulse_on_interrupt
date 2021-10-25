/* PULSE ON INTERRUPT
 * Produces a negative pulse on multiples of Timer1 interrupt, which is ~10us
 * A counter determines the multiples and is checked in the main loop
 * Rising edge interrupt on pin 2 resets the count as the means of syncing to an input clock
 * 
 * 
 */

const byte INPIN = 2;
const byte OUTPIN = 3;
volatile int cnt1 = 0;     //counter for t1 interrupt multiplier

void setup() {

cli();//stop interrupts
  
//set timer1 interrupt
  TCCR1A = 0;               // set entire TCCR1A register to 0
  TCCR1B = 0;               // set entire TCCR1B register to 0
  TCNT1  = 0;               //initialize counter value to 0
  // set compare match register for 100khz increments
  OCR1A = 19;            // = (16*10^6) / (100000*8) - 1 
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler --> 16MHz / 8 = 2MHz --> 1/2MHz = 0.5 x 10^-6 = 0.0000005 = 500ns
  TCCR1B |= (1 << CS11);    // set prescaler to div 8 0b00001010
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

sei();//allow interrupts

pinMode(OUTPIN, OUTPUT);
pinMode(INPIN, INPUT);
digitalWrite(OUTPIN, HIGH);   //initialize "outpin" to high, pulse will be low
attachInterrupt(digitalPinToInterrupt(INPIN), IOC, RISING);
}


void loop() {
  
  if ((cnt1 >= 31)&&(cnt1 < 60)){
    digitalWrite(OUTPIN, LOW);
  }
  else if((cnt1 >= 60)){
    digitalWrite(OUTPIN, HIGH);
  }
//  else if(cnt1 >= 200){
//    cnt1 = 0;
//  }

}

//timer1 interrupt 10us
ISR(TIMER1_COMPA_vect){   
  cnt1++;
}

// Interrupt On Change ISR
// as soon as pin goes high, this ISR is executed
void IOC()
{
  cnt1 = 0;
}
