#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial serial_conn(15,16); //RX 15, TX 16
TinyGPSPlus gps; //Objeto gps
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial_conn.begin(9600);
  Serial.println("GPS Start");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(serial_conn.available()){
    gps.encode(serial_conn.read());
  }
  
  if(gps.location.isUpdated()){
   Serial.println("DEBUG");
     Serial.println("Cantidad de Satelites:");
     Serial.println(gps.satellites.value());
     Serial.println("Latitud:");
     Serial.println(gps.location.lat(),6);
     Serial.println("Longitud:");
     Serial.println(gps.location.lng(),6);
     Serial.println("Velocidad MPH");
     Serial.println(gps.speed.mph());
     Serial.println("Altitud:");
     Serial.println(gps.altitude.kilometers());
     Serial.println("");
    }
}
