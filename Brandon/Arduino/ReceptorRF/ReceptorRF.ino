//Librerias para NRF24L01
#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN 9
#define CSN_PIN 10

const byte slaveAddress[5] = {'R','x','A','A','A'};   //Canal a usar

RF24 radio(CE_PIN,CSN_PIN);            //Crear objeto radio tipo RF24

char dataReceived[160];        //Array que se va a recibir
float data [6];
bool newData = false;


String mensaje;

void setup() {
  //**********RF*************
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
        //radio.read( &dataReceived, sizeof(dataReceived) ); //Lee la data y la guarda en el array
        radio.read(data, sizeof(data));
        newData = true; // Setear variable de recibido de la informacion
    }else{
      Serial.println("No data available"); 
    }
}

void showData() {
    if (newData == true) { //Si la informacion se recibio
        Serial.print("Data received: ");
        mensaje = "Aceleración en X: " + String(data[0]) + "   Aceleración en Y: " + String(data[1]) + "   Aceleración en Z: " + String(data[2]) + "   Rotacion en X: " + String(data[3]) + "   Rotacion en Y: " + String(data[4]) + "   Rotacion en Z: " + String(data[5]);
        //for (uint8_t i = 0; i < sizeof(dataReceived); i++) Serial.print(dataReceived[i]); //display character array
        Serial.println(mensaje);
        //Serial.println(dataReceived); //Imprime lo que se encuentra en el array
        newData = false; //Despues de mostrar la informacion cambiamos la variable de recepcion de datos a falso
    }
}
