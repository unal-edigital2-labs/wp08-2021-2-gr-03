1. [ Infrarrojo. ](#infrarrojo)
2. [ Motores. ](#motores)
3. [ Radar. ](#radar)
4. [ UART. ](#uart)
5. [ I2C. ](#I2C)
6. [ Camara. ](#camara)

# Infrarrojo

Para implementar el sensor de infrarrojo se utilizó el sensor de seguimiento de linea BFD-1000 que cuenta con 5 sensores infrarrojos, tal como se muestra en la siguiente imagen:

![infra1](https://user-images.githubusercontent.com/92388558/153109712-89b3b481-5225-40f7-828e-785313257cef.png)

A continuación se muestra la descripción del código realizado en verilog, donde iL, iLC, iC, iRC e iR representan las señales de entrada del sensor:

- iL  - Sensor izquierda más lejano al centro.
- iLC - Sensor izquierda más cercano al centro.
- iC  - Sensor central.
- iRC - Sensor derecho más cercano al centro.
- iR  - Sensor derecho más lejano al centro.

Además L, LC, C, RC y R representan las salidas del módulo. Debido a que el sensor BFD-1000 es un sensor digital, el proceso se realiza asignando el valor de los pines de entrada a los pines de salida, es decir un 0 o un 1 dependiendo de lo que el sensor este leyendo (línea negra o línea blanca).

![infra2](https://user-images.githubusercontent.com/92388558/153109981-1bd77b88-afe3-4cd5-864b-e43fc98c5d0c.png)

Luego se realizó la implementación por medio de python, tal como se muestra a continuación:

![infra3](https://user-images.githubusercontent.com/92388558/153111305-151ff5fb-90b4-490e-acdd-b6956288fe0a.png)

En este caso ........ Por último, se instanció el periférico del infrarrojo en el archivo `main.c`, definiendo los pines de entrada según.......

![infra4](https://user-images.githubusercontent.com/92388558/153111311-69e165d1-daaa-430b-bc7a-8440550dc648.png)


# Motores

Para el debido uso de los motores se utilizó un puente H que, según la codificación de la señal de entrada, le entrega una instrucción de 2 bits a cada motor para que el robot se movilice en una determinada dirección. En este caso el sistema de motores montado en el chasis utilizado se ilustra en la siguiente imagen:

![motores1](https://user-images.githubusercontent.com/92388558/153331110-ffb2355c-2163-4166-8bc1-d759afda07e7.JPG)

Para la construcción del módulo se tuvo en cuenta el reloj interno de la FPGA y los parámetros de entrada recibidos por el registro `movimiento`, entregando a la salida la instrucción de movimiento que recibe el puente H. Se definen entonces cuatro parámetros que indican la dirección en la que se tiene que mover el robot, los cuales son `A` (avanza), `R` (retrocede), `P` (pausa), `GD` (giro a la derecha) y `GI` (giro a la izquierda). Se debe hacer énfasis en que, para indicar las instrucciones de cada motor de manera correcta, se debe tener en cuenta que estos operan de manera simétrica (como en un espejo) ya que si, por ejemplo, se requiere que el robot avance en una línea recta, un motor debe girar en una dirección y el motor del otro lado debe girar en sentido contrario, tal como se ilustra en el siguiente ejemplo:

![Giro-de-motor](https://user-images.githubusercontent.com/82488285/153473137-6f472307-1b36-4784-ac9b-18d44414ba33.png)

En este caso se tiene que el motor de la parte superior gira en sentido horario (retrocediendo el robot, por ejemplo), mientras que el motor de la parte inferior (que corresponde al motor del otro lado del robot) debe girar en sentido antihorario para que se cumpla la condición de retroceso. Con esto entonces se puede ver en el código diseñado que para que el robot avance (opción `A`) ambos motores deben ir en la dirección `0 1` (ver tabla de la Figura [Figura 9](README.md/https://user-images.githubusercontent.com/92388558/152260815-00da78f2-fa9d-409e-9b7c-f7f58820eceb.png))

![motores2](https://user-images.githubusercontent.com/92388558/153112643-3000f310-da91-46f0-98f2-34b0ced0da81.png)

![motores3](https://user-images.githubusercontent.com/92388558/153112407-af8a2eea-5ad9-4855-8be2-a361639ce087.png)

![motores4](https://user-images.githubusercontent.com/92388558/153112428-dbc1b0f7-8f90-44ea-bbe3-f09426d8a1b5.png)


# Radar
El radar consta de el modulo de ultrasonido HC-SRO4 y el servomotor sg90. A continuacion se muestra la descripcion del codigo de verilog para el ultrasonido basado el el programa implementado por el grupo 11 del semestre 2021-1.

![us1](https://user-images.githubusercontent.com/92388558/153326680-40f9857f-a0aa-4109-990e-eb47225fc63d.png)

![us2](https://user-images.githubusercontent.com/92388558/153326692-74947cc3-cc75-4dd3-80d9-9f4730198798.png)

![us3](https://user-images.githubusercontent.com/92388558/153326702-50d04a21-04a9-4c39-b5de-a9d9b73299f4.png)


Ahora se muestra la implementacion epor medio de python para el ultrasonido:

![us4](https://user-images.githubusercontent.com/92388558/153326870-c0ece5e1-cf98-47d5-b70c-0cd0e4219144.png)

Por ultimo se muestra como se implementa el ultrasonido en el Soc del proyecto

![us5](https://user-images.githubusercontent.com/92388558/153327313-2081f45c-170f-435d-8b8d-4854e3d02919.png)



Ahora se muestra la descripcion del codigo de verilog para el servomotor

![sv1](https://user-images.githubusercontent.com/92388558/153329096-80c64f61-3d31-4bc3-a190-d29c9fd01bca.png)

A continuacion se puede observar la implementacion del servo por medio de python:


![sv2](https://user-images.githubusercontent.com/92388558/153329390-5873305a-9623-4519-b9ba-b0be2d873ad0.png)



# UART
Se implemento un modulo UART para poder leer los datos RX,TX del bluethoot y poderlos observar por medio de un telefono celular a continuacion se muestra el codigo implementado en el SOC del proyecto en el cual se importa directamente de litex un modulo de uart el cual se usa para el modulo de bluethoot.

![bt1](https://user-images.githubusercontent.com/92388558/153327906-514506eb-a5ab-4eea-8114-46509ff7beaa.png)


# I2C
El modulo de I2C se usa para la implementacion del sensor de temperatura y humedad, a continuacion se muestra el codigo de verilog para el modulo I2C

![I2C](https://user-images.githubusercontent.com/92388558/153329980-8b509c84-380e-43de-bce2-c32128106c07.png)



# Camara
