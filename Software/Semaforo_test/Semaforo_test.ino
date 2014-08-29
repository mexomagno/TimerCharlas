/* SETUP:
        Se trabaja mediante 2 shift registers
        -Pin 2 a SER
        -Pin 3 a RCLK
        -Pin 4 a SRCLK
        
*/
/************ CONSTANTES ************/
/************ VARIABLES *************/
/************ METODOS ***************/
void initProgram(){
  initDisplay(2,3,4);
  setTime(0,0,20);
  triggerAmarilla(0,0,15);
  triggerRoja(0,0,5);
  timerStart();
}
long t0,t1;
void setup(){
  initProgram();
  /*setDigit(1,0);
  setDigit(2,0);
  setDigit(3,3);
  setDigit(4,4);
  setDigit(5,5);
  setSemaforo('a');*/
  t0=millis();
}
char valor=0;
int dilei=500;
char indecs=0;
char colores[]={'v','a','r'};
void loop(){
  /*t1=millis();
  if (t1-t0>dilei){
    //setDigit(1,valor);
    //setDigit(2,valor);
    //setDigit(3,valor);
    //setDigit(4,valor);
    //setDigit(1,valor);
    indecs=(indecs+1)%3;
    setSemaforo(colores[indecs]);
    valor=(valor+1)%8;
    t0=t1;
  }*/
}
