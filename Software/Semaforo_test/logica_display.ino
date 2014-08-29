/* Este código establece la manera de interactuar con el display. Los métodos y variables están dedicados a 
  hacer transparente el acceso a la electrónica involucrada.

*/
#define getBit(w,n) ((w&(1<<n))>>n)
#define setBit(w,n) (w|(1<<n))
#define clrBit(w,n) (w&~(1<<n))
#define writeBit(w,n,b) (b ? setBit(w,n) : clrBit(w,n))
/* todo en segundos. Tiempo de inicio, tiempo actual, tiempo de encender luz amarilla y tiempo de encender luz roja */
int t_begin,t_current,t_amarilla,t_roja;
/* para disminuir cada segundo en interrupcion */
char mitad_segundo=0;
typedef enum{
  P1_1,
  P1_8,
  P1_64,
  P1_256,
  P1_1024
}t1_PRESCALER;
t1_PRESCALER prescaler1 = P1_256;
/*********** METODOS ***********/
void initTimer1(){
	/* Desactivar interrupciones por overflow del contador*/
	TIMSK1 = clrBit(TIMSK1,TOIE1);
	/* Activar interrupciones por Compare Match A*/
	TIMSK1 = setBit(TIMSK1,OCIE1A);
	/* Limpiar registros TCCR2A y TCCR2B*/
	TCCR1A = 0;
	TCCR1B = 0;
	/* Setear modo CTC (Clear Timer on Compare match): se resetea
	automáticamente el timer al igualarse al valor OCR2A por fijar*/
	TCCR1B = setBit(TCCR1B,WGM12);
	/* Configurar el prescaler a 64*/
	switch (prescaler1){
		case P1_1:
			TCCR1B = setBit(TCCR1B,CS10);
			break;
		case P1_8:
			TCCR1B = setBit(TCCR1B,CS11);
			break;
		case P1_64:
			TCCR1B = setBit(TCCR1B,CS10);
			TCCR1B = setBit(TCCR1B,CS11);
			break;
		case P1_256:
			TCCR1B = setBit(TCCR1B,CS12);
			break;
		case P1_1024:
			TCCR1B = setBit(TCCR1B,CS12);
			TCCR1B = setBit(TCCR1B,CS10);
	}
	/* Setear el valor para el cual se generará una interrupción.
	El contador del timer 1 llega a 65535. Se quiere que genere
	una interrupción cada 500 ms. El contador aumenta con una frecuencia
	de 16000000/prescaler Hz, osea cada prescaler/16000000 segundos.
	Prescaler de 256 es compatible, pues produce conteo cada 0.000016 seg 
	y si se cuenta hasta 31250 se tiene interrupción cada 500 ms.
	*/
	OCR1A=31250;
}
void endTimer1(){
	TIMSK1 = clrBit(TIMSK1,OCIE1A);
}
ISR(TIMER1_COMPA_vect){
	/* restar 1 segundo cuando corresponda (cada 2 interrupciones) */
	if (t_current > 0){
		if (!mitad_segundo){
			t_current--;
		}
		sendNumbers();
	}
	else {
		if (mitad_segundo)
			clearDisplay();
		else
			sendZeros();
	}
	mitad_segundo = ~mitad_segundo;
	//PORTB = writeBit(PORTB, 5, mitad_segundo);
	dosPuntos(mitad_segundo? 1 : 0);
}
/* sendNumbers()
			Revisa el valor del tiempo y las luces de semáforo, y envía al display para dibujarlos.*/
void sendNumbers(){
	/* enviar tiempo */
	char h,m,s;
	h = (char)(t_current/3600);
	m = (char)((t_current - h*3600)/60);
	s = (char)(t_current - h*3600 - m*60);
	setDigit(1,mitad_segundo ? h | B10000000 : h);
	setDigit(2,m/10);
	setDigit(3,m%10);
	setDigit(4,s/10);
	setDigit(5,s%10);
	if (t_current>t_amarilla)
		setSemaforo('v');
	else if (t_current>t_roja)
		setSemaforo('a');
	else setSemaforo('r');
	/*if (t_current <= t_amarilla){
		if (t_current <= t_roja)
			setSemaforo('r');
		else
			setSemaforo('a');
	}
	else setSemaforo('v');*/
}
void sendZeros(){
	for(unsigned char i=1;i<=5;i++){
		setDigit(i,0);
	}
}
void clearDisplay(){
	for(unsigned char i=1;i<=5;i++){
		setDigit(i,255);
	}
}
void setTime(char hr, char mins, char sec){
	char h=constrain(hr,0,1), m=constrain(mins,0,59), s=constrain(sec,0,59);
	t_current = t_begin = 3600*hr + 60*mins + s;
}
/* triggerAmarilla y roja
			Establece cuándo se prenderá el semáforo en luz amarilla.
			El tiempo ingresado indica el tiempo restante que debe quedar para que se prenda esta luz. */
void triggerAmarilla(char hr, char mins, char sec){
	char h=constrain(hr,0,1), m=constrain(mins,0,59), s=constrain(sec,0,59);
	int aux = 3600*hr + 60*mins + s;
	t_amarilla = aux <= t_begin ? aux : t_begin;
}
void triggerRoja(char hr, char mins, char sec){
	char h=constrain(hr,0,1), m=constrain(mins,0,59), s=constrain(sec,0,59);
	int aux = 3600*hr + 60*mins + s;
	t_roja = aux <= t_amarilla ? aux : t_amarilla;
}
/*unsigned char getHours(){}
unsigned char getMinutes(){}
unsigned char getSeconds(){}*/

/* timer */
void timerStart(){
	initTimer1();
}
void timerStop(){
	endTimer1();
}

/* RESUMEN DE METODOS

void initTimer1()
void endTimer1()
ISR(TIMER1_COMPA_vect)
void sendNumbers()
void clearDisplay()
unsigned char getHours()
unsigned char getMinutes()
unsigned char getSeconds()

public void setTime(char hr, char mins, char sec)
public void triggerAmarilla(char hr, char mins, char sec)
public void triggerRoja(char hr, char mins, char sec)
public void timerStart()
public void timerStop()

 */
