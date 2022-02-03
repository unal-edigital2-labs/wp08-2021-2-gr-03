# Robot Cartografo - Entrega final

- Daniela Valentina Amaya Vargas
- Julian Andres Silva Cuadros
- Julian Castro

Un robot cartografo es un robot el cual consstruye y guarda mapas, la implementacion de este robot cartografo ademas hara que este pueda recorrer un laberinto mientras va tomando fotos por medio de una camara, a continuacion se muestra la lista de los principales componentes requeridos para la elaboracion de este robot:

## Materiales
- Camara OV7970
- Tarjeta Arduino 
- Tarjeta de desarrollo FPGA, Nexys4
- Sensor de ultrasonido HC-SR04
- Sensor de temperatura y humedad SHT31
- Sensor de infrarrojo
- Modulo Bluetooth HC-05
- Servomotor SG90
- Dos ruedas con sus respectivos motores DC
- Puente H
- Chasis

## Mapa de memoria general
![image](https://user-images.githubusercontent.com/92388558/152260404-e48b593e-7f58-4ace-8166-2372b95e602d.png)

## Perifericos
Para el desarrollo del robot cartografo se utilizaron los siguientes perifericos donde se muestra una breve descripcion del funcionamiento, el mapa de memoria y el codigo de cada uno.

### Camara OV7970
El procesamiento de la imagen se realizo el procesamiento de imagen por medio de hardware debido a que es mas eficiente el procesamiento y ademas tiene posibilidad de procesar mas datos, a la vez que se libera capacidad del procesador.

![image](https://user-images.githubusercontent.com/92388558/152260713-95e9bbd3-05b8-4f3e-963a-ab27da3a6557.png)

### Radar
Para el radar se decide implementarlo de manera conjunta, es decir, PWM, servomotor y ultrasonido. El propósito de tener el PWM, servlmotor y ultrasonido juntos hace que se pueda modificar o realizar mantenimiento al proyecto de una manera más eficiente. 

![image](https://user-images.githubusercontent.com/92388558/152260580-c6536ac5-ab7f-4783-915c-954f5ce0afb3.png)

### I2C master
![image](https://user-images.githubusercontent.com/92388558/152260659-46fae33a-f907-4c84-bba0-4f59c06c7d4d.png)

### VGA
![image](https://user-images.githubusercontent.com/92388558/152260920-4152c507-ba86-4421-afad-13d9cef7c200.png)

### Sensor infrarrojo
![image](https://user-images.githubusercontent.com/92388558/152260901-054ff930-e5d0-402b-99af-4f2ac66e55f9.png)

### Modulo Bluetooth
![image](https://user-images.githubusercontent.com/92388558/152260533-bed31836-c504-4048-9d4b-ca2219d9d602.png)

### Motores
![image](https://user-images.githubusercontent.com/92388558/152260787-a9ec6321-36d7-467d-b7d3-655596e4e367.png)
A continuacion se muestra la codificacion de 2 motores DC que van a ser controlados mediante un Puente H
![image](https://user-images.githubusercontent.com/92388558/152260815-00da78f2-fa9d-409e-9b7c-f7f58820eceb.png)

### Sensor de temperatura y humedad



