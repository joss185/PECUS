
#include <SD.h>
#include <SPI.h>  
#include "RF24.h"

const int pinCEt = 9;
const int pinCSt = 10;

const int ss = 53;

File myFile;
int pinCS = 53;

RF24 myRadio (pinCEt, pinCSt);
byte slaveAddress[5] = {'R','x','A','A','A'};
float datos[3];

void setup()
{
  pinMode(ss, OUTPUT);
  digitalWrite(ss, HIGH);

  pinMode(pinCEt, OUTPUT);
  pinMode(pinCSt, OUTPUT);
  pinMode(pinCS, OUTPUT);


  SD.begin();
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe(slaveAddress); 
  delay(1000);

  //digitalWrite(pinCS, HIGH);
  //digitalWrite(pinCSt, HIGH);
}

void loop()
{
  datos[0]=analogRead(0)* (5.0 / 1023.0);;
  datos[1]=millis();
  datos[2]=3.14;
  digitalWrite(pinCSt, LOW);
  myRadio.write(&datos, sizeof(datos)); 
     Serial.print("datos enviados");
     Serial.print(datos[0]); 
     Serial.print(" , "); 
     Serial.print(datos[1]); 
     Serial.print(" , "); 
     Serial.println(datos[2]); 
  digitalWrite(pinCSt, HIGH);
  delay(2000);
  digitalWrite(pinCS, LOW);
  save();
  digitalWrite(pinCS, HIGH);
  delay(1000);
}

void save() {
  myFile = SD.open("testing.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
    Serial.print("datos guardados");
     Serial.print(datos[0]); 
     Serial.print(" , "); 
     Serial.print(datos[1]); 
     Serial.print(" , "); 
     Serial.println(datos[2]); 
    myFile.close(); // close the file
    Serial.println("Done.");
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening testing.txt");
  }
}
