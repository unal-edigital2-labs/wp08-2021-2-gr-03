# Robot Cartógrafo - Entrega final

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

Inicialmente se planteó una estructura de organización para cada uno de los registros que se pensaban utilizar por cada periférico implementado. Esta estructura se condensó en un mapa de memoria, tal como se puede ver en las siguientes imágenes:

Primero se contruyó el mapa de memoria general, en donde se indica en qué posición de memoria inician los registros de cada periférico, definiendo un tamaño de 32 bytes para cada uno, a excepción de la RAM y la SRAM, las cuales deben tener un mayor tamaño debido a la cantidad de datos que almacenan.

## Mapa de Memoria General
![image](https://user-images.githubusercontent.com/92388558/152260404-e48b593e-7f58-4ace-8166-2372b95e602d.png)

## Periféricos
Para el desarrollo del robot cartografo se utilizaron los perifericos ilustrados a continuación, donde se muestra una breve descripcion del rol que cumple cada registro que conforma el mapa de memoria.

### Cámara
El procesamiento de la imagen se realizó por medio de hardware debido a que es más eficiente y además se tiene la posibilidad de procesar más datos, a la vez que se libera capacidad del procesador. A continuación se ilustra el mapa de memoria construido, el cual fue realizado con base en los registros `init` (único de lectura y escritura, el resto son únicamente de lectura), `done` y los datos de `figura` y `color` entregados, teniendo cada uno un tamaño de 4 bits:

![image](https://user-images.githubusercontent.com/92388558/152260713-95e9bbd3-05b8-4f3e-963a-ab27da3a6557.png)

### Radar
Para el radar se decide implementar de manera conjunta el módulo PWM, el del servomotor y el del ultrasonido. El propósito de tener estos tres juntos hace que se pueda realizar un seguimiento más adecuado al proyecto, pudiendo modificar o realizar el debido mantenimiento de una manera más eficiente. En este caso el mapa de memoria incluye los registros `init` (único de lectura y escritura, el resto son únicamente de lectura), `done` y los datos de la posición entregados por la operación conjunta de estos módulos, teniendo cada uno un tamaño de 4 bits como en el anterior mapa de memoria:

![image](https://user-images.githubusercontent.com/92388558/152260580-c6536ac5-ab7f-4783-915c-954f5ce0afb3.png)

### Sensor de Temperatura y Humedad - I2C Master
Como el sensor de temperatura y humedad utiliza un protocolo de comunicación I2C, en el mapa de memoria construido se incluyen los registros característicos de dicha comunicación, los cuales son `sda` (registro de los datos), `scl` (registro del reloj empleado en la comunicación) y el reloj interno de la FPGA `clk`. En este caso todos los registros son de lectura y escritura, teniendo un tamaño de 4 bits cada uno. El mapa de memoria se ilustra a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260659-46fae33a-f907-4c84-bba0-4f59c06c7d4d.png)

### VGA
En el caso del periférico del VGA se tienen únicamente dos registros: `RGB` y `coordenadas`, los cuales son los encargados de almacenar los datos del color de los pixeles (Red Green Blue) y las coordenadas evaluadas para determinar la forma de la figura captada por la cámara. En este caso ambos registros son únicamente de lectura, el primero de un tamaño de 12 bits y el segundo de un tamaño de 4 bits. El mapa de memoria a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260920-4152c507-ba86-4421-afad-13d9cef7c200.png)

### Sensor Infrarrojo
Para el periférico del infrarrojo se definen tres registros: `read`, `write` y `dir`, los cuales se encargan de permitir obtener y almacenar los datos que el sensor envía. En este caso los tres registros son de lectura y escritura, cada uno de un tamaño de 4 bits. El mapa de memoria construido se ilustra a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260901-054ff930-e5d0-402b-99af-4f2ac66e55f9.png)

### Modulo Bluetooth - UART
Como el módulo de Bluetooth se comunica utilizando el protocolo de comunicación UART, para el desarrollo de este periférico únicamente se tienen en cuenta los registros básicos de dicha comunicación: `RXTX` y `baudios`, los cuales se encargan de transmitir los datos y de establecer la frecuencia a la cual se transmiten dichos datos. En este caso ambos registros son de lectura y escritura, cada uno de un tamaño de 4 bits. Con esto entonces se construyó el mapa de memoria ilustrado a continuación:

![image](https://user-images.githubusercontent.com/92388558/152260533-bed31836-c504-4048-9d4b-ca2219d9d602.png)

### Motores
Por último, para el periférico de los motores se utilizó un único registro de lectura y escritura de 4 bits que se encarga de definir el movimiento de los motores DC a utilizar, dependiendo de la codificación requerida por el puente H, el cual controla dichos motores. Con esto entonces se construyó el siguiente mapa de memoria:

![image](https://user-images.githubusercontent.com/92388558/152260787-a9ec6321-36d7-467d-b7d3-655596e4e367.png)

Cabe resaltar que la codificación de los motores dada por el puente H es la ilustrada en la siguiente tabla:

![image](https://user-images.githubusercontent.com/92388558/152260815-00da78f2-fa9d-409e-9b7c-f7f58820eceb.png)
