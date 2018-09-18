    
/*
  MPUManager.cpp - Library defining project structure
  File with actual algorithms and functions
*/
#include "Arduino.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "MPUManager.h"
#include "Wire.h"


MPUManager::MPUManager(MPU6050 &sensor) {
    
    _mpuA = &sensor;
    Serial.begin(9600);    //Iniciando puerto serial
    Wire.begin();           //Iniciando I2C  
}


float* MPUManager::getData(){
  
 // Leer las aceleraciones y velocidades angulares
  _mpuA->getAcceleration(&ax, &ay, &az);
  _mpuA->getRotation(&gy, &gx, &gz);

 //Calcular los angulos de rotacion:
  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
// 131 es el offset para el girosensor
  girosc_ang_x = (gx/131)*dt/1000.0 + girosc_ang_x_prev;
  girosc_ang_y = (gy/131)*dt/1000.0 + girosc_ang_y_prev;
  girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;


  girosc_ang_x_prev=girosc_ang_x;
  girosc_ang_y_prev=girosc_ang_y;
  girosc_ang_z_prev=girosc_ang_z;
   //calcular datos de
  ax_m_s2 = ax * (9.81/16384.0); //9.81 de gravedad | 16384 offset de aceleracion para MPU 
  ay_m_s2 = ay * (9.81/16384.0);
  az_m_s2 = az * (9.81/16384.0); 

accel[0] = ax_m_s2;
accel[1] = ay_m_s2;
accel[2] = az_m_s2;
accel[3] = girosc_ang_x;
accel[4] = girosc_ang_y;
accel[5] = girosc_ang_z;

return accel;
 
}


void MPUManager::showData(){
   //Mostrar los aceleracion separadas por un [tab]
  Serial.print("Aceleración en X:  ");
  Serial.print(ax_m_s2);
  Serial.print("    Aceleración en Y:  ");
  Serial.print(ay_m_s2);
  Serial.print("    Aceleración en Z:  ");
  Serial.print(az_m_s2);
  //Mostrar los angulos separadas por un [tab]
  Serial.print("    Rotacion en X:  ");
  Serial.print(girosc_ang_x); 
  Serial.print("    Rotacion en Y: ");
  Serial.print(girosc_ang_y);
  Serial.print("    Rotacion en Z: ");
  Serial.println(girosc_ang_z);
  delay(100);
}

void MPUManager::startConnection(){ 

  _mpuA->initialize();    //Iniciando el sensor

  if (_mpuA->testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  tiempo_prev=millis();
}
