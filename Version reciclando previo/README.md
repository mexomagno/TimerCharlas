Automatización Semáforo Existente
===============================
Este proyecto toma el semáforo básico existente y lo automatiza completamente. El semáforo anterior constaba sencillamente de un semáforo controlado con <strong>switches manuales</strong> que permitían encender cualquier luz.

La nueva versión permite programar el dispositivo, mediante interacción por botones y un display led.

----------------------
###**Componentes**
El control consta de tres botones:
>- SET
>- UP / START
>- DOWN / STOP

Además, posee un display numérico de 5 dígitos, que permite programar tiempos de hasta 2 horas.

Se cuenta con tres leds que simbolizan las luces del semáforo real.

Posee un conector de alimentación, y un conector al semáforo.

###**Instrucciones de uso**
Una vez encendido el dispositivo, debe esperar unos 5 segundos aproximadamente para que inicie (bootloader :( ).
####Configurar tiempo de cuenta regresiva
>Presione el botón **SET** para entrar al modo configuración. Los puntos comenzarán a parpadear. Esto indica que está seteando el primer dígito. Presione **UP/START** o **DOWN/STOP** para aumentar o disminuir este valor.

>Para configurar el siguiente dígito, presione nuevamente **SET**. El dígito seleccionado parpadeará. 

>Repita lo anterior para configurar todos los dígitos.

>Cuando llegue al último dígito, presionar **SET** termina la configuración.
####Iniciar, pausar y detener cuenta regresiva
>*Iniciar*
>Una vez configurado el tiempo de cuenta regresiva, presione **UP/START**. Durante la cuenta, el semáforo se encenderá de los siguientes colores:

> - Verde: Si quedan más de 5 minutos de cuenta.
> - Amarillo: Si quedan menos de 5 minutos de cuenta.
> - Rojo: Si el tiempo se ha terminado.

>*Pausar*
>Presione **UP/START** para pausar la cuenta regresiva. Los números se encenderán de manera intermitente. 

>*Detener*
>Presione **DOWN/STOP** para detener la cuenta regresiva. El contador retornará a cero y volverá a un estado equivalente a encender el dispositivo.

####Cambiar encendido de luz amarilla
>Por defecto, la luz amarilla se enciende cuando quedan 5 minutos de cuenta. Si usted desea cambiar este valor, debe seguir los siguientes pasos:

>En el estado de inicio del dispositivo, mantenga presionado **SET** por 2 segundos. Los puntos parpadearán, *junto con la luz amarilla del semáforo*. 
>El display mostrará el tiempo pre-configurado en el cual el semáforo se enciende de color amarillo. Cambie este valor a su voluntad.

>Finalice la configuración presionando **SET** hasta haber configurado cada dígito.

>La próxima vez que inicie la cuenta regresiva, el semáforo se encenderá de color amarillo cuando el tiempo restante sea el que usted ha configurado.

>*<strong>Importante</strong>: este valor <strong>no</strong> es almacenado al apagarse el dispositivo. Cuando lo apague y encienda nuevamente, volverá a estar configurado para encender la luz amarilla cuando queden 5 minutos restantes.*

>Si al mantener presionado **SET** el parpadeo de la luz amarilla del semáforo no sucede, no presionó el botón el tiempo suficiente y estará configurando el tiempo de cuenta regresiva. En este caso, para salir,  presione **SET** hasta haber recorrido todos los dígitos (5 veces).

---------
###Agradecimientos
NADIE.