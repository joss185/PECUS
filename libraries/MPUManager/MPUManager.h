/*
  MPUManager.h - Library controlling MPU Data for the bovine necklace
  Header file (it defines the class structure)
*/
#ifndef MPUManager_h
#define MPUManager_h

#include "String.h"
#include "Arduino.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"


class MPUManager {
  public:
    MPUManager(MPU6050& sensor);
    
    void startConnection();
    float* getData();
    void showData();
    
  private:
    // Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
    int ax, ay, az;
    int gx, gy, gz;

    long tiempo_prev, dt;
    float girosc_ang_x, girosc_ang_y, girosc_ang_z;
    float girosc_ang_x_prev, girosc_ang_y_prev,girosc_ang_z_prev;

    float ax_m_s2;
    float ay_m_s2;
    float az_m_s2;

    float gyro [3];
    float accel [6];
    //int i = 0;
   
    MPU6050* _mpuA;

};
#endif
