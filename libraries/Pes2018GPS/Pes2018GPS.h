
/*
Pes2018GPS.h
* Libreria para realizar los metodos del Modulo MSM
* Creada: Yilmar Mazo Vargas
* Febrero 2018 ---

*/

#ifndef Pes2018GPS_h
#define Pes2018GPS_h
#include "Arduino.h"
#include <TinyGPS.h>

class Pes2018GPS {

public:
	///<summary>Constructor de la libreria MSM</summary>
	///<param name="Telefono">solicita el puero serial el cual estar conectado al modulo MSM</param>
	Pes2018GPS(HardwareSerial *serialX);
	///<summary>
	///Metodo el utilizamos para obtener los datos de temperatura
	///</summary>
	///<param name="flat">solicita el dato "flat"</param>
	///<param name="flon">solicita el dato "flon"</param>
	///<param name="age">solicita el dato "age"</param>
	///<param name="sat">solicita el dato "sat"</param>
	///<param name="prec">solicita el dato "prec"</param>
	///<param name="chars">solicita el dato "chars"</param>
	///<param name="sentences">solicita el dato "sentences"</param>
	///<param name="failed">solicita el dato "failed"</param>
	void Pes2018GPS::loopgps(double *flat, double *flon, unsigned long *age=0, unsigned short *sat=0, unsigned long  *prec=0, unsigned long *chars=0, unsigned short *sentences=0, unsigned short *failed=0);
	void Pes2018GPS::smartdelay(unsigned long ms);
	
private:
	HardwareSerial *_portX;
	TinyGPS gps;
	bool newData = false;
	// void Pes2018GPS::smartdelay(unsigned long ms);
	/*
	variables GPS
	*/

};

#endif

