#define zerocross 2       //pin for the external interrupt (Input with Pullup)
#define triac 3           //pin to trigger the Triac (Output)
#define ledred 4          //LED shows whether system is ready to softstart
#define ledgreen 5        //LED  shows whether softstart occured
#define push 6            //Push Button Input to start the softstart
bool zero = 0;            //variable as a status flag which shows whether a zero crossing point occured
bool pushinput = 0;       //variable as a status flag which shows whether push button was pushed
int i = 0;                //variable that decreases the time delay for the phase angle control

void setup() {
  pinMode(zerocross, INPUT_PULLUP);
  pinMode(triac, OUTPUT);
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(push, INPUT_PULLUP);
  digitalWrite(triac, LOW);
  digitalWrite(ledred, HIGH);
  digitalWrite(ledgreen, LOW);
  TCCR1A = 0;                                 //clear timer0 register (TCCR0A)            
  TCCR1B = 0;                                 //clear timer0 register (TCCR0B)
  TCCR1B |= (1 << CS12) | (1 << WGM12);       //set prescaler of timer0 to 256
  TIMSK1 |= (1 << OCIE1B);                    //activate timer compare match interrupt for OCR1B
  attachInterrupt(0, zerocrossing, RISING);   //activate external interrupt on pin 2 at a rising edge
  sei();                                      //activate interrupts
}

void zerocrossing () {                        //as soon as zero crossing occurs and push button was pushed, triac is turned off and the timer is set for the compare match interrupt whose delay slowly decreases through the variable i
  digitalWrite(triac, LOW);
  if (pushinput == 1) {
    OCR1B = 590 - i;
    TCNT1 = 0;
    zero = 1;
    i++;
    if (i == 588) {
      i = 587;
      digitalWrite(ledgreen, HIGH);
    }
  }
}

ISR(TIMER1_COMPB_vect) {                      //compare match interrupt that activates the triac
  if (zero == 1) {
    digitalWrite(triac, HIGH);
    zero = 0;
  }
}

void loop() {                                 
  if (digitalRead(push) == LOW) {
    pushinput = 1;
  }
}
