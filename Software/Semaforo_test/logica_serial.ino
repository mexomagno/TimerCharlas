/*  Este código está dedicado a formar una interfaz entre la capa de lógica del display, y los puertos arduino.
  No se debiera tener que acceder a los puertos del arduino directamente en ningún otro archivo aparte de este.

*/
/************ CONSTANTES Y MACROS ************/
#define N_BITS 16
#define N_DIGITOS 5
#define getBit(w,n) ((w&(1<<n))>>n)
#define setBit(w,n) (w|(1<<n))
#define clrBit(w,n) (w&~(1<<n))
#define writeBit(w,n,b) (b ? setBit(w,n) : clrBit(w,n))
unsigned char serialpin, shiftpin, latchpin;
typedef enum{
  P2_1,
  P2_8,
  P2_32,
  P2_64
}t2_PRESCALER;
t2_PRESCALER prescaler2 = P2_64;
/************ VARIABLES *************/
/* bits a enviar a shift registers */
unsigned char bits[N_BITS];
/* Digitos presentes en el display */
unsigned char digitos[N_DIGITOS];
/* Dígito actual */
unsigned char current_digit;
/* Luz del semáforo */
char semaforo;
/************ METODOS ***************/
/* initTimer2(const prescaler, unsigned char OCR2A) 
      Inicializa el timer2. Setea prescaler, y cada cuántos clicks generar una interrupción.
      Prescaler puede ser:  
                            P_1,  P_8,  P_32,  P_64
      OCR2A puede ser: nros del 0 al 255
*/
void initTimer2(unsigned char ocr2a){
  cli();
  /* Desactivar interrupciones por overflow del contador*/
  TIMSK2  = clrBit(TIMSK2,TOIE2);
  /* Activar interrupciones por Compare Match A*/
  TIMSK2 = setBit(TIMSK2,OCIE2A);
  /* Limpiar registros TCCR2A y TCCR2B*/
  TCCR2A = TCCR2B = 0;
  /* Setear modo CTC (Clear Timer on Compare match): se resetea
  automáticamente el timer al igualarse al valor OCR2A por fijar*/
  TCCR2A = setBit(TCCR2A,WGM21);
  /* Configurar el prescaler*/
  switch (prescaler2){
    case P2_1:
      TCCR2B = setBit(TCCR2B,CS20);
      break;
    case P2_8:
      TCCR2B = setBit(TCCR2B,CS21);
      break;
    case P2_32:
      TCCR2B = setBit(TCCR2B,CS20);
      TCCR2B = setBit(TCCR2B,CS21);
      break;
    case P2_64:
      TCCR2B = setBit(TCCR2B,CS22);
      break;
    default:
      TCCR2B = setBit(TCCR2B,CS22);
  }
  /* Setear el valor para el cual se generará una interrupción */
  OCR2A=ocr2a;
  sei();
}
/* Handler timer interrupts */
ISR(TIMER2_COMPA_vect){
  refreshBits();
  sendBits();
}
/* refreshBits()
      Revisa los valores actuales y setea los bits correspondientes para mostrarlos en el display.
      Debe ser llamado por el interrupt del timer.
    Estructura de los bits:
        0-6    : segmentos a prender (A B C D E F G)
        7-11   : digito a seleccionar (D1 D2 D3 D4 D5)
        12-14  : luz de semáforo (verde, amarillo, rojo)
      
      */
void refreshBits(){
  /* revisar selector de dígito */
  bits[6+current_digit]=0;
  current_digit=((current_digit+1)%N_DIGITOS+1);
  bits[6+current_digit]=1;
  /* revisar dígitos */
  unsigned char new_digito;
  switch (digitos[current_digit-1]){
    case 255: //para aclarar pantalla
      new_digito=0;
      break;
    case 0:
      new_digito=(current_digit == 1) ? 0 : B11111100;
      break;
    case 1:
      new_digito=B01100000;
      break;
    case 2:
      new_digito=B11011010;
      break;
    case 3:
      new_digito=B11110010;
      break;
    case 4:
      new_digito=B01100110;
      break;
    case 5:
      new_digito=B10110110;
      break;
    case 6:
      new_digito=B10111110;
      break;
    case 7:
      new_digito=B11100000;
      break;
    case 8:
      new_digito=B11111110;
      break;
    case 9:
      new_digito=B11110110;
      break;
    case B10000000:
      new_digito=B10000000;
      break;
    case B10000001:
      new_digito=B11100000;
      break;
  }
  for (char i=0; i<7; i++){
    bits[i]=(getBit(new_digito,(7-i))) ? 0 : 1;
  }
  /* revisar semáforo */
  bits[12]= ((semaforo == '1')||(semaforo == 'v')) ? 1 : 0;
  bits[13]= ((semaforo == '1')||(semaforo == 'a')) ? 1 : 0;
  bits[14]= ((semaforo == '1')||(semaforo == 'r')) ? 1 : 0;
}
/* sendBits()
      Envía bits almacenados a los shift registers */
void sendBits(){
  for (int i=N_BITS-1;i>=0;i--){
    PORTD=writeBit(PORTD,serialpin,bits[i]);
    clock(shiftpin);
  }
  clock(latchpin);
}
void clock(unsigned int pin){
  //genera un ciclo de reloj en el pin pedido
  PORTD = writeBit(PORTD,pin,0);
  delayMicroseconds(1);
  PORTD = writeBit(PORTD,pin,1);
  delayMicroseconds(1);
}

/* INICIALIZADORES */
void initDisplay(unsigned char serial, unsigned char shift, unsigned char latch){
  serialpin=serial;
  shiftpin=shift;
  latchpin=latch;
  pinMode(serialpin,OUTPUT);
  pinMode(shiftpin,OUTPUT);
  pinMode(latchpin,OUTPUT);
  /* inicializar bits */
  for (char i=0;i<N_BITS;i++)
    bits[i]=0;
  /* dar valor inicial a los dígitos */
  for (char i=0; i<N_DIGITOS;i++){
    digitos[i]=0;
  }
  current_digit=1;
  semaforo = 'v';
  initTimer2(255);
}

/* EDICION DE DATOS */
/* setDigit(char digito, char valor)
      Setea a un dígito su valor.
         digito puede ser de 1 a N_DIGITOS
         valor puede ser de 0 a 9.
      Si se ingresa algo inválido, sencillamente no se hace nada.
      IMPORTANTE: NO tiene efecto inmediato en el display. El timer es el encargado de actualizarlo.*/
void setDigit(unsigned char digito, unsigned char valor){
  if ((digito <= N_DIGITOS)&&(digito >= 1))
    if (((valor <= 9)&&(valor >= 0))||(valor == 255))
      digitos[digito-1]=valor;
}
void setSemaforo(char color){
  semaforo=color;
}
void dosPuntos(char valor){
  digitos[0]=writeBit(digitos[0],7,valor);
}
/*char getSemaforo(){
  return semaforo;
}*/
/* RESUMEN DE METODOS

  void initTimer2(PRESCALER prescaler, unsigned char OCR2A)
  void ISR(TIMER2_COMPA_vect)
  void refreshBits()
  void sendBits()
  void clock(unsigned int pin)

  public void initDisplay(unsigned char serial, unsigned char shift, unsigned char latch)
  public void setDigit(char digito, char valor)
  public void setSemaforo(char color)
  public void dosPuntos(char valor)

*/
