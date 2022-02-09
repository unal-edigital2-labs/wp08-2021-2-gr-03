1. [ Infrarrojo. ](#infrarrojo)
2. [ Motores. ](#motores)
3. [ Radar. ](#radar)
4. [ Temperatura y humedad. ](#temperatura y humedad)
5. [ Camara. ](#camara)

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

# Motores

# Radar

# Temperatura y humedad

# Camara
