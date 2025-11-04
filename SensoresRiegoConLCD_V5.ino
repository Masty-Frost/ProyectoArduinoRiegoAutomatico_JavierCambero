/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <LiquidCrystal.h>
#include "DHT.h"

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.

// These #defines make it easy to set the backlight color
#define WHITE 0x7
#define DHTPIN 2
#define DHTTYPE DHT11
#define RELEPIN 3

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);

// GLOBAL VARIABLES
unsigned long previousMillis = 0;
const long interval = 1000;
unsigned long previousMillis2 = 0;
// Tiempo entre lecturas de los sensores cuando no se está regando
const long interval2_sinriego = 30000;
// Tiempo entre lecturas de los sensores cuando se está regando
const long interval2_conriego = 5000;
// Tiempo entre lecturas de los sensores
long interval2 = interval2_sinriego;

// Se establece el valor umbral hasta el que hay que regar dependiendo de si hace más calor o hay mayor humedad en el aire
int valor_humedad_riego_normal = 900;
int valor_humedad_riego_extra = 800;
int valor_humedad_riego_reducido = 1000;

// Se establecen los valores de los umbrales a partir de los cuales se regará (umbral_seco), hará más calor y por tanto habrá que regar más (temperatura_min)
// y hay mayor nivel de humedad en el aire y por tanto no hace falta regar tanto (umbral_humedad_aire)
int umbral_seco = valor_humedad_riego_normal;
float temperatura_min = 25.00;
float umbral_humedad_aire = 60.00;

// Se inicializan las variables a un valor que iremos variando a medida que vayamos leyendo los valores de los sensores
int sensorHumedadSueloValue = 0;
float h = 0.00;
float t = 0.00;

// Inicializamos los acumuladores que irán acumulando los valores de las lecturas de los sensores y el contador de lecturas
long int acumulador_sensorHumedadSueloValue = 0;
float acumulador_humedad = 0.00;
float acumulador_temperatura = 0.00;
int contadorLecturas = 0;

// Se inicializan los valores de las medias de los valores de los sensores, para evitar fluctuaciones de los valores de forma inesperada
long int media_sensorHumedadSueloValue = 0;
float media_humedad = 0.00;
float media_temperatura = 0.00;

// El valor de si los sensores están listos o no se inicializa a false para después cambiar su valor a true cuando se haya realizado la primera lectura de sensores válida
bool sensoresListos = false;

// Bandera que cambiará su valor a true cuando se hayan leído correctamente los sensores por primera vez
bool primeraVez = false;

// Bandera que cambia su valor a true si se está regando la planta y, en consecuencia, se calculará la media de las lecturas de los sensores cada menos tiempo
bool regando = false;

// Bloque de configuración inicial
void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  dht.begin();
  // Configuramos el pin del relé como pin de salida
  pinMode(RELEPIN, OUTPUT);
  // Inicializamos el relé a un nivel bajo para que esté desactivado hasta que se realice una lectura
  digitalWrite(RELEPIN, LOW);
  // Lanzamos un mensaje de bienvenida al iniciar el programa
  Serial.println("=================================");
  Serial.println("  SISTEMA DE RIEGO AUTOMÁTICO");
  Serial.println("  Iniciando programa de riego...");
  Serial.println("=================================");

  // Lanzamos un mensaje de bienvenida en la pantalla LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(3000);
  lcd.clear();
}

// Bloque iterativo que ejecuta el programa
void loop() {
  unsigned long currentMillis = millis();
  // Leemos los valores de los sensores cada 1 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Leemos el valor del sensor de humedad del suelo en la entrada analógica "A1"
    sensorHumedadSueloValue = analogRead(A1);
    // Leemos el valor de la humedad del aire en el sensor DHT11
    h = dht.readHumidity();
    // Leemos el valor de la temperatura del ambiente en el sensor DHT11
    t = dht.readTemperature();
    // En caso de que los valores leídos por los sensores no sean los correctos, no se tendrán en cuenta
    if ((!isnan(h) && !isnan(t) && !isnan(sensorHumedadSueloValue)) && (primeraVez == false)) {
      acumulador_sensorHumedadSueloValue = acumulador_sensorHumedadSueloValue + sensorHumedadSueloValue;
      acumulador_humedad = acumulador_humedad + h;
      acumulador_temperatura = acumulador_temperatura + t;
      contadorLecturas = contadorLecturas + 1;
    }
    // En caso de que haya habido una lectura correcta por primera vez
    else if ((!isnan(h) && !isnan(t) && !isnan(sensorHumedadSueloValue)) && (primeraVez == true) && (sensorHumedadSueloValue >= media_sensorHumedadSueloValue-90 && sensorHumedadSueloValue <= media_sensorHumedadSueloValue+90)) {
      acumulador_sensorHumedadSueloValue = acumulador_sensorHumedadSueloValue + sensorHumedadSueloValue;
      acumulador_humedad = acumulador_humedad + h;
      acumulador_temperatura = acumulador_temperatura + t;
      contadorLecturas = contadorLecturas + 1;
    }
  }
  // Si se está regando, se calculará la media de las lecturas de los sensores cada 5 segundos
  if (regando == true) {
    interval2 = interval2_conriego;
  }
  // Si no se está regando, se calculará la media de las lecturas de los sensores cada 30 segundos
  else {
    interval2 = interval2_sinriego;
  }
  // Calculamos la media de los valores de los sensores cada más o menos tiempo según se esté regando o no (una vez que se han leído sus valores varias veces)
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    // Se calcularán las medias siempre y cuando no hayan sido erróneas todas las lecturas
    if (contadorLecturas > 0) {
      // Calculamos las medias de los valores de los sensores
      media_sensorHumedadSueloValue = acumulador_sensorHumedadSueloValue / contadorLecturas;
      media_humedad = acumulador_humedad / contadorLecturas;
      media_temperatura = acumulador_temperatura / contadorLecturas;
      // Imprimimos por el monitor serie los valores de las medias
      Serial.print("Humedad del suelo es: ");
      Serial.println(media_sensorHumedadSueloValue);
      Serial.print("Humedad del aire es: ");
      Serial.print(media_humedad);
      Serial.println(" %");
      Serial.print("Temperatura es: ");
      Serial.print(media_temperatura);
      Serial.println(" ºC");
      primeraVez = true;
      sensoresListos = true;
    }
    else {
      // Si todas las lecturas han sido erróneas, se imprimirá que ha habido un error en todas las lecturas
      Serial.println("¡Error en todas las lecturas de los sensores!");
      lcd.setCursor(0, 0);
      lcd.print("Error en");
      lcd.setCursor(0, 1);
      lcd.print("lecturas");
      sensoresListos = false;
    }
    // Dejamos un hueco entre informaciones de datos
    Serial.println(" ");
    acumulador_sensorHumedadSueloValue = 0;
    acumulador_humedad = 0.00;
    acumulador_temperatura = 0.00;
    contadorLecturas = 0;
  }
  // Si se supera una temperatura, hará más calor y por tanto la planta debe alcanzar un mayor nivel de humedad
  if (media_temperatura >= temperatura_min) {
    umbral_seco = valor_humedad_riego_extra;
  }
  // Si se supera un valor de humedad del aire, habrá mayor humedad en el aire de lo normal y por tanto la planta deberá alcanzar un menor nivel de humedad
  else if (media_humedad >= umbral_humedad_aire) {
    umbral_seco = valor_humedad_riego_reducido;
  }
  // Si las condiciones climáticas son las normales, se deberá alcanzar el nivel de humedad establecido para cuando no hace más calor y no hay tanta humedad
  else {
    umbral_seco = valor_humedad_riego_normal;
  }
  // Si el valor de humedad del suelo supera el umbral en el que está húmeda la planta y los sensores han realizado una lectura correcta,
  // se activará el relé y, por tanto, la bomba de agua hasta que el valor de la humedad del suelo alcance el valor configurado en función de si
  // se supera un umbral de temperatura (hace más calor de lo normal), de si la humedad del aire supera un umbral (hay mayor humedad en el ambiente)
  // o si las condiciones son normales (la temperatura no supera el umbral establecido y la humedad del aire tampoco supera el umbral establecido)
  if ((media_sensorHumedadSueloValue >= umbral_seco) && (sensoresListos == true)){
    // Activa el relé en caso de cumplirse que se supera el valor umbral de humedad del suelo
    digitalWrite(RELEPIN, HIGH);
    // Cambiado el valor de regando a true si se está regando
    regando = true;
    // Escribe el texto "Regando la maceta" en la pantalla LCD cuando se activa el relé
    lcd.setCursor(0, 0);
    lcd.print("Regando maceta");
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print("Suelo:");
    lcd.print(media_sensorHumedadSueloValue);
    lcd.print("   ");
  }
  else {
    // Desactiva el relé en caso de haber alcanzado el valor umbral de humedad del suelo
    digitalWrite(RELEPIN, LOW);
    // Cambia el valor de regando a false si no se está regando
    regando = false;
    // Escribe en la pantalla LCD los valores de la humedad del suelo, humedad del aire y temperatura cuando el relé está desactivado
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print((int)media_temperatura);
    lcd.print("C H:");
    lcd.print((int)media_humedad);
    lcd.print("%");
    lcd.print("   ");

    lcd.setCursor(0, 1);
    lcd.print("Suelo:");
    lcd.print(media_sensorHumedadSueloValue);
    lcd.print("   "); // limpia huecos sobrantes
  }  
}