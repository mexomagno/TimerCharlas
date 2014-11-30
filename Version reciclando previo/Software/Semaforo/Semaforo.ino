/* SETUP:
        Se trabaja mediante 2 shift registers
        -Pin 2 a SER
        -Pin 3 a RCLK
        -Pin 4 a SRCLK
       
        3 botones configuran
        -Pin 5 boton SET
        -Pin 6 boton UP/START
        -Pin 7 boton DOWN/STOP
        Usar capacitores para debounce
        
*/
/************ CONSTANTES ************/
typedef enum{
  IDLE,
  H1,
  M1,
  M2,
  S1,
  S2,
  RUN,
  PAUSE
}STATE;
/************ VARIABLES *************/
STATE state;
/************ METODOS ***************/
#define getBit(w,n) ((w&(1<<n))>>n)
#define setBit(w,n) (w|(1<<n))
#define clrBit(w,n) (w&~(1<<n))
#define writeBit(w,n,b) (b ? setBit(w,n) : clrBit(w,n))
void initProgram(){
  initBotones();
  initDisplay(2,3,4);
  state=IDLE;
  timerStop();
  setTime(0,0,0);
  triggerAmarilla(0,5,0);
  triggerRoja(0,0,0);
}
long t0,t1;
void setup(){
  initProgram();
  pinMode(13,OUTPUT);
}
void loop(){
  checkBotones();
}
