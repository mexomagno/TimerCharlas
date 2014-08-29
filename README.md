Timer para Charlas
============

<h3>Display de cuenta regresiva para charlas y presentaciones.</h3>
<p>Al momento de dar una charla, exponer en un seminario, hablar ante audiencias con tiempo limitado, llevar la cuenta del tiempo restante es esencial. Cuando el escenario es grande o es necesario moverse, es cómodo poder enterarse de cuánto tiempo queda a través de un estímulo visual.</p>
<p>El timer de charlas pretende ser un apoyo para las personas que tengan esta necesidad particular, y que no consideren suficientes las aplicaciones de tablets destinadas a este mismo fin, donde el tamaño del display depende del tamaño del tablet, además de ser poco discreto poner un tablet en el suelo.</p>
Este es un proyecto personal que no busca ser nada grande. Se dio por dos cosas:
<ol>
<li>Mi madre quería reemplazar un sistema de semáforo que tenían que era totalmente manual y poco práctico.</li>
<li>Es un desafío personal e involucra usar los distintos conocimientos ociosos que he ido adquiriendo sobre electrónica en general.</li>
</ol>
El concepto básico es (con el gentil auspicio de <i>ms-paint</i>) como sigue:
<img src="http://www.dcc.uchile.cl/~acastro/random/prototipos.png" alt="imagen_prototipo.png">
<h4><i>Hardware</i></h4>
<p>Es un artefacto de aproximadamente 40x10 cm<sup>2</sup> de superficie y el grosor que tenga que tener, dependiendo de la circuitería.</p>
El diseño de las partes se encuentra documentado, así como los posibles brainstorms que les dan nacimiento.
<h4><i>Software</i></h4>
<p>El display es totalmente controlado por un Arduino Pro Mini. El manejo de los segmentos y del programa en sí del Timer de Charlas está implementado mediante el uso de interrupciones de dos de los tres timers internos del Atmega328 (el timer1 y el timer2), para así obtener toda la precisión que el cristal de cuarzo de 16 [MHz] puede ofrecer. </p>
<p>Si a alguien le es útil esto, por favor úselo.</p>


<h4><strong>Prestaciones implementadas</strong></h4>
<ul>
<li>Cuenta regresiva de tiempo de alta precisión, con tiempo máximo de 1:59:59 (un tiempo humanamente aceptable para una charla).</li>
<li>Luces tipo semáforo indicadoras rápidas del estado del tiempo restante.</li>
<li>Activación de luz amarilla y roja del semáforo ajustables.</li>
<li>Dígitos de 7 segmentos, 5 leds rojos de 5mm por cada segmento; correcto manejo de corrientes admitidas por cada dispositivo.</li>
<li>Transparencia en el código del programa principal, mediante la implementación de métodos de alto nivel que ocultan funcionalidades de bajo nivel.</li>
</ul>
<h4><strong>Posibles adiciones</strong></h4>
<ul>
<li>Interfaz por control remoto IR.</li>
<li>Permitir uso de batería.</li>
<li>Permitir el ajuste del brillo de los leds.</li>
</ul>
