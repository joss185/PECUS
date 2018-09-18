#include "Pes2018GPS.h"


TinyGPS TGPS;

Pes2018GPS GPS = Pes2018GPS(&Serial3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  Serial.println("Iniciando...");
  
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
  }

}
void loop() {
  // put your main code here, to run repeatedly:

       GPS.loopgps();

}




