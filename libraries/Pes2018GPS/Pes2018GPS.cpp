/*
Pes2018GPS.h
* Libreria para realizar los metodos del Modulo MSM
* Creada: Yilmar Mazo Vargas
* Febrero 2018 ---

*/
#include "Arduino.h"
#include "Pes2018GPS.h"
// include the GPS library:
#include "TinyGPS.h"

Pes2018GPS::Pes2018GPS(HardwareSerial *serialX)
{
	_portX = serialX;
	_portX->begin(9600);
	
}
void Pes2018GPS::loopgps(double *flat, double *flon,unsigned long *age, unsigned short *sat, unsigned long  *prec,
	unsigned long *chars, unsigned short *sentences, unsigned short *failed) {
	//TinyGPS gps;
	bool newData = false;
	for (unsigned long start = millis(); millis() - start < 1000;)
	{
		while (_portX->available())
		{
			char c = _portX->read();
			// Serial.write(c); // uncomment this line if you want to see the GPS data flowing
			if (gps.encode(c)) // Did a new valid sentence come in?
				newData = true;
		}
	}
	if (newData)
	{
		
		long flat1, flon1;
		unsigned long age1;
		gps.get_position(&flat1, &flon1, &age1);
		//Serial.print(flat1/100000,6);
		flat1 == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat1, 6;
		//Serial.print("flat ");
		*flat = flat1;
		//Serial.print(*flat);
		flon1 == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon1, 6;
		//Serial.print("flon ");
		*flon = flon1;
		//Serial.print(*flon);
		*sat = gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites();
		*prec = gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop();


	}
	gps.stats(chars, sentences, failed);
	if (chars == 0)
	Serial.println("** No characters received from GPS: check wiring **");
	
}
 void Pes2018GPS::smartdelay(unsigned long ms)
{
	TinyGPS gps;
	unsigned long start = millis();
	do
	{
		while (_portX->available())
		{
			char c = _portX->read();
			// Serial.write(c); // uncomment this line if you want to see the GPS data flowing
			if (gps.encode(c)) {// Did a new valid sentence come in?
				newData = true;
			}
		}
	} while (millis() - start < ms);
}

