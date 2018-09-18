//****** MPU Libraries ******
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "MPUManager.h"

//Librerias para NRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN 9
#define CSN_PIN 10

char dataToSendA[80] = "Mensaje: ";        //Array que se va a transmitir
char dataToSendR[32] = "Mensaje: ";
char txNum = '0';     
bool rslt;          //Vaiable para confirmacion de recepcion de mensaje

RF24 radio(CE_PIN,CSN_PIN);            //Crear objeto radio tipo RF24

const byte slaveAddress[5] = {'R','x','A','A','A'};  //Variable con la direccion del Canal para transmitir


//******* MPU objects *******
MPU6050 sensor;

MPUManager MPUInstance(sensor);
float* dataReceived;
float data [6];
String mensaje;
unsigned long time1;
unsigned long time2;
//unsigned long t2 = millis();

void setup() {
 Serial.begin(9600);
 radio.begin();
 radio.setDataRate( RF24_250KBPS );
 radio.setRetries(3,5); // delay, count
 radio.openWritingPipe(slaveAddress);        //Abrir canal para escribir

  MPUInstance.startConnection();
}

void loop() {
//******* MPU functions ********
  dataReceived = MPUInstance.getData();
  for(int i=0;i<6;i++){
    data[i] = dataReceived[i];
    }
  mensaje = "AX: " + String(data[0]) + " AY: " + String(data[1]) + " AZ: " + String(data[2]) + " RX: " + String(data[3]) + " RY: " + String(data[4]) + " RZ: " + String(data[5]);

 
  //mensaje.toCharArray(dataToSendA, mensaje1.length() + 1);  

  Serial.println(mensaje);
  // Codigo de Transmisor RF para enviar Informacion
 
    sendData();
    
    delay(1000);
  }



//****************RF Functions*******************
void sendData() {
    Serial.println("=> Sending Data...");

   // Serial.println(time1);
    rslt = radio.write(data,sizeof(data)); //Envia la data del array
 
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


