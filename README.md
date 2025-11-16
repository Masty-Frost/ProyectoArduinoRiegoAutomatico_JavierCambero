# Sistema de Riego Automático con Arduino

Este proyecto implementa un **sistema de riego automático** que activa o detiene el riego en función de parámetros ambientales medidos por sensores. El objetivo es automatizar el riego de plantas optimizando el consumo de agua y reduciendo la intervención manual.

---

## 🌱 Descripción general

El sistema mide continuamente:

- **Humedad del aire**  
- **Temperatura ambiente**  
- **Humedad del suelo**

Usa los siguientes sensores:

- **DHT11** → humedad del aire y temperatura  
- **Capacitive Soil Moisture Sensor v1.2** → humedad del suelo  

En base a estos valores, se realiza una media de los valores leídos de forma correcta y el Arduino decide si la tierra necesita riego. Si la humedad del suelo supera un umbral definido, activa una **bomba de agua** o una **electroválvula** mediante un módulo de relé.

---

## 🔧 Materiales utilizados

- **Arduino Uno**
- **Placa protoboard**
- **Cables de conexión**
- **Fuente de alimentación externa de 6V y 1A**
- **Sensor de humedad del suelo — Capacitive Soil Moisture Sensor v1.2**
- **Sensor DHT11 — temperatura y humedad del aire**
- **Módulo de relé**
- **Escudo de display LCD 16x2 (Shield LCD 16x2)**
- **Bomba de agua sumergible o electroválvula**

---

## ⚙️ Funcionamiento del sistema

1. El Arduino hace varias lecturas de:
   - Humedad del aire (%)  
   - Temperatura del ambiente (°C)  
   - Humedad del suelo (valor analógico)

2. El LCD 16x2 muestra en tiempo real los valores de las medias de los valores de:
   - Humedad del aire  
   - Temperatura  
   - Humedad del suelo 

3. Si el valor de humedad del suelo está por encima del umbral establecido:
   - El relé se activa  
   - La bomba de agua o electroválvula comienza a regar
   - Se calculan las medias de los valores de los sensores cada menos tiempo para asegurar que la cantidad de agua no excede la necesaria

4. Una vez alcanzado el nivel de humedad adecuado:
   - El relé se desactiva  
   - El riego se detiene
   - Se vuelven a calcular las medias de los valores de los sensores cada más tiempo

El proceso se repite de forma automática y continua.

---

## 🛠️ Montaje e instalación

1. **Conexiones principales**
- Soldar más pines al escudo de la pantalla LCD para poder utilizar los pines que han sido ocupados en la placa Arduino Uno para conectar el escudo de la pantalla LCD.
- Seguidamente, conectar el escudo de la pantalla LCD a los pines de la placa Arduino Uno.
- Ajustar brillo de la pantalla LCD mediante el tornillo habilitado para ello hasta que se crea que es suficiente.
- Conectar a la protoboard dos cables, uno de alimentación de "Vin" procedente del pin correspondiente al pin de "Vin" en el escudo de la pantalla LCD y que está conectado al pin "Vin" de la placa Arduino Uno y otro de "GND" procedente del pin correspondiente al pin de "GND" en el escudo de la pantalla LCD y que está conectado al pin de "GND" de la placa Arduino Uno.
- Conectar el sensor de humedad del suelo a 3 cables, uno que irá a la alimentación de "Vin" de la protoboard, otro que irá a la conexión de "GND" de la protoboard y otro más que se conectará a la entrada analógica “A1” del escudo de la pantalla LCD (conectado este también a la entrada analógica “A1” de la placa Arduino Uno) y así mismo se habrá realizado el conexionado de cada cable con su respectivo pin de la placa de Arduino Uno ("GND" del sensor con "GND" de la placa Arduino Uno, "Vcc" del sensor con "Vin" de la placa Arduino Uno y "AOUT" del sensor con el pin analógico “A1” de la placa Arduino Uno. Tras ello, realizaremos pruebas y comprobaremos que funciona correctamente.
- Conectar el sensor de humedad y temperatura (DHT11) en 3 huecos de la protoboard para posteriormente conectar cables en serie con dicho sensor que vayan de la siguiente manera: conectando (-) con "GND", (+) con "Vin" y “S” con el pin digital “2” del escudo de la pantalla LCD. Tras ello, realizaremos pruebas y comprobaremos que funciona correctamente.
- Conectar el módulo de relé a nuestra placa de Arduino Uno para controlar el voltaje que le llega a la bomba de agua en función de los valores que recibamos de los sensores. Para ello, conectaremos de la siguiente manera los pines del módulo de relé: “S” al pin digital “3”, (+) a la alimentación de "Vin" de la protoboard y (-) a "GND" en la protoboard. Para conectar la bomba de agua al relé, lo que haremos será conectar el extremo positivo de la bomba al NO (normalmente abierto), el extremo negativo de la bomba a "GND" en la protoboard y conectaremos un cable entre la línea de alimentación de "Vin" de la protoboard y entre el Común del relé.
- Una vez realizado todo este conexionado, conectaremos a la placa de Arduino una fuente externa de 6V y 1A que servirá para estabilizar la caída de voltaje que sufre el circuito al activarse la bomba de agua.

2. **Subir el código**
   - Conectar Arduino al PC  
   - Abrir el IDE de Arduino  
   - Cargar y subir el sketch del proyecto

---

## ▶️ Uso del sistema

Una vez encendido:

- Arduino lanza un mensaje de bienvenida.
- Arduino realiza varias lecturas de los sensores durante un tiempo para hacer la media de sus valores.
- El LCD muestra los valores.
- Si el suelo está seco según el umbral configurado → riega, muestra en el LCD el valor de humedad del suelo e indica que está regando y reduce el tiempo de cálculo de la media de los valores de los sensores.
- Si el suelo está húmedo → permanece apagado y realiza la media de los valores de los sensores con menor frecuencia.

No se requiere ninguna acción adicional del usuario.

---

## 📈 Resultados

El sistema permite:

- Riego automático basado en datos reales del entorno.  
- Control eficiente del agua.  
- Visualización instantánea de todos los parámetros en pantalla.  
- Reducción de errores humanos en el riego.  
- Mayor autonomía para plantas y cultivos pequeños.

---

## 👨‍💻 Autor

**Javier Cambero Molano**

---

