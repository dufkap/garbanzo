void setup() {
  cli();
  DDRB = 0x08;    //Sets output for PWM pin
  DDRD = 0x68;    //Sets output for PWM pin
  DDRC = 0x30;    //Sets output for LED blinkers
  TCCR0A = 0xA1;  //TC in non-inverting mode, phase correct, counting to OCR0A, Prescaler 1
  TCCR0B = 0x01;  //TC in non-inverting mode, phase correct, counting to OCR0B, Prescaler 1
  TCCR2A = 0xA1;  //TC in non-inverting mode, phase correct, counting to OCR2A, Prescaler 1
  TCCR2B = 0x01;  //TC in non-inverting mode, phase correct, counting to OCR2B, Prescaler 1
  ADCSRA = 0xEF;
  ADCSRB = 0x00;
  ADMUX = 0x40;  //initializes A0
  sei();
  Serial.begin(9600);
}

volatile unsigned int MiddleSensor = 0;  //A0
volatile unsigned int LeftSensor = 0;    //A1
volatile unsigned int RightSensor = 0;   //A2
volatile unsigned char X = 0;            //Dummy variable
volatile unsigned char MuxArray[] = { 0x40, 0x41, 0x42 };

//>900 is on the black line
//<900 is on the white paper

void loop() {
  if (MiddleSensor > 900) {
    OCR0A = 0;
    OCR0B = 255;
    OCR2A = 0;
    OCR2B = 255;
  }

  if (LeftSensor > 900) {
    do {
      OCR0A = 0;
      OCR0B = 0;
      OCR2A = 0;
      OCR2B = 255;
      _delay_ms(50);
    } while (MiddleSensor < 900);
  }

  else if (RightSensor > 900) {
    do {
      OCR0A = 0;
      OCR2B = 0;
      OCR2A = 0;
      OCR0B = 255;
      _delay_ms(50);
    } while (MiddleSensor < 900);
  }


  while ((RightSensor < 900) & (LeftSensor < 900) & (MiddleSensor < 900)) {
    OCR0A = 0;
    OCR2B = 0;
    OCR2A = 0;
    OCR0B = 0;
  }
}
ISR(ADC_vect) {
  ADMUX = MuxArray[X];
  if (ADMUX == 0x40) {
    MiddleSensor = ADC;
  } else if (ADMUX == 0x41) {
    LeftSensor = ADC;
  } else if (ADMUX == 0x42) {
    RightSensor = ADC;
  }

  X++;
  if (X == 3) {
    X = 0;
  }
}