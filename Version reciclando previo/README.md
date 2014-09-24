Automatizaci�n Sem�foro Existente
===============================
Este proyecto toma el sem�foro b�sico existente y lo automatiza completamente. El sem�foro anterior constaba sencillamente de un sem�foro controlado con <strong>switches manuales</strong> que permit�an encender cualquier luz.

La nueva versi�n permite programar el dispositivo, mediante interacci�n por botones y un display led.

----------------------
###**Componentes**
El control consta de tres botones:
>- SET
>- UP / START
>- DOWN / STOP

Adem�s, posee un display num�rico de 5 d�gitos, que permite programar tiempos de hasta 2 horas.

Se cuenta con tres leds que simbolizan las luces del sem�foro real.

Posee un conector de alimentaci�n, y un conector al sem�foro.

###**Instrucciones de uso**
Una vez encendido el dispositivo, debe esperar unos 5 segundos aproximadamente para que inicie (bootloader :( ).
####Configurar tiempo de cuenta regresiva
>Presione el bot�n **SET** para entrar al modo configuraci�n. Los puntos comenzar�n a parpadear. Esto indica que est� seteando el primer d�gito. Presione **UP/START** o **DOWN/STOP** para aumentar o disminuir este valor.

>Para configurar el siguiente d�gito, presione nuevamente **SET**. El d�gito seleccionado parpadear�. 

>Repita lo anterior para configurar todos los d�gitos.

>Cuando llegue al �ltimo d�gito, presionar **SET** termina la configuraci�n.
####Iniciar, pausar y detener cuenta regresiva
>*Iniciar*
>Una vez configurado el tiempo de cuenta regresiva, presione **UP/START**. Durante la cuenta, el sem�foro se encender� de los siguientes colores:

> - Verde: Si quedan m�s de 5 minutos de cuenta.
> - Amarillo: Si quedan menos de 5 minutos de cuenta.
> - Rojo: Si el tiempo se ha terminado.

>*Pausar*
>Presione **UP/START** para pausar la cuenta regresiva. Los n�meros se encender�n de manera intermitente. 

>*Detener*
>Presione **DOWN/STOP** para detener la cuenta regresiva. El contador retornar� a cero y volver� a un estado equivalente a encender el dispositivo.

####Cambiar encendido de luz amarilla
>Por defecto, la luz amarilla se enciende cuando quedan 5 minutos de cuenta. Si usted desea cambiar este valor, debe seguir los siguientes pasos:

>En el estado de inicio del dispositivo, mantenga presionado **SET** por 2 segundos. Los puntos parpadear�n, *junto con la luz amarilla del sem�foro*. 
>El display mostrar� el tiempo pre-configurado en el cual el sem�foro se enciende de color amarillo. Cambie este valor a su voluntad.

>Finalice la configuraci�n presionando **SET** hasta haber configurado cada d�gito.

>La pr�xima vez que inicie la cuenta regresiva, el sem�foro se encender� de color amarillo cuando el tiempo restante sea el que usted ha configurado.

>*<strong>Importante</strong>: este valor <strong>no</strong> es almacenado al apagarse el dispositivo. Cuando lo apague y encienda nuevamente, volver� a estar configurado para encender la luz amarilla cuando queden 5 minutos restantes.*

>Si al mantener presionado **SET** el parpadeo de la luz amarilla del sem�foro no sucede, no presion� el bot�n el tiempo suficiente y estar� configurando el tiempo de cuenta regresiva. En este caso, para salir,  presione **SET** hasta haber recorrido todos los d�gitos (5 veces).

---------
###Agradecimientos
NADIE.