//****** MPU Libraries ******
#include "I2Cdev.h"
#include "MPU6050.h"
#include "MPUManager.h"
#include "string.h"


//Librerias para NRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#include <Wire.h>
// include the GPS library:
#include <TinyGPS.h>
// include the sensor library
#include <Adafruit_MLX90614.h>
// include the PES2018 library
#include <Pes2018GPS.h>

#define CE_PIN 9
#define CSN_PIN 10

/*RF Transmisor
**CE --> Pin 22
**CSN --> Pin 23
**MOSI --> Pin 51
**MISO --> Pin 50
**SCK --> Pin 52
  RF Receptor
**CE --> Pin 30
**CSN --> Pin 31
**MOSI --> Pin 32
**MISO --> Pin 33
**SCK --> Pin 34
  Acelerometro
**SCL --> Pin 21
**SDA --> Pin 20
**INT --> Pin 2

*/


char dataToSendA[80] = "Mensaje: ";        //Array que se va a transmitir
char dataToSendR[32] = "Mensaje: ";
char txNum = '0';
bool rslt;          //Vaiable para confirmacion de recepcion de mensaje

RF24 radio(CE_PIN, CSN_PIN);           //Crear objeto radio tipo RF24

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'}; //Variable con la direccion del Canal para transmitir


//******* MPU objects *******
MPU6050 sensor;

MPUManager MPUInstance(sensor);
float* dataReceived;
float data [6];
String mensaje;
unsigned long time1;
unsigned long time2;
//unsigned long t2 = millis();


/*
   variables GPS
*/
TinyGPS gps;
String stringValGPS = "";
String mensajeMLX;
Pes2018GPS GP = Pes2018GPS(&Serial3);
 double flat, flon;
  unsigned long chars, prec, age;
  unsigned short sentences, failed, sat;

/*
   variables MLX90614 Temp Sensor
*/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  radio.begin();
  mlx.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3, 5); // delay, count
  radio.openWritingPipe(slaveAddress);        //Abrir canal para escribir
  Serial.println("Iniciando...");
  
  MPUInstance.startConnection();
}

void loop() {
  Serial.println("  Ack received.");
  //******* MPU functions ********
  dataReceived = MPUInstance.getData();
  for (int i = 0; i < 6; i++) {
    data[i] = dataReceived[i];
  }
  mensaje = "AX: " + String(data[0]) + " AY: " + String(data[1]) + " AZ: " + String(data[2]) + " RX: " + String(data[3]) + " RY: " + String(data[4]) + " RZ: " + String(data[5]);

  //mensaje.toCharArray(dataToSendA, mensaje1.length() + 1);

  Serial.println(mensaje);
  // Codigo de Transmisor RF para enviar Informacion

 // sendDataMPU();

  delay(1000);



  //***************GPS Functions***************


 
  GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
  stringValGPS += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed ; //combining both whole and decimal part in string with a fullstop between them
  //StringConcatenar(stringValGPS, "char:   ");
  stringValGPS = "";
  Serial.println(stringValGPS);
  sendDataGPS();

  delay(30000);

  //***************MLX Sensor Functions***************




  mensajeMLX = "temperatura ambiente C : " + String(mlx.readAmbientTempC()) + "temperatura ambiente F : " + String(mlx.readAmbientTempF()) + " temperatura Objeto C : " + String(mlx.readObjectTempC()) + " temperatura Objeto F : " + String(mlx.readObjectTempF());

  sendDataMLX();
  delay(28000);
  
}



//****************RF Functions*******************
void sendDataMPU() {
  Serial.println("=> Sending Data...");

  // Serial.println(time1);
  rslt = radio.write(data, sizeof(data)); //Envia la data del array

  //Serial.println(time2);
  Serial.print("=> Data Sent: ");

  if (!rslt) { //Si la informacion se envio y fue recibida
    Serial.println("  Ack received.");
    //updateMessage(); //Actualiza el mensaje con contador txNum
  }
  else {
    Serial.println("  Tx failed!");
  }
}

void sendDataGPS() {
  Serial.println("=> Sending Data...");

  int messageSize = stringValGPS.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSend[1];
    charToSend[0] = stringValGPS.charAt(i);
    rslt = radio.write(charToSend, 1);
  }
  //  rslt = radio.write(stringValGPS,strlen(stringValGPS); //Envia la data del array

  Serial.print("=> Data Sent: ");

  if (!rslt) { //Si la informacion se envio y fue recibida
    Serial.println("  Ack received.");
    //updateMessage(); //Actualiza el mensaje con contador txNum
  }
  else {
    Serial.println("  Tx failed!");
  }
}

void sendDataMLX() {
  Serial.println("=> Sending Data...");

  int messageSize = mensajeMLX.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSendMLX[1];
    charToSendMLX[0] = mensajeMLX.charAt(i);
    rslt = radio.write(charToSendMLX, 1);
  }
  //  rslt = radio.write(stringValGPS,strlen(stringValGPS); //Envia la data del array

  Serial.print("=> Data Sent: ");

  if (!rslt) { //Si la informacion se envio y fue recibida
    Serial.println("  Ack received.");
    //updateMessage(); //Actualiza el mensaje con contador txNum
  }
  else {
    Serial.println("  Tx failed!");
  }
}


