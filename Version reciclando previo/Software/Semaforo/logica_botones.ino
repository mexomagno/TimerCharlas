/**************** VARIABLES Y CONSTANTES */
#define getBit(w,n) ((w&(1<<n))>>n)
#define setBit(w,n) (w|(1<<n))
#define clrBit(w,n) (w&~(1<<n))
#define writeBit(w,n,b) (b ? setBit(w,n) : clrBit(w,n))
/* pines botones */
char botonpin[]={5,6,7};
/* estado de los botonpin */
unsigned char pressed[]={0,0,0};
/* Tiempo de hold button */
int t_hold=2000;
/* Valor de verdad. Se configura tiempo de luz amarilla? */
char amarillaconfig=0;
/**************** METODOS */
void initBotones(){
	pinMode(botonpin[0],INPUT);
	pinMode(botonpin[1],INPUT);
	pinMode(botonpin[2],INPUT);
}
void checkBotones(){
	char which=-1;
	char action;
	//revisar si alguno ha sido clickeado
	for (char i=0;i<3;i++){
		if (action=isClicked(i)){
			which=i;
			break;
		}
	}
	//ver cual fue clickeado
	if (which!=-1){
		if ((which==0) && (action==2) && (state==IDLE)){
			//Se hizo hold en el boton MODE. Si se está en idle, hay que configurar el tiempo de la luz amarilla
			amarillaconfig=1;
			Serial.begin(9600);
			Serial.println("HOLD del boton 0!!!");
			Serial.end();
		}
		changeState(which);
		doStuff(which);
	}
}
/* isClicked(char boton)
		Revisa si el boton boton ha sido presionado.
	Valores de retorno:
			0: si no se ha hecho click en el boton
			1: si se hizo click en el boton
			2: si se hizo hold en el boton 0 por t_hold milisegundos
			*/
char isClicked(char boton){
	//leer boton
	//si boton está apretado y no lo estaba antes
	if ((getBit(PIND, botonpin[boton]))&& !(pressed[boton])){
		pressed[boton]= 1;
		/* ver si hace hold en el estado IDLE */
		if ((boton == 0) && (state == IDLE)){
			long t0=millis(),t1;
			while ((getBit(PIND, botonpin[boton])) && (pressed[boton])){
				t1=millis();
				if (t1-t0>t_hold)
					return 2;
				delay(1);
			}
		}
		return 1;
	}
	else if (!(getBit(PIND, botonpin[boton]))&& (pressed[boton]))
		pressed[boton]= 0;
	return 0;
}
char togle=0;
void changeState(char boton){
	switch(state){
		case IDLE:
			state=(boton == 0 ? H1 : (boton == 1 ? RUN : IDLE));
			break;
		case H1:
			state=(boton == 0 ? M1 : state);
			break;
		case M1:
			state=(boton == 0 ? M2 : state);
			break;
		case M2:
			state=(boton == 0 ? S1 : state);
			break;
		case S1:
			state=(boton == 0 ? S2 : state);
			break;
		case S2:
			state=(boton == 0 ? IDLE : state);
			if (state == IDLE)
				amarillaconfig=0;
			break;
		case RUN:
			state=(boton == 0 ? RUN : (boton == 1 ? PAUSE : IDLE));
			if (boton == 2) setTime(0,0,0);
			break;
		case PAUSE:
			state=(boton == 0 ? PAUSE : (boton == 1 ? RUN : IDLE));
			if (boton == 2) setTime(0,0,0);
			break;
	}
	/*Serial.begin(9600);
	Serial.print("boton: ");
	Serial.print((int)boton);
	Serial.print(", state: ");
	char buff[50];
	strcpy(buff, (state == IDLE ? "IDLE" : (state == RUN ? "RUN" : (state == H1 ? "H1" : (state == M1 ? "M1" : (state == M2 ? "M2" : (state == S1 ? "S1" : (state == S2 ? "S2" : "PAUSE"))))))));
	Serial.println(buff);
	Serial.end();*/
}
void doStuff(char boton){
	switch(state){
		case IDLE:
			//Se detuvo con stop, o se acaba de prender, o volvio de S2
			timerStop();
			break;
		case H1:
			//changeState nos dejó en este estado. Si el timer no estaba iniciado, hay que iniciarlo.
			if (!timer1Enabled())
				timerStart();
			//boton 1: subir hora. Boton 2: bajar hora.
			if (boton == 1){
				setHours((getHours()+1)%2);
			}
			if (boton == 2){
				setHours((getHours()+2-1)%2);
			}
			break;
		case M1:
			//boton 1: subir decena minuto. Boton 2: bajar decena minuto.
			if (boton == 1){
				setMinutes((getMinutes()+10)%60);
			}
			if (boton == 2){
				setMinutes((getMinutes()+60-10)%60);
			}
			break;
		case M2:
			//boton 1: subir decena minuto. Boton 2: bajar decena minuto.
			if (boton == 1){
				setMinutes(((getMinutes()+1)%10 == 0 ? (getMinutes()-9) : (getMinutes()+1))%60);
			}
			if (boton == 2){
				setMinutes(((getMinutes()+60-1)%10 == 9 ? (getMinutes()+9) : (getMinutes()+60-1))%60);
			}
			break;
		case S1:
			//boton 1: subir decena minuto. Boton 2: bajar decena minuto.
			if (boton == 1){
				setSeconds((getSeconds()+10)%60);
			}
			if (boton == 2){
				setSeconds((getSeconds()+60-10)%60);
			}
			break;
		case S2:
			//boton 1: subir decena minuto. Boton 2: bajar decena minuto.
			if (boton == 1){
				setSeconds(((getSeconds()+1)%10 == 0 ? (getSeconds()-9) : (getSeconds()+1))%60);
			}
			if (boton == 2){
				setSeconds(((getSeconds()+60-1)%10 == 9 ? (getSeconds()+9) : (getSeconds()+60-1))%60);
			}
			break;
		case RUN:
			//Se puso en RUN!!!
			if ((boton == 1)&&(!timer1Enabled()))
				timerStart();
			break;
	}
	Serial.begin(9600);
	if (!amarillaconfig){
		Serial.print("t_current: ");
		Serial.println(getTCurrent());
	}
	else {
		Serial.print("t_amarilla: ");
		Serial.println(getTAmarilla());
	}
	Serial.end();
}
/* RESUMEN DE MÉTODOS 

public	initBotones();
public	checkBotones();
		isClicked(char boton);
		changeState(char boton);

*/