//Librerias para NRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 22
#define CSN_PIN 23

char dataToSend[11] = "Mensaje: ";        //Array que se va a transmitir
char txNum = '0';     
bool rslt;          //Vaiable para confirmacion de recepcion de mensaje

RF24 radio(CE_PIN,CSN_PIN);            //Crear objeto radio tipo RF24

const byte slaveAddress[5] = {'R','x','A','A','A'};  //Variable con la direccion del Canal para transmitir


void setup() {
 Serial.begin(9600);
 radio.begin();
 radio.setDataRate( RF24_250KBPS );
 radio.setRetries(3,5); // delay, count
 radio.openWritingPipe(direccion);        //Abrir canal para escribir


}

void loop() {
  // Codigo de Transmisor RF
    sendData(); //Enviar Informacion
    
  }

void sendData() {
    Serial.println("=> Sending Data...");
    
    rslt = radioT.write(&dataToSend,sizeof(dataToSend)); //Envia la data del array
    
    Serial.print("=> Data Sent");
    Serial.print(dataToSend);
    if (rslt) { //Si la informacion se envio y fue recibida
        Serial.println("  Ack received.");
        updateMessage(); //Actualiza el mensaje con contador txNum
    }
    else {
        Serial.println("  Tx failed!");
    }
}


void updateMessage() {
        // Verificar si se esta enviando nueva informacion
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}

