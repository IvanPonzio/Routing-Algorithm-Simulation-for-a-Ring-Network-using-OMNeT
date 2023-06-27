# Informe: Diseñando un algoritmo de enrutamiento para una red de anillo.

## Integrantes del grupo:

- **Fernando Alvarado**
- **Ivan Ponzio**
- **Armando Carral**

## Resumen
En este trabajo, se aborda el problema de congestión en una red por fallas en el enrutamiento. Contabamos con una red de anillo de 8 nodos, cada uno con una capa de aplicacion, de red y de enlace (con dos interfaces hacia nodos vecinos). Empezamos haciendo los dos casos solicitados sin aplicar ningun algoritmo, tomando distintas metricas para un posterior analisis y entendiendo la funcion de cada una de las capas en esta red. Luego, ideamos un algoritmo basado en el OSPF, pero simplificado acorde a nuestro diseño de red y lo implementamos. Realizamos los mismos casos que en la primera parte, pero esta vez con el algoritmo implementado. Analizamos los resultados obtenidos y los comparamos con los resultados de la primera parte.

## Métodos
El algoritmo implementado es una variante más simple del OSPF,ya que mientras que OSPF se basa en un enfoque de estado de enlace más avanzado y complejo, el algoritmo implementado utiliza un enfoque de vector de distancia más básico. Nuestro algoritmo, se basa en dos arrays de tamaño fijo en 8 (por la cantidad de nodos), que representan la tabla de enrutamiento de cada nodo y para contabilizar el costo de alcanzar el destino que denota el indice. Realizamos un intercambio de información entre los nodos de la red utilizando mensajes de vector de distancia (el cual creamos para enviar los arreglos, con sus getters y setters para construccion y lectura, en capa de red) entonces al igual que OSPF, el algoritmo implementado utiliza mensajes para actualizar las tablas de enrutamiento. Ambos algoritmos consideran los costos de los enlaces y los próximos saltos hacia los destinos alcanzables. Proporciona enrutamiento dinámico y eficiente, adaptándose a los cambios en la topología de la red, ya que ante la caída de un enlace o la adición de un nuevo nodo, los nodos actualizan sus tablas de enrutamiento a través del intercambio de mensajes, y seleccionando las mejores rutas en función de los costos de los enlaces.

## Resultados
Empezando con el analisis, de la parte 1 sin modificaciones, y el caso 1, donde solo los nodos 0 y 2 generan paquetes a destino, nodo 5, solamente a traves de la interfaz 0 de la capa de enlace. Logramos apreciar una congestion en los nodos 0 y 2, de nivel exponencial llegando a mas de 100 paquetes en buffer cuando terminamos de correr la simulacion. Asi mismo, vemos que de la cantidad de paquetes enviados por ambos nodos, arriban a destino la mitad (en caso del nodo 0, ya que esta mas cerca del nodo 5), y menos de la mitad en el caso del nodo 2. 

<image src="Graficos/SOLAMENTE CASO 1 PARTE 1/GraficoDeRetraso.png" alt=""> 
<image src="Graficos/SOLAMENTE CASO 1 PARTE 1/CantidadEnviadoRecibido.png" alt="">

Luego probamos el caso 2, donde todos los nodos generan paquetes hacia nodo 5, por lo que vemos que la congestion es aun mayor, llegando casi a 200 paquetes en buffer en los nodos 2 y 3, cuando termina la simulacion, y un poco menos en los otros. Se logra ver en el segundo grafico, donde se muestra que el nodo mas cercano (el 6 y 7), tiene una mayor cantidad de mensajes arribados a destino, en cambio los demas nodos, como estan mas lejos, ya que solo enviamos por la interfaz 0, tienen una menor cantidad de paquetes arribados a destino, mostrando que a mayor distancia, mayor congestion. Los mensajes se encolan en el buffer y nunca tienen la posibilidad de arribar. 

<image src="Graficos/SOLAMENTE CASO 2 PARTE 1/GraficoDeretrso.png">
<image src="Graficos/SOLAMENTE CASO 2 PARTE 1/EnviadosYrecibidos.png">

Tambien logramos buscar el equilibro subiendo el InterArrivalTime, para que no se generen tantos paquetes y asi reducir la congestion, lo logramos un poco pero sacrificando el tiempo de envio de los paquetes.

InterArrivalTime de 1 y 1.3:

<image src="Graficos/EQUILIBRIO/exp1-1,3.png" >

InterArrivalTime de 1.5 y 1.7:

<image src="Graficos/EQUILIBRIO/exp1,5-1,7.png" >

InterArrivalTime de 1.9:

<image src="Graficos/EQUILIBRIO/exp1,9.png" >

Logramos ver que poco a poco nos vamos acercando a un equilibrio, pero sacrificando el tiempo de envio de los paquetes, ya que al aumentar el InterArrivalTime, se envian menos paquetes, y por ende, se reduce la congestion.

Para el analisis de la parte 2, cuando ya aplicamos el algoritmo de enrutamiento, tambien probamos ambos casos. En el caso 1, podemos apreciar que los buffer, con la misma cantidad de paquetes generados, solamente llegan a 12, una diferencia abismal a la primera parte, y que todos los paquetes enviados, llegan a destino, ya que el algoritmo de enrutamiento, logra encontrar la ruta mas corta, y se envian por ambas interfaces. Todo eso conduce a que todos los paquetes que se encolan en los buffers, terminan llegando a destino, y asi reducir el delay.

<image src="Graficos/SOLAMENTE CASO 1 PARTE 2/GraficoDeRetrasoo.png" >
<image src="Graficos/SOLAMENTE CASO 1 PARTE 2/EnviadosYRecibidos.png" >

En el caso 2, notamos que, si bien no logramos reducir la congestion en una gran cuantia, si hacemos que lleguen una mayor cantidad de paquetes (el doble). Esto se ve reflejado, sobre todo en los nodos 4 y 3 y 2, ya que sin el algoritmo, siempre enviaban por la misma via y les quedaba muy lejos llegar al nodo 5, ahora al enviar por la otra interfaz, reducen sus saltos y tienen una mejor capacidad de llegada.

<image src="Graficos/SOLAMENTE CASO 2 PARTE 2/Grafico de Retraso.png" >
<image src="Graficos/SOLAMENTE CASO 2 PARTE 2/enviadosyrecibidos.png" >

## Conclusiones
A modo de conclusion, mostramos unos graficos comparativos, para ambos casos, en los que se muestra el contraste de aplicar o no el algoritmo de enrutamiento. 

Comparativa para el caso 1:

<image src="Graficos/COMPARATIVA CASO 1/ComparativaRecibidos.jpeg" >
<image src="Graficos/COMPARATIVA CASO 1/PaquetesRecibidosComparativa.png" >
<image src="Graficos/COMPARATIVA CASO 1/DelayPorNodo.png" >
<image src="Graficos/COMPARATIVA CASO 1/PromedioDeSaltosVERDADERO.png" >

Como se logra apreciar, para el caso 1, vemos que la cantidad de mensajes que arribaron a destino se duplican con el algoritmo, asi como tambien el delay promedio se reduce en una cantidad importante. Esto demuestra que los paquetes no estan tanto tiempo en buffer, sino que una vez que se encolan, son casi directamente enviados. Tambien en el promedio de saltos se reduce en uno, mostrando que con el algoritmo de enrutamiento, se logra encontrar la ruta mas corta, y asi reducir la cantidad de saltos que se deben dar para llegar a destino.

Comparativa para el caso 2:

<image src="Graficos/COMPARATIVA CASO 2 /ComparativaPaquetesRecibidos.png" >
<image src="Graficos/COMPARATIVA CASO 2 /ComparativaRecibidos.png" >
<image src="Graficos/COMPARATIVA CASO 2 /ComparativaDelay.png" >
<image src="Graficos/COMPARATIVA CASO 2 /PromedioDeSaltosVERdadero.png" >


En el caso 2, se sostiene la postura, ya que tambien duplica la cantidad de mensajes recibidos, con el algoritmo de enrutamiento. En el segundo grafico, como se habia mostrado antes no se lograba apreciar la mejora, pero al ver el primero, que es la suma de todos los mensajes arribados al nodo 5, logramos ver que son muchos mas que sin el algoritmo. En cuanto al delay, la principal diferencia se nota en los nodos 4 y 3, donde sin el algoritmo, llegan una cantidad infima de mensajes, en cambio con el algoritmo vemos que son muchos mas, ya que al mandar por su ruta mas corta al nodo 5, llegan, y se desencolan, mucho mas rapido, por reducir la cantidad de saltos que realizan.

En resumen, la implementación del algoritmo de enrutamiento logró mejorar significativamente el rendimiento de la red, reduciendo la congestión, mejorando el retardo y aumentando la cantidad de paquetes que llegan a su destino.

