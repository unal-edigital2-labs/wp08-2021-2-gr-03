1. [ Infrarrojo. ](#infrarrojo)
2. [ Motores. ](#motores)
3. [ Radar. ](#radar)
4. [ UART. ](#uart)
5. [ I2C. ](#I2C)
6. [ Camara. ](#camara)

# Infrarrojo

Para implementar el sensor de infrarrojo se utilizo el sensor de seguimiento de linea BFD-1000 el cual cuenta con 5 sensores infrarrojos como se muestra a continuacion:

![infra1](https://user-images.githubusercontent.com/92388558/153109712-89b3b481-5225-40f7-828e-785313257cef.png)

A continuacion se muestra la descripcion del codigo realizado en verilog donde iC, iL, iLC, iR,iRC representan las señales de entrada del sensor donde:

- iC  - Sensor central.
- iL  - Sensor izquierda mas lejano al centro.
- iLC - Sensor izquierda mas cercano al centro.
- iR  - Sensor derecho mas lejano al centro.
- iRC - Sensor derecho mas cercano al centro.

y L, LC, C, RC,R representan las salidas. Ya que el sensor BFD-1000 es un sensor digital lo que se realiza es asignar el valor de los pines de entrada a los pines de salida, es decir un 0 o un 1 dependiendo de lo que el sensor este leyendo, (linea negra o linea blanca).

![infra2](https://user-images.githubusercontent.com/92388558/153109981-1bd77b88-afe3-4cd5-864b-e43fc98c5d0c.png)

Luego se realiza la implementacion por medio de python de la siguiente manera:

![infra3](https://user-images.githubusercontent.com/92388558/153111305-151ff5fb-90b4-490e-acdd-b6956288fe0a.png)

![infra4](https://user-images.githubusercontent.com/92388558/153111311-69e165d1-daaa-430b-bc7a-8440550dc648.png)


# Motores

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


# UART

# I2C

# Camara
