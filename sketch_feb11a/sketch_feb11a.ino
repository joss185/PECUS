#include <Adafruit_MLX90614.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Pes2018MSM.h>
Pes2018MSM lGSM();
void setup() {
  Serial1.begin(9600); //Configura velocidad del puerto serie para el Serial1 Modulo GSM
  Serial3.begin(9600); //Configura velocidad del puerto serie para el Serial3 Modulo GPS
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
//  mlx.begin(); //Configura velocidad del MLX90614 Temp Sensor para el Modulo Temp Sensor
  delay(1000);
  Serial.println("Iniciando...");
  lGSM.iniciarGSM();
  

}
void loop() {
  // put your main code here, to run repeatedly:

}
