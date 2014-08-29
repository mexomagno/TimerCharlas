/* Setup:
        -pin 2 a serial in del shift register
        -pin 3 al shift clock del " "
        -pin 4 al latch clock (store) del " "
        
   La manera de controlar el display es la siguiente:
   Se debe enviar N_BITS bits a los shift registers. A continuación
   no se tomará en cuenta el orden de envío de los bits al shift register
   sino que el orden en el que quedarán una vez enviados.
   
   Los primeros 7 bits indican qué segmento del digito seleccionado se prenderá.
   Ojo, es selección inversa: HIGH desselecciona y LOW selecciona.
   los siguientes 10 bits establecen qué dígito está seleccionado. Claramente
   sólo uno de estos bits puede estar en HIGH.
   Los bits a enviar se guardan en un arreglo de chars bits[N_BITS]*/
/* 
  Los caracteres numéricos son los dígitos del 2 al 8
  Para escribir en ellos, basta asignar los chars que se quiere escribir.
  Caracter 0: DVD, VCD, MP3, play, pausa, PBC, segmento del cd
  Caracter 1: segmentos del cd
  Caracter 9: deshabilitado por ahora
  
  Es de interes por ahora los numéricos y el CD.
  
  Los chars de los caracteres numéricos almacenan el valor del caracter que 
  representan, por ejemplo, si el display muestra un 0, su char es '0'.
  Los chars de los otros elementos son especiales: sus bits internos representan
  cada uno de los segmentos del "digito". Por ejemplo, si quiero prender el 
  último segmento del "digito" 0, su char es algo como B01000000. getBit de este
  char en la posición 6 retorna 1, y en cualquier otra posición retorna 0.
  
*/
/****************** VARIABLES Y CONSTANTES GLOBALES *******************/
#define N_BITS 16
#define N_CHARS 9
/* manejo de bits */
#define getBit(w,n) ((w&(1<<n))>>n)
#define setBit(w,n) (w|(1<<n))
#define clrBit(w,n) (w&~(1<<n))
#define writeBit(w,n,b) (b ? setBit(w,n) : clrBit(w,n))
//pines
unsigned char serialpin, shiftpin, latchpin;
//LISTA DE CARACTERES A MOSTRAR EN EL DISPLAY
char chars[N_CHARS];
//caracter actual. Va del 0 a N_CHARS-1. Representa cada uno de los caracteres seleccionables en el display. Numericos + especiales.
unsigned char currentchar;
//leds dibujados del disco
unsigned char discleds;
//Digitos seleccionables. Va del 0 al 9
unsigned char currentdigit=0;
//BITS PARA ESCRIBIR EN LOS SHIFT REGISTERS
unsigned char bits[N_BITS];
//activa - desactiva el modo marquesina para los digitos numéricos
char marquee_enable;


/******************* MÉTODOS ********************/
void selectNextDigit(){
  //deshabilita digito actual
  bits[currentdigit+7]=LOW;
  currentdigit=(currentdigit+1)%10;
  //habilita siguiente digito
  bits[currentdigit+7]=HIGH;
}
void selectDigit(unsigned char digit){
  //deshabilita digito actual
  bits[currentdigit+7]=LOW;
  //habilita digito pedido
  char indice;
  if (digit<2)
    indice=digit;
  else if ((digit==2)||(digit==3))
    indice=digit+1;
  else if (digit==4)
    indice=2;
  else indice=digit;
  currentdigit=indice;
  bits[indice+7]=HIGH;
}
/* storeDigit(d,t) escribe digito d de tipo t. Tipo es: 'N'=numerico, 'E'=display especial (como caracter 0, 1 y 9) */
void storeDigit(char digito, char tipo){
  //arreglo de valores a escribir en el dígito seleccionado
  unsigned char segments[7];
  if (tipo=='N'){
    //es de tipo numérico. Leer qué número representa el char digito.
    switch (digito){
      case '0':
        segments[0]=HIGH;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=HIGH;
        segments[4]=HIGH;
        segments[5]=HIGH;
        segments[6]=LOW;
        break;
      case '1':
        segments[0]=LOW;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=LOW;
        segments[4]=LOW;
        segments[5]=LOW;
        segments[6]=LOW;
        break;
      case '2':
        segments[0]=HIGH;
        segments[1]=HIGH;
        segments[2]=LOW;
        segments[3]=HIGH;
        segments[4]=HIGH;
        segments[5]=LOW;
        segments[6]=HIGH;
        break;
      case '3':
        segments[0]=HIGH;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=HIGH;
        segments[4]=LOW;
        segments[5]=LOW;
        segments[6]=HIGH;
        break;
      case '4':
        segments[0]=LOW;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=LOW;
        segments[4]=LOW;
        segments[5]=HIGH;
        segments[6]=HIGH;
        break;
      case '5':
        segments[0]=HIGH;
        segments[1]=LOW;
        segments[2]=HIGH;
        segments[3]=HIGH;
        segments[4]=LOW;
        segments[5]=HIGH;
        segments[6]=HIGH;
        break;
      case '6':
        segments[0]=HIGH;
        segments[1]=LOW;
        segments[2]=HIGH;
        segments[3]=HIGH;
        segments[4]=HIGH;
        segments[5]=HIGH;
        segments[6]=HIGH;
        break;
      case '7':
        segments[0]=HIGH;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=LOW;
        segments[4]=LOW;
        segments[5]=LOW;
        segments[6]=LOW;
        break;
      case '8':
        segments[0]=HIGH;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=HIGH;
        segments[4]=HIGH;
        segments[5]=HIGH;
        segments[6]=HIGH;
        break;
      case '9':
        segments[0]=HIGH;
        segments[1]=HIGH;
        segments[2]=HIGH;
        segments[3]=HIGH;
        segments[4]=LOW;
        segments[5]=HIGH;
        segments[6]=HIGH;
        break;
      case 'X':
        segments[0]=LOW;
        segments[1]=LOW;
        segments[2]=LOW;
        segments[3]=LOW;
        segments[4]=LOW;
        segments[5]=LOW;
        segments[6]=LOW;
        break;
      default:
        segments[0]=LOW;
        segments[1]=LOW;
        segments[2]=LOW;
        segments[3]=LOW;
        segments[4]=LOW;
        segments[5]=LOW;
        segments[6]=HIGH;
        break;
    }
  }
  else if (tipo == 'E'){
    //tipo especial. Leer bits del char y mapearlo a segmentos.
    for (int i=0;i<7;i++){
      segments[i]=getBit(digito,i);
    }
  }
  //escribir digito en arreglo de bits
  for (int i=0;i<7;i++){
    if ((tipo=='N')&&(i==7)) break;
    bits[i]=segments[i]==HIGH ? LOW : HIGH;
  }
}
void show(){
  //enviar datos guardados en bits[] hacia el display, usando los shift registers.
  for (int i=N_BITS-1;i>=0;i--){
    digitalWrite(serialpin,bits[i]);
    clock(shiftpin);
  }
  clock(latchpin);
}
void clock(unsigned int pin){
  //genera un ciclo de reloj en el pin pedido
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
}

//Metodo alto nivel para escribir. 
//printDigit(a,b,t) escribe el caracter b en el display, en el digito a, de tipo t. (numerico o especial).
void printDigit(int digitn, char digitval, char tipo){
  selectDigit(digitn);
  storeDigit(digitval, tipo);
  show();
}
/* writeChar(d,c) Escribe caracter c en digito d. OJO: digitos van del 0 al 6, del menos
significativo al más significativo (derecha a izquierda) */
void writeChar(int d, char c){
  chars[6-d+2]=c;
}
void writeWord(char *w){
  int digitindex=0;
  for (int i=strlen(w)-1;i>=0;i--){
    writeChar(digitindex++, w[i]);
  }
  for (int i=digitindex;i<7;i++){
    writeChar(i,'X');
  }
}
void writeWord(long w){
  char buff[8];
  String(w).toCharArray(buff,8);
  writeWord(buff);
}
void marquee(char *w,char on_off){
  marquee_enable=on_off ? true : false;
}
void writeDisc(char w){
  /* w es un byte de la forma ABCDEFGH. Cada letra representa
  un pedazo del cd, de la forma: 
    B  C
   A    D
   H    E
    G  F
    
    BCFGHDE
  */
  //escribir segmento A. Está en el caracter especial 0 del display.
  /* chars[0] contiene al primer caracter, que interesa pues tiene el 
  primer segmento del cd ubicado en el ultimo bit del char */
  chars[0]=writeBit(chars[0],6,getBit(w,0));
  /* Escribir segmentos restantes. Están en el caracter especial 1*/
  chars[1]=writeBit(chars[1],0,getBit(w,1));
  chars[1]=writeBit(chars[1],1,getBit(w,2));
  chars[1]=writeBit(chars[1],5,getBit(w,3));
  chars[1]=writeBit(chars[1],6,getBit(w,4));
  chars[1]=writeBit(chars[1],3,getBit(w,5));
  chars[1]=writeBit(chars[1],2,getBit(w,6));
  chars[1]=writeBit(chars[1],4,getBit(w,7));
  /*Serial.begin(9600);
  printChar(chars[0]);
  printChar(chars[1]);
  Serial.println();
  Serial.end();*/
  discleds=w;
}

void rotateDisc(char d){
  //d=0: CLOCK (forward). d=1: CCLOCK (reverse)
  char aux= d==0 ? writeBit((discleds<<1),0,getBit(discleds,7)) : writeBit((discleds >> 1),7,getBit(discleds,0));
  //char aux= d=='F' ? discleds<<1 : discleds>>1;
  writeDisc(aux);
}
void printChar(char w){
  for (int i=7;i>=0;i--){
    Serial.print(getBit(w,i));
  }
  Serial.println();
}
void initTimer2(){
  cli();
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
  /* Configurar el prescaler a 64*/
  TCCR2B |= (1<<CS22);
  /* Setear el valor para el cual se generará una interrupción */
  OCR2A=255;
  sei();
}
ISR(TIMER2_COMPA_vect){
  //rotar cada caracter a mostrar
  currentchar=(currentchar+1)%N_CHARS;
  char tipo = (currentchar>=2) && (currentchar<=8) ? 'N' : 'E';
  printDigit(currentchar,chars[currentchar],tipo);
}

void initChars(){
  for (int i=0;i<N_CHARS;i++){
    if ((i<=1) || (i==9))
      chars[i]=B00000000;
    else
      chars[i]='X';
  }
  currentchar=0;
}

void initDisplay(unsigned char serial, unsigned char shift, unsigned char latch){
  serialpin=serial;
  shiftpin=shift;
  latchpin=latch;
  pinMode(serialpin,OUTPUT);
  pinMode(shiftpin,OUTPUT);
  pinMode(latchpin,OUTPUT);
  //setear digitos en apagado.
  for (int i=0;i<7;i++){
    bits[i]=HIGH;
  }
  //desseleccionar todos los dígitos
  for (int i=7;i<N_BITS;i++){
    bits[i]=LOW;
  }
  discleds=0;
  marquee_enable=false;
  //escribir en display
  show();
  initChars();
  initTimer2();
}

/* 
  Para controlar la pantalla, debe preferirse el método 
  -writeChar(d,c) para escribir el caracter c en el digito numerico d (derecha a izquierda)
  -writeWord(w) para escribir la palabra w en los digitos numericos
  -writeDisc(w) para dibujar en el disco
  -rotateDisc(d) para girar el disco en direccion d
  
*/
