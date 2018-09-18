#include <RF24.h>
#include <RFManager.h>

#define CER_PIN 22
#define CSNR_PIN 23

#define CET_PIN 30
#define CSNT_PIN 31

RF24 _radio1(CER_PIN,CSNR_PIN);
RF24 _radio2(CET_PIN,CSNT_PIN);

RFManager rfManR(_radio1,Serial);
RFManager rfManT(_radio2, Serial);

void setup() {
  Serial.begin(9600);
  Serial.println("Test");
}

void loop() {
  rfManR.setListener();
  rfManR.getData();

  rfManT.setWriter();
  rfManT.sendData();

  rfManR.showData();
  
}
