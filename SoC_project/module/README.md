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

Además L, LC, C, RC y R representan las salidas del módulo. Debido a que el sensor BFD-1000 es un sensor digital, el proceso se realiza asignando el valor de los pines de entrada a los pines de salida, es decir un 0 o un 1 dependiendo de si está leyendo una línea negra o una línea blanca respectivamente.

![infra2](https://user-images.githubusercontent.com/92388558/153109981-1bd77b88-afe3-4cd5-864b-e43fc98c5d0c.png)

Luego se realizó la implementación por medio de Python, con la cual se interconectaron cada una de las señales de entrada del módulo infrarrojo con las señales de salida de dicho módulo por medio del uso del método de Python `__init__`. En este caso se incluyen las señales de entrada y salida, que son los argumentos de la clase `ir`, en el método `__init__` con el fin de inicializar el atributo correspondiente a cada una. Con esto entonces se especificará cada atributo de la instancia como `self.atributo`, con lo cual se muestra que la clase `ir` tiene dicho atributo y se le asigna el valor a ese atributo [[1](https://www.delftstack.com/es/howto/python/self-in-python/)]. Además, se define el tipo de registro y la cantidad de bits utilizados (todos son de 1 bit) de cada una de las salidas, siendo todos registros de tipo `Status` debido a que le brindan información al SoC (únicamente son de lectura) [[2](https://github.com/enjoy-digital/litex/blob/master/litex/soc/interconnect/csr.py)]. Por último, se crean los objetos del driver `infrarrojo`, asignándoles el valor almacenado en los atributos correspondientes. Esto se realizó tal como se muestra a continuación:

![infra3](https://user-images.githubusercontent.com/92388558/153111305-151ff5fb-90b4-490e-acdd-b6956288fe0a.png)

En este caso ........ Por último, se instanció el periférico del infrarrojo en el archivo `buildSoCproject.py`, definiendo los pines de entrada según.......

![infra4](https://user-images.githubusercontent.com/92388558/153111311-69e165d1-daaa-430b-bc7a-8440550dc648.png)


# Motores

Para el debido uso de los motores se utilizó un puente H que, según la codificación de la señal de entrada, le entrega una instrucción de 2 bits a cada motor para que el robot se movilice en una determinada dirección. En este caso el sistema de motores montado en el chasis utilizado se ilustra en la siguiente imagen:

![motores1](https://user-images.githubusercontent.com/92388558/153331110-ffb2355c-2163-4166-8bc1-d759afda07e7.JPG)

Para la construcción del módulo se tuvo en cuenta el reloj interno de la FPGA y los parámetros de entrada recibidos por el registro `movimiento`, entregando a la salida la instrucción de movimiento que recibe el puente H. Se definen entonces cuatro parámetros que indican la dirección en la que se tiene que mover el robot, los cuales son `A` (avanza), `R` (retrocede), `P` (pausa), `GD` (giro a la derecha) y `GI` (giro a la izquierda). Se debe hacer énfasis en que, para indicar las instrucciones de cada motor de manera correcta, se debe tener en cuenta que estos operan de manera simétrica (como en un espejo) ya que si, por ejemplo, se requiere que el robot avance en una línea recta, un motor debe girar en una dirección y el motor del otro lado debe girar en sentido contrario, tal como se ilustra en el siguiente ejemplo:

![Giro-de-motor](https://user-images.githubusercontent.com/82488285/153473137-6f472307-1b36-4784-ac9b-18d44414ba33.png)

En este caso se tiene que el motor de la parte superior gira en sentido horario (retrocediendo el robot, por ejemplo), mientras que el motor de la parte inferior (que corresponde al motor del otro lado del robot) debe girar en sentido antihorario para que se cumpla la condición de retroceso. Con esto entonces se puede ver en el código diseñado que para que el robot avance (opción `A`) ambos motores deben ir en la dirección `0 1` (ver tabla de la imagen /wp08-2021-2-gr-03/Imágenes/Instrucciones Motores.jpg), por lo que la instrucción que se le entregará al puente H será un `0 1` para el motor A y un `1 0` para el motor B, así mismo, para que el robot retroceda (opción `R`) ambos motores deben ir en la dirección `1 0`, por lo que la instrucción que se le entregará al puente H será un `1 0` para el motor A y un `0 1` para el motor B. En el caso de la pausa (opción `P`) ambos motores deben ir en la dirección `0 0`, pero como ninguno de ellos se mueve no hay necesidad de invertir la instrucción para el motor B. En el caso de los giros se debe tener mucho cuidado pues ambos motores deben girar en direcciones opuestas y en sentidos contrarios, es decir, para realizar un giro hacia la derecha el motor A debe recibir la instrucción `0 1` (Rotar Derecha - Avanzar según la tabla) mientras que el motor B debe recibir la instrucción `0 1` ya que corresponde al inverso de la instrucción `1 0` (Rotar Izquerda - Retroceder), por otro lado, para realizar un giro hacia la izquierda el motor A debe recibir la instrucción `1 0` (Rotar Derecha - Retroceso) mientras que el motor B debe recibir la instrucción `1 0` ya que corresponde al inverso de la instrucción `0 1` (Rotar Izquerda - Avanzar). El código construido entonces se ilustra en la siguiente imagen:

![motores2](https://user-images.githubusercontent.com/92388558/153112643-3000f310-da91-46f0-98f2-34b0ced0da81.png)

Posteriormente se realizó la implementación por medio de Python, tal como se ilustra en la siguiente imagen:

![motores3](https://user-images.githubusercontent.com/92388558/153112407-af8a2eea-5ad9-4855-8be2-a361639ce087.png)

En este caso ........ Por último, se instanció el periférico de los motores en el archivo `buildSoCproject.py`, definiendo los pines de entrada según.......

![motores4](https://user-images.githubusercontent.com/92388558/153112428-dbc1b0f7-8f90-44ea-bbe3-f09426d8a1b5.png)


# Radar

El radar está constituido por el módulo de ultrasonido HC-SRO4 y el servomotor SG90, por lo que se debió realizar dos veces el mismo proceso que con los anteriores periféricos. A continuación se muestra la descripción del código de verilog para el ultrasonido basado en el programa implementado por el grupo 11 del semestre 2021-1:

![us1](https://user-images.githubusercontent.com/92388558/153326680-40f9857f-a0aa-4109-990e-eb47225fc63d.png)

En este código se hace uso del reloj interno de la FPGA así como también se utiliza un registro de inicio (`init`) y un registro que almacena el dato de la recepción del ultrasonido enviado (`echo`), por otro lado, las salidas del módulo corresponden al disparo del ultrasonido (`trig`), la distancia que recorre dicho pulso (`distance`) y el registro de finalización (`done`). Así mismo, se utilizan tres registros para el correcto funcionamiento del proceso, los cuales son un contador que se encarga de contar el tiempo del disparo del pulso (`counter`), un registro que indica si el pulso del ultrasonido ya fue emitido (`echoStart`) y el registro que se encarga de definir el estado de la máquina de estados implementada. Además, para medir los diferentes tiempos se utilizará un reloj de periodo de 1 microsegundo, por lo que se definirá un nuevo reloj (`newCLK`) junto con el módulo del divisor de frecuencia que lo genera (`divFreq`). Posteriormente se construye una máquina de estados que en general tiene tres estados, el primero es el inicial, en el cual todos los reistros se inicializan en cero, posteriormente está el estado del pulso, en el cual se dispara la señal de ultrasonido de una duración de 11 microsegundos, controlando dicho tiempo mediante el contador, y por último está el estado del echo, el cual calcula la distancia que recorrió el pulso a partir del tiempo transcurrido desde que se dejó de emitir el pulso hasta que se recibe la señal del trigger. El código construido entonces se ilustra en las siguientes imágenes:

![us2](https://user-images.githubusercontent.com/92388558/153326692-74947cc3-cc75-4dd3-80d9-9f4730198798.png)

![us3](https://user-images.githubusercontent.com/92388558/153326702-50d04a21-04a9-4c39-b5de-a9d9b73299f4.png)

Ahora se muestra la implementación por medio de Python para el ultrasonido:

![us4](https://user-images.githubusercontent.com/92388558/153326870-c0ece5e1-cf98-47d5-b70c-0cd0e4219144.png)

Por último se muestra como se implementa el ultrasonido en el archivo `buildSoCproject.py` del proyecto:

![us5](https://user-images.githubusercontent.com/92388558/153327313-2081f45c-170f-435d-8b8d-4854e3d02919.png)


Posteriormente se desarrolló la descripción del código de Verilog para el servomotor utilizando nuevamente el reloj interno de la FPGA y un registro (`pos`) que se encarga de recibir las instrucciones de movimiento que el servomotor va a recibir por medio de la señal `servo` después de su traducción. El proceso se basa en el valor de un contador (registro `contador`) que aumenta cada ciclo positivo del reloj de la FPGA y se reinicia cuando llega a 1000000, con lo cual se determina el ángulo al cual se configurará el servomotor. Cuando la señal de la posición que es ingresada es `0 0` el servomotor se configura en un ángulo de 0 grados gracias a que el contador aumenta hasta alcanzar un valor de 50000. Si la señal ingresada es `0 1` el servomotor se configura en un ángulo de 90 grados gracias a que el contador aumenta hasta alcanzar un valor de 150000. Por último, cuando la entrada es `1 0` el servomotor se configura en un ángulo de 180 grados gracias a que el contador aumenta hasta alcanzar un valor de 200000. Con esto entonces se puede evidenciar que el contador determina la amplitud a la cual el servomotor se configura, definiendo un ángulo mayor a medida que se aumenta su valor. El código construido entonces es el ilustrado a continuación:

![sv1](https://user-images.githubusercontent.com/92388558/153329096-80c64f61-3d31-4bc3-a190-d29c9fd01bca.png)

A continuación se puede observar la implementación del servomotor por medio de Python:

![sv2](https://user-images.githubusercontent.com/92388558/153329390-5873305a-9623-4519-b9ba-b0be2d873ad0.png)

Por último se muestra como se implementa el servomotor en el archivo `buildSoCproject.py` del proyecto:



# UART
Se implementó un módulo UART para poder enviar y recibir los diferentes datos que se observarán por medio de un lector de serial Bluetooth en un teléfono celular. A continuación se muestra el código implementado en el SoC del proyecto (archivo `buildSoCproject.py`) en el cual se importa directamente de Litex un módulo de comunicación UART para su uso específico con el módulo de bluetooth.

![bt1](https://user-images.githubusercontent.com/92388558/153327906-514506eb-a5ab-4eea-8114-46509ff7beaa.png)


# I2C
Debido a que el sensor de temperatura y humedad SHT31 utiliza el protocolo de comunicación I2C, se implementará un módulo de I2C para su implementación. A continuación se muestra el código de Verilog utilizado:

![I2C](https://user-images.githubusercontent.com/92388558/153329980-8b509c84-380e-43de-bce2-c32128106c07.png)



# Cámara



# Bibliografía
[1]   Anónimo, "La palabra clave self en Python", DelftStack, 2021. Dispoible en: https://www.delftstack.com/es/howto/python/self-in-python/
[2]   A. Dennison, "csr.py", GitHub, 2021. Disponible en: https://github.com/enjoy-digital/litex/blob/master/litex/soc/interconnect/csr.py
