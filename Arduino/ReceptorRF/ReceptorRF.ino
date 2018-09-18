//Librerias para NRF24L01
#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 31
#define CSN_PIN 30

const byte slaveAddress[5] = {'R','x','A','A','A'};   //Canal a usar

RF24 radio(CE_PIN,CSN_PIN);            //Crear objeto radio tipo RF24

char dataReceived[11];        //Array que se va a recibir

bool newData = false;

void setup() {
  
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1,slaveAddress);
  radio.startListening();
}

void loop() {
  // Codigo de Receptor RF

  getData(); //Obtiene la data del transmisor
  
  showData(); //Muestra data recibida

  delay(1000);
}

void getData() { 
    if ( radio.available() ) { //Si el canal esta disponible
        Serial.println("=> Radio receiving..."); 
        radio.read( &dataReceived, sizeof(dataReceived) ); //Lee la data y la guarda en el array
        newData = true; // Setear variable de recibido de la informacion
    }
}

void showData() {
    if (newData == true) { //Si la informacion se recibio
        Serial.print("Data received: ");
        Serial.println(dataReceived); //Imprime lo que se encuentra en el array
        newData = false; //Despues de mostrar la informacion cambiamos la variable de recepcion de datos a falso
    }
}
