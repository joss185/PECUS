//Librerias para NRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SoftwareSerial.h>


#define CER_PIN 2
#define CSNR_PIN 3

#define CET_PIN 9
#define CSNT_PIN 10

SoftwareSerial portT(9,10);
SoftwareSerial portR(2,3);

char msgT[16]="Transmitiendo\n";        //Array que se va a transmitir
char msgR[16];
RF24 radioR(CER_PIN,CSNR_PIN);       //Crear objeto radio tipo RF24
RF24 radioT(CET_PIN,CSNT_PIN);
byte direccion[5] = {'c','a','n','a','l'};  //Vriable con la direccion del Canal para transmitir


void setup() {
  // put your setup code here, to run once:
 //Serial.begin(9600);
 radioT.begin();
 radioR.begin();
 radioT.openWritingPipe(direccion);        //Transmitir Abrir para escribir
 radioR.openReadingPipe(1,direccion);      //Recibir
 
 portT.begin(9600);
 portR.begin(9600);

}

void loop() {
  // Codigo de Transmisor
  bool ok = radioT.write(msgT,16);
  /*if(ok){
    portT.print("Datos enviados: ");
    for(int i = 0; i <= sizeof(msg); i++){
      portT.print(msg[i]);
      }
    
    }else{
      portT.println("No se ha podido enviar");
    }
*/
  //Codigo Receptor
  if (radioR.available()){
    int done = radioR.read(msgR,16);
    portR.println(done);
    
    }
  
  }

