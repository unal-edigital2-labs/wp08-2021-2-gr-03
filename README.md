# Robot Cartógrafo - Entrega Final

- Daniela Valentina Amaya Vargas
- Julian Andrés Castro Pardo
- Julián Andrés Silva Cuadros


Un robot cartógrafo es un robot que construye y guarda mapas, dependiendo de los datos recogidos por los diferentes sensores que lo conforman. En este caso, se programará este robot cartografo para que pueda recorrer un laberinto mientras va tomando fotos por medio de una cámara, analizando los diferentes colores y figuras que se identifiquen. A continuacion se muestra la lista de los principales componentes requeridos para la elaboracion de este robot:


## Materiales
- Cámara OV7970.
- Tarjeta Arduino.
- Tarjeta de desarrollo FPGA, Nexys4DDR.
- Sensor de ultrasonido HC-SR04.
- Sensor de temperatura y humedad SHT31.
- Sensor de infrarrojo (seguidor de línea) BFD-1000.
- Modulo Bluetooth HC-05.
- Servomotor SG90.
- Dos ruedas con sus respectivos motores DC.
- Puente H.
- Chasis.


## Mapa de Memoria General

Inicialmente se planteó una estructura de organización para cada uno de los registros que se pensaban utilizar por cada periférico implementado, siendo condensada dicha estructura en un mapa de memoria para cada caso. Primero se contruyó el mapa de memoria general, en donde se indica en qué posición de memoria inician los registros de cada periférico, definiendo un tamaño de 32 bytes para cada uno, a excepción de la RAM y la SRAM, las cuales deben tener un mayor tamaño debido a la cantidad de datos que almacenan.

![image](https://user-images.githubusercontent.com/92388558/152260404-e48b593e-7f58-4ace-8166-2372b95e602d.png)


Con esto entonces se diseñaron los diversos periféricos planteados para conformar el robot cartógrafo, iniciando con el desarrollo del periférico correspondiente al infrarrojo, tal como se puede ver a continuación:

1. [ Infrarrojo. ](#Infrarrojo)
2. [ Motores. ](#Motores)
3. [ Radar. ](#Radar)
4. [ UART. ](#UART)
5. [ I2C. ](#I2C)
6. [ Cámara. ](#Cámara)
7. [ Montaje del Robot y Pruebas Finales. ](#Montaje)
8. [ Bibliografía. ](#Bibliografía)


## Infrarrojo

Para el periférico del infrarrojo se definen tres registros: `read`, `write` y `dir`, los cuales se encargan de permitir obtener y almacenar los datos que el sensor envía. En este caso los tres registros son de lectura y escritura, cada uno de un tamaño de 4 bits. El mapa de memoria construido se ilustra a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260901-054ff930-e5d0-402b-99af-4f2ac66e55f9.png)


Para implementar el sensor de infrarrojo se utilizó el sensor de seguimiento de linea BFD-1000 que cuenta con 5 sensores infrarrojos, tal como se muestra en la siguiente imagen:

![infra1](https://user-images.githubusercontent.com/92388558/153109712-89b3b481-5225-40f7-828e-785313257cef.png)

A continuación se muestra la descripción del código realizado en verilog, donde iL, iLC, iC, iRC e iR representan las señales de entrada del sensor:

- iL  - Sensor izquierdo más lejano al centro.
- iLC - Sensor izquierdo más cercano al centro.
- iC  - Sensor central.
- iRC - Sensor derecho más cercano al centro.
- iR  - Sensor derecho más lejano al centro.

Además L, LC, C, RC y R representan las salidas del módulo. Debido a que el sensor BFD-1000 es un sensor digital, el proceso se realiza asignando el valor de los pines de entrada a los pines de salida, es decir un 0 o un 1 dependiendo de si está leyendo una línea negra o una línea blanca respectivamente.

![infra2](https://user-images.githubusercontent.com/92388558/153109981-1bd77b88-afe3-4cd5-864b-e43fc98c5d0c.png)

Luego se realizó la implementación por medio de Python, con la cual se interconectaron cada una de las señales de entrada y salida del módulo del infrarrojo con las referencias de los pines físicos de la FPGA correspondientes, utilizando el método de Python `__init__`. En este caso se incluyen en dicho método las señales de entrada y salida, que son los argumentos de la clase `ir`, con el fin de inicializar el atributo correspondiente a cada una. Con esto entonces se especificará cada atributo de la instancia como `self.atributo`, con lo cual se muestra que la clase `ir` tiene dicho atributo, asingnándole el valor correspondiente [[1](https://www.delftstack.com/es/howto/python/self-in-python/)]. Además, se define el tipo de registro y la cantidad de bits utilizados de cada una de las salidas (todas son de 1 bit), siendo todos registros de tipo `Status` debido a que brindan la posición de memoria que el SoC puede leer (únicamente son de lectura) [[2](https://github.com/enjoy-digital/litex/blob/master/litex/soc/interconnect/csr.py)]. Por último, se crean los objetos del driver `infrarrojo`, asignándoles el valor almacenado en los atributos correspondientes. El proceso descrito se muestra a continuación:

![infra3](https://user-images.githubusercontent.com/92388558/153111305-151ff5fb-90b4-490e-acdd-b6956288fe0a.png)

Por último, se instanció el periférico del infrarrojo en el archivo `buildSoCproject.py` generando el driver `ir_driver` y definiendo los pines de entrada según la información brindada por el driver creado en el archivo `infrarrojo.py` (en la clase `ir`). Estos pines se relacionan con los pines físicos gracias a la plataforma definida, que en ese caso es la tarjeta Nexys4DDR, definiendo los nombres de dichos pines dependiendo de los asignados en el archivo `nexys4ddr.py`. El código implementado a continuación:

![infra4](https://user-images.githubusercontent.com/92388558/153111311-69e165d1-daaa-430b-bc7a-8440550dc648.png)


## Motores

Para el periférico de los motores se utilizó un único registro de lectura y escritura de 4 bits que se encarga de definir el movimiento de los motores DC a utilizar, dependiendo de la codificación requerida por el puente H, el cual controla dichos motores. Con esto entonces se construyó el siguiente mapa de memoria:

![image](https://user-images.githubusercontent.com/92388558/152260787-a9ec6321-36d7-467d-b7d3-655596e4e367.png)

Cabe resaltar que la codificación de los motores dada por el puente H es la ilustrada en la siguiente tabla:

![Tabla 1](https://user-images.githubusercontent.com/92388558/152260815-00da78f2-fa9d-409e-9b7c-f7f58820eceb.png)



El puente H utilizado controla los motores según la codificación de la señal que recibe, entregando una instrucción de 2 bits a cada motor para que el robot se movilice en una determinada dirección. En este caso el sistema de motores montado en el chasis utilizado se ilustra en la siguiente imagen:

<img src="https://user-images.githubusercontent.com/92388558/153331110-ffb2355c-2163-4166-8bc1-d759afda07e7.JPG" width="500">

Para la construcción del módulo se tuvo en cuenta el reloj interno de la FPGA y los parámetros de entrada recibidos por el registro `movimiento`, entregando a la salida la instrucción de movimiento que recibe el puente H. Se definen entonces cuatro parámetros que indican la dirección en la que se tiene que mover el robot, los cuales son `A` (avanza), `R` (retrocede), `P` (pausa), `GD` (giro a la derecha) y `GI` (giro a la izquierda). Se debe hacer énfasis en que, para indicar las instrucciones de cada motor de manera correcta, se debe tener en cuenta que estos operan de manera simétrica (como en un espejo) ya que si, por ejemplo, se requiere que el robot avance en una línea recta, un motor debe girar en una dirección y el motor del otro lado debe girar en sentido contrario, tal como se ilustra en el siguiente ejemplo:

![Giro-de-motor](https://user-images.githubusercontent.com/82488285/153473137-6f472307-1b36-4784-ac9b-18d44414ba33.png)

En este caso se tiene que el motor de la parte superior gira en sentido horario (retrocediendo el robot, por ejemplo), mientras que el motor de la parte inferior (que corresponde al motor del otro lado del robot) debe girar en sentido antihorario para que se cumpla la condición de retroceso. Con esto entonces se puede ver en el código diseñado que para que el robot avance (opción `A`) ambos motores deben ir en la dirección `0 1` (ver la [Tabla 1](https://github.com/unal-edigital2-labs/wp08-2021-2-gr-03/blob/main/Im%C3%A1genes/Instrucciones%20Motores.jpg)), por lo que la instrucción que se le entregará al puente H será un `0 1` para el motor A y un `1 0` para el motor B, así mismo, para que el robot retroceda (opción `R`) ambos motores deben ir en la dirección `1 0`, por lo que la instrucción que se le entregará al puente H será un `1 0` para el motor A y un `0 1` para el motor B. En el caso de la pausa (opción `P`) ambos motores deben ir en la dirección `0 0`, pero como ninguno de ellos se mueve no hay necesidad de invertir la instrucción para el motor B. En el caso de los giros se debe tener mucho cuidado pues ambos motores deben girar en direcciones opuestas y en sentidos contrarios, es decir, para realizar un giro hacia la derecha el motor A debe recibir la instrucción `0 1` (Rotar Derecha - Avanzar según la tabla) mientras que el motor B debe recibir la instrucción `0 1` ya que corresponde al inverso de la instrucción `1 0` (Rotar Izquerda - Retroceder), por otro lado, para realizar un giro hacia la izquierda el motor A debe recibir la instrucción `1 0` (Rotar Derecha - Retroceso) mientras que el motor B debe recibir la instrucción `1 0` ya que corresponde al inverso de la instrucción `0 1` (Rotar Izquerda - Avanzar). El código construido entonces se ilustra en la siguiente imagen:

![motores2](https://user-images.githubusercontent.com/92388558/153112643-3000f310-da91-46f0-98f2-34b0ced0da81.png)

Posteriormente se realizó la implementación por medio de Python, tal como se ilustra en la siguiente imagen:

![motores3](https://user-images.githubusercontent.com/92388558/153112407-af8a2eea-5ad9-4855-8be2-a361639ce087.png)

En este caso se interconectaron cada una de las señales de entrada y salida del módulo de los motores con las referencias de los pines físicos de la FPGA correspondientes, utilizando el método de Python `__init__`. En este caso se incluyen en dicho método las señales de entrada y salida, que son los argumentos de la clase `mt`, con el fin de inicializar el atributo correspondiente a cada una. Con esto entonces se especificará cada atributo de la instancia como `self.atributo`, tal como se explicó en la parte del [Infrarrojo](#infrarrojo). Además, se define el tipo de registro y la cantidad de bits utilizados de cada una de las entradas: en el caso del reloj `clk` se especifica que dicha señal corresponde al reloj interno de la FPGA con la función `ClockSignal()`, mientras que en el caso de la señal de entrada de 3 bits `movimiento` se define dicho registro como uno de tipo `Storage` debido a que brinda la posición de memoria al SoC para su lectura o escritura [[2](https://github.com/enjoy-digital/litex/blob/master/litex/soc/interconnect/csr.py)]. Finalmente, se crean los objetos del driver `motores`, asignándoles el valor almacenado en los atributos correspondientes.

Por último, se instanció el periférico de los motores en el archivo `buildSoCproject.py`, generando el driver `mt_driver` y definiendo los pines de entrada según la información brindada por el driver creado en el archivo `motores.py` (en la clase `mt`). Estos pines se relacionan con los pines físicos gracias a la plataforma definida, que en ese caso es la tarjeta Nexys4DDR, definiendo los nombres de dichos pines dependiendo de los asignados en el archivo `nexys4ddr.py`. El código implementado a continuación:

![motores4](https://user-images.githubusercontent.com/92388558/153112428-dbc1b0f7-8f90-44ea-bbe3-f09426d8a1b5.png)




## Radar

Para el radar se decide implementar de manera conjunta el módulo PWM, el del servomotor y el del ultrasonido. El propósito de tener estos tres juntos hace que se pueda realizar un seguimiento más adecuado al proyecto, pudiendo modificar o realizar el debido mantenimiento de una manera más eficiente. En este caso el mapa de memoria incluye los registros `init` (único de lectura y escritura, el resto son únicamente de lectura), `done` y los datos de la posición entregados por la operación conjunta de estos módulos, teniendo cada uno un tamaño de 4 bits como en el anterior mapa de memoria:

![image](https://user-images.githubusercontent.com/92388558/152260580-c6536ac5-ab7f-4783-915c-954f5ce0afb3.png)

El radar está constituido por el módulo de ultrasonido HC-SRO4 y el servomotor SG90, por lo que se debió realizar dos veces el mismo proceso que con los anteriores periféricos. A continuación se muestra la descripción del código de verilog para el ultrasonido basado en el programa implementado por el [grupo 11 del semestre 2021-1](https://github.com/unal-edigital2-labs/2021-1-w07_entrega-_final-grupo11):

![us1](https://user-images.githubusercontent.com/92388558/153326680-40f9857f-a0aa-4109-990e-eb47225fc63d.png)

En este código se hace uso del reloj interno de la FPGA así como también se utiliza un registro de inicio (`init`) y un registro que almacena el dato de la recepción del ultrasonido enviado (`echo`), por otro lado, las salidas del módulo corresponden al disparo del ultrasonido (`trig`), la distancia que recorre dicho pulso (`distance`) y el registro de finalización (`done`). Así mismo, se utilizan tres registros para el correcto funcionamiento del proceso, los cuales son un contador que se encarga de contar el tiempo del disparo del pulso (`counter`), un registro que indica si el pulso del ultrasonido ya fue emitido (`echoStart`) y el registro que se encarga de definir el estado de la máquina de estados implementada. Además, para medir los diferentes tiempos se utilizará un reloj de periodo de 1 microsegundo, por lo que se definirá un nuevo reloj (`newCLK`) junto con el módulo del divisor de frecuencia que lo genera (`divFreq`). Posteriormente se construye una máquina de estados que en general tiene tres estados, el primero es el inicial, en el cual todos los reistros se inicializan en cero, posteriormente está el estado del pulso, en el cual se dispara la señal de ultrasonido de una duración de 11 microsegundos, controlando dicho tiempo mediante el contador, y por último está el estado del echo, el cual calcula la distancia que recorrió el pulso a partir del tiempo transcurrido desde que se dejó de emitir el pulso hasta que se recibe la señal del trigger. El código construido entonces se ilustra en las siguientes imágenes:

![us2](https://user-images.githubusercontent.com/92388558/153326692-74947cc3-cc75-4dd3-80d9-9f4730198798.png)
![us3](https://user-images.githubusercontent.com/92388558/153326702-50d04a21-04a9-4c39-b5de-a9d9b73299f4.png)

Ahora se muestra la implementación por medio de Python para el ultrasonido:

![us4](https://user-images.githubusercontent.com/92388558/153326870-c0ece5e1-cf98-47d5-b70c-0cd0e4219144.png)

En este caso se interconectaron cada una de las señales de entrada y salida del módulo del ultrasonido con las referencias de los pines físicos de la FPGA correspondientes, utilizando el método de Python `__init__`. En este caso se incluyen en dicho método las señales de entrada y salida, que son los argumentos de la clase `us`, con el fin de inicializar el atributo correspondiente a cada una. Con esto entonces se especificará cada atributo de la instancia como `self.atributo`, tal como se explicó en la parte del [Infrarrojo](#infrarrojo). Además, se define el tipo de registro y la cantidad de bits utilizados de cada una de las entradas: en el caso del reloj `clk` se especifica que dicha señal corresponde al reloj interno de la FPGA con la función `ClockSignal()`, en el caso de la señal de inicio de 1 bit `init` se define dicho registro como uno de tipo `Storage` debido a que brinda la posición de memoria al SoC para su lectura o escritura, mientras que los registros `distance` (de 10 bits) y `done` (de 1 bit) se definen como de tipo `Storage` debido a que brindan la posición de memoria que el SoC puede leer. Finalmente, se crean los objetos del driver `ultrasonido`, asignándoles el valor almacenado en los atributos correspondientes.

Por último, se instanció el periférico del ultrasonido en el archivo `buildSoCproject.py`, generando el driver `us_driver` y definiendo los pines de entrada según la información brindada por el driver creado en el archivo `ultrasonido.py` (en la clase `us`). Tal como en los periféricos anteriores, estos pines se relacionan con los pines físicos gracias a la plataforma definida, que en ese caso es la tarjeta Nexys4DDR, definiendo los nombres de dichos pines dependiendo de los asignados en el archivo `nexys4ddr.py`. El código implementado se muestra a continuación:

![us5](https://user-images.githubusercontent.com/92388558/153327313-2081f45c-170f-435d-8b8d-4854e3d02919.png)


Posteriormente se desarrolló la descripción del código de Verilog para el servomotor utilizando nuevamente el reloj interno de la FPGA y un registro (`pos`) que se encarga de recibir las instrucciones de movimiento que el servomotor va a recibir por medio de la señal `servo` después de su traducción. El proceso se basa en el valor de un contador (registro `contador`) que aumenta cada ciclo positivo del reloj de la FPGA y se reinicia cuando llega a 1000000, con lo cual se determina el ángulo al cual se configurará el servomotor. Cuando la señal de la posición que es ingresada es `0 0` el servomotor se configura en un ángulo de 0 grados gracias a que el contador aumenta hasta alcanzar un valor de 50000. Si la señal ingresada es `0 1` el servomotor se configura en un ángulo de 90 grados gracias a que el contador aumenta hasta alcanzar un valor de 150000. Por último, cuando la entrada es `1 0` el servomotor se configura en un ángulo de 180 grados gracias a que el contador aumenta hasta alcanzar un valor de 200000. Con esto entonces se puede evidenciar que el contador determina la amplitud a la cual el servomotor se configura, definiendo un ángulo mayor a medida que se aumenta su valor. El código construido entonces es el ilustrado a continuación:

![sv1](https://user-images.githubusercontent.com/92388558/153329096-80c64f61-3d31-4bc3-a190-d29c9fd01bca.png)

A continuación se puede observar la implementación del servomotor por medio de Python:

![sv2](https://user-images.githubusercontent.com/92388558/153329390-5873305a-9623-4519-b9ba-b0be2d873ad0.png)

En este caso se interconectaron cada una de las señales de entrada y salida del módulo del servomotor con las referencias de los pines físicos de la FPGA correspondientes, utilizando el método de Python `__init__`, tal como se viene haciendo con todos los periféricos. En este caso se incluyen en dicho método las señales de entrada y salida, que son los argumentos de la clase `servoUS`, con el fin de inicializar el atributo correspondiente a cada una. Con esto entonces se especificará cada atributo de la instancia como `self.atributo`, tal como se explicó en la parte del [Infrarrojo](#infrarrojo). Además, se define el tipo de registro y la cantidad de bits utilizados de cada una de las entradas: en el caso del reloj `clk` se especifica que dicha señal corresponde al reloj interno de la FPGA con la función `ClockSignal()`, mientras que en el caso de la señal de entrada de 2 bits `pos` se define dicho registro como uno de tipo `Storage` debido a que brinda la posición de memoria al SoC para su lectura o escritura. Finalmente, se crean los objetos del driver `servo`, asignándoles el valor almacenado en los atributos correspondientes.

Por último, se instanció el periférico del servomotor en el archivo `buildSoCproject.py`, generando el driver `servo_driver` y definiendo los pines de entrada según la información brindada por el driver creado en el archivo `servo.py` (en la clase `servoUS`). Tal como en los periféricos anteriores, estos pines se relacionan con los pines físicos gracias a la plataforma definida, que en ese caso es la tarjeta Nexys4DDR, definiendo los nombres de dichos pines dependiendo de los asignados en el archivo `nexys4ddr.py`. El código implementado se muestra a continuación:

<img src="https://user-images.githubusercontent.com/82488285/153637976-b419ad33-d6f3-4932-a654-3aa7d887875b.png" width="500">



## UART

Como el módulo de Bluetooth se comunica utilizando el protocolo de comunicación UART, para el desarrollo de este periférico únicamente se tienen en cuenta los registros básicos de dicha comunicación: `RXTX` y `baudios`, los cuales se encargan de transmitir los datos y de establecer la frecuencia a la cual se transmiten dichos datos. En este caso ambos registros son de lectura y escritura, cada uno de un tamaño de 4 bits. Con esto entonces se construyó el mapa de memoria ilustrado a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260533-bed31836-c504-4048-9d4b-ca2219d9d602.png)

Se implementó un módulo UART para poder enviar y recibir los diferentes datos que se observarán por medio de un lector de serial Bluetooth en un teléfono celular. A continuación se muestra el código implementado en el SoC del proyecto (archivo `buildSoCproject.py`) en el cual se importa directamente de Litex un módulo de comunicación UART para su uso específico con el módulo de bluetooth.

![bt1](https://user-images.githubusercontent.com/92388558/153327906-514506eb-a5ab-4eea-8114-46509ff7beaa.png)


## I2C
Debido a que el sensor de temperatura y humedad SHT31 utiliza el protocolo de comunicación I2C, se implementará un módulo de I2C para su implementación. A continuación se muestra el código de Verilog utilizado:

![I2C](https://user-images.githubusercontent.com/92388558/153329980-8b509c84-380e-43de-bce2-c32128106c07.png)

## Sensor de Temperatura y Humedad - I2C Master

Como el sensor de temperatura y humedad utiliza un protocolo de comunicación I2C, en el mapa de memoria construido se incluyen los registros característicos de dicha comunicación, los cuales son `sda` (registro de los datos), `scl` (registro del reloj empleado en la comunicación) y el reloj interno de la FPGA `clk`. En este caso todos los registros son de lectura y escritura, teniendo un tamaño de 4 bits cada uno. El mapa de memoria se ilustra a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260659-46fae33a-f907-4c84-bba0-4f59c06c7d4d.png)



## Camara
El procesamiento de la imagen se realizó por medio de hardware debido a que es más eficiente y además se tiene la posibilidad de procesar más datos, a la vez que se libera capacidad del procesador. A continuación se ilustra el mapa de memoria construido, el cual fue realizado con base en los registros `init` (único de lectura y escritura, el resto son únicamente de lectura), `done` y los datos de `figura` y `color` entregados, teniendo cada uno un tamaño de 4 bits:

![image](https://user-images.githubusercontent.com/92388558/152260713-95e9bbd3-05b8-4f3e-963a-ab27da3a6557.png)


## VGA
En el caso del periférico del VGA se tienen únicamente dos registros: `RGB` y `coordenadas`, los cuales son los encargados de almacenar los datos del color de los pixeles (Red Green Blue) y las coordenadas evaluadas para determinar la forma de la figura captada por la cámara. En este caso ambos registros son únicamente de lectura, el primero de un tamaño de 12 bits y el segundo de un tamaño de 4 bits. El mapa de memoria a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260920-4152c507-ba86-4421-afad-13d9cef7c200.png)

Para realizar la implementación del periférico de la cámara en conjuto con el VGA, se debe tener en cuenta que el conector de 15 pines de este último cuenta con 5 pines asignados al proceso de recepción de los datos enviados por el SoC, los cuales son RED (pin 1), GREEN (pin 2), BLUE (pin 3), HSYNC (pin 13) y VSYNC (pin 14). Además, se debe aclarar que la velociadad a la que operan tanto la cámara como el VGA escuatro veces menor que la velocidad de la FPGA. 

![image](https://user-images.githubusercontent.com/82488285/153674539-37a2fa1d-8b73-4603-992b-459bf008f269.png)

En este caso, se decidió realizar el procesamiento de las imágenes recibidas por la cámara mediante hardware. Con esto, el proceso se realizará utilizando diferentes módulos, los cuales se encargan de recibir los datos de la cámara utilizando el protocolo I2C, interpretando las diferentes variables enviadas y realizando el procesado de las imágenes para identificar el color y la figura captados.

# Montaje del Robot y Pruebas Finales

A continuación se muestra el diseño que se implemento para el montaje de nuestro robot cartografo.

![WhatsApp Image 2022-02-11 at 9 01 42 PM](https://user-images.githubusercontent.com/92388558/153692078-6c808a47-8cbc-442f-bfc6-27b85e02dfdc.jpeg)

![WhatsApp Image 2022-02-11 at 9 01 43 PM](https://user-images.githubusercontent.com/92388558/153692091-9bdc923f-6209-4277-b780-741d5da43894.jpeg)

![WhatsApp Image 2022-02-11 at 5 10 01 PM(1)](https://user-images.githubusercontent.com/92388558/153692096-50812bd0-ff77-42cd-813f-8031b9d184f4.jpeg)

![WhatsApp Image 2022-02-11 at 5 10 01 PM](https://user-images.githubusercontent.com/92388558/153692098-cdd41b7d-7ac4-4f3c-97c9-d82bb9d1f96d.jpeg)

A continuacion se muestra el diseño de la pista en la que se realizaron las pruebas finales del robot.

![WhatsApp Image 2022-02-11 at 9 01 03 PM](https://user-images.githubusercontent.com/92388558/153692106-97e373ab-60d9-495c-bdfb-6d8f41504e6b.jpeg)


# Bibliografía
[1]   Anónimo, "La palabra clave self en Python", DelftStack, 2021. Dispoible en: https://www.delftstack.com/es/howto/python/self-in-python/

[2]   A. Dennison, "csr.py", GitHub, 2021. Disponible en: https://github.com/enjoy-digital/litex/blob/master/litex/soc/interconnect/csr.py


