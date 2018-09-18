#define heartratePin A1
#include "DFRobot_Heartrate.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "MPUManager.h"
#include <Wire.h>
#include <TinyGPS.h>
#include <Adafruit_MLX90614.h>
#include <Pes2018GPS.h>
#include <Pes2018SD.h>
#include <string.h>


  String stringVal = "";
  const int chipSelect = 53;
  float* dataReceived;
  float data [6];
  String mensaje;
  uint8_t rateValue;
  String srtRateValue;
  unsigned long previousMillis = 0;
  const long interval = 1000;
  double flat, flon;
  unsigned long chars, prec, age;
  unsigned short sentences, failed, sat;

DFRobot_Heartrate heartrate(DIGITAL_MODE); ///< ANALOG_MODE or DIGITAL_MODE
TinyGPS gps;
Pes2018GPS GP = Pes2018GPS(&Serial3);
Pes2018SD SDC = Pes2018SD(chipSelect);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MPU6050 sensor;
MPUManager MPUInstance(sensor);


void setup() {
  Serial.begin(9600);
  mlx.begin();
  MPUInstance.startConnection();
  heartrate.begin();//Configura velocidad del MLX90614 Temp Sensor para el Modulo Temp Sensor
  SDC.iniciarSD(); // Iniciamos el módulo SD
  delay(1000);
  Serial.print("\Inicializando SD card...");
  SDC.validarSD();
  delay(1000);
}

void loop() {
  //******* HeartRate functions ********
  heartrate.getValue(heartratePin); ///< A1 foot sampled values
  rateValue = heartrate.getRate(); ///< Get heart rate value
  int num = atoi((const char*) rateValue);
  srtRateValue = "Heart Rate: " + String(rateValue);
  getHeartRate();
  //******* MPU functions ********

  getMPU();
  //funcion timer simula un hilo
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
    getGPS();
  }


}//end of loop

void getHeartRate() {



  if (rateValue)  {
    Serial.println(srtRateValue);
  }
 // delay(20);



}

void getGPS() {

  heartrate.getValue(heartratePin); ///< A1 foot sampled values
  rateValue = heartrate.getRate(); ///< Get heart rate value
  int num = atoi((const char*) rateValue);
  srtRateValue = "Heart Rate: " + String(rateValue);

  dataReceived = MPUInstance.getData();
  for (int i = 0; i < 6; i++) {
    data[i] = dataReceived[i];
  }
  mensaje = "AX: " + String(data[0]) + " AY: " + String(data[1]) + " AZ: " + String(data[2]) + " RX: " + String(data[3]) + " RY: " + String(data[4]) + " RZ: " + String(data[5]);
  Serial.println(mensaje);
  
  GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
  stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed ; //combining both whole and decimal part in string with a fullstop between them
  // StringConcatenar(stringVal, "char:   ");
  SDC.GuardarSDGPS(stringVal, "GPS1.txt");
  SDC.GuardarSDTem(mlx.readAmbientTempC(), mlx.readAmbientTempF(), 1, "TempA1.txt");
  SDC.GuardarSDTem(mlx.readObjectTempC(), mlx.readObjectTempF(), 2, "TempO1.txt");
  SDC.GuardarHR(srtRateValue, "HR1.txt");
  SDC.GuardarMPU(mensaje, "MPU1.txt");
  stringVal = "";
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
  // delay(30000);
}

void getMPU(){

  dataReceived = MPUInstance.getData();
  for (int i = 0; i < 6; i++) {
    data[i] = dataReceived[i];
  }
  mensaje = "AX: " + String(data[0]) + " AY: " + String(data[1]) + " AZ: " + String(data[2]) + " RX: " + String(data[3]) + " RY: " + String(data[4]) + " RZ: " + String(data[5]);
  Serial.println(mensaje);
  
}


