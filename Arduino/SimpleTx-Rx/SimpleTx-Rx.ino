// SimpleTx

#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>



#define CET_PIN 22
#define CSNT_PIN 23
#define CER_PIN 30
#define CSNR_PIN 31

const byte slaveAddress[5] = {'R','x','A','A','A'};


RF24 radioT(CET_PIN, CSNT_PIN); // Crear a Radio
RF24 radioR(CER_PIN, CSNR_PIN);


char dataToSend[11] = "Mensaje: "; //Array que se envia
char txNum = '0';

char dataReceived[11]; //Array que se recibe
bool newData = false;
bool rslt;

//unsigned long currentMillis;
//unsigned long prevMillis;
//unsigned long txIntervalMillis = 1000; // envio una por segundo


void setup() {
//Tx
    Serial.begin(9600);

    //Serial.println("SimpleTx Starting");

    radioT.begin();
    radioT.setDataRate( RF24_250KBPS );
    radioT.setRetries(3,5); // delay, count
    radioT.openWritingPipe(slaveAddress);

//Rx
 
    radioR.begin();
    radioR.setDataRate( RF24_250KBPS );
    radioR.openReadingPipe(1, slaveAddress);
    radioR.startListening();
}

//====================

void loop() {
   //currentMillis = millis();
    getData();

    
   // if (currentMillis - prevMillis >= txIntervalMillis) {
      Serial.println("SimpleTx Starting");
        sendData();
    //    prevMillis = millis();
   // }
    
    
    showData();

    delay(1000);
}

//====================

void sendData() {
    Serial.println("=> Sending Data...");
    
    rslt = radioT.write(&dataToSend,sizeof(dataToSend));
    //Serial.println("=> After Write");
    //Serial.println("Fine");
    Serial.print("=> Data Sent ");
    Serial.print(dataToSend);
    if (rslt) {
        Serial.println("  Ack received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {
        // Para ver si se esta enviando nueva informacion
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}

///////////////////////////////////////////////////////////

// SimpleRx

void getData() {
    if ( radioR.available() ) {
        Serial.println("=> Radio R receiving...");
        radioR.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        Serial.println("SimpleRx Starting");
        Serial.print("Data received ");
        Serial.println(dataReceived);
        newData = false;
    }
}
