/* Setup:
    */
char pins[]={2,3,4,5};
char shiftpin=6;
//char digit=0; //0: menos significativo.

char number=0;
/*long t0=millis();
long t1;*/

void initTimer2(){
  /* Desactivar interrupciones por overflow del contador*/
  TIMSK2  &= ~(1<<TOIE2);
  /* Activar interrupciones por Compare Match A*/
  TIMSK2 |= (1<<OCIE2A);
  /* Limpiar registros TCCR2A y TCCR2B*/
  TCCR2A = 0;
  TCCR2B = 0;
  /* Setear modo CTC (Clear Timer on Compare match): se resetea
  automáticamente el timer al igualarse al valor OCR2A por fijar*/
  TCCR2A |= (1<<WGM21);
  /* Configurar el prescaler a 1024. Con eso basta*/
  TCCR2B |= (1<<CS20);
  TCCR2B |= (1<<CS21);
  TCCR2B |= (1<<CS22);
  /* Setear el valor para el cual se generará una interrupción. con 128, se genera una interrupción cada 8,192 ms*/
  OCR2A=170;
}

ISR(TIMER2_COMPA_vect){
  /* se debe cambiar el display activo*/
  PORTD ^= 1 << shiftpin;
  PORTD &= B11000011;
  PORTD |= ((PORTD&B01000000? number/10 : number%10)<<pins[0]);
  //digit=~digit;//PORTD&(1<<shiftpin);
}

void setup(){
  for (int i=0;i<4; i++){
    pinMode(pins[i], OUTPUT);
  }
  pinMode(shiftpin,OUTPUT);
  //digitalWrite(shiftpin,HIGH);
  /* setea timer 2 para turnar displays*/
  cli();
  initTimer2();
  sei();
}
/*void setNum(char numero){
  number=numero
}*/
void loop(){
  //setNum(number);
  //delay(100);
  //number= number==99? 0 : number+1;
  /*t1=millis();
  if (t1-t0>=1000){
    number= number==9? 0 : number+1;
    setNum(number);
    t0=millis()-(t1-t0-1000);
  }*/
}
