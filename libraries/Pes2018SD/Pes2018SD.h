
/*
Pes2018SD.h
* Libreria para realizar los metodos del Modulo MSM
* Creada: Yilmar Mazo Vargas
* Febrero 2018 ---

*/

#ifndef Pes2018SD_h
#define Pes2018SD_h
#include "Arduino.h"

class Pes2018SD {

public:
	
	///<summary>
	///Construtor
	///	Modelo Arduino		SS SLAVE
	///		UNO				  10
	///		MEGA				  53
	///</summary>
	///<param name="serialTe">solicita el puero o PI en el cual se conecto el modulo SD SS SLAVE</param>
	Pes2018SD(HardwareSerial *serialTe);
	///<summary>
	///inici el serial del modulo SD
	///</summary>
	void Pes2018SD::iniciarSD();
	///<summary>
	///	Metodo el utilizamos para guardar los datos de temperatura
	///</summary>
	///<param name="readTempC">solicita la temperatura en Celsius </param>
	///<param name="readTempF">solicita la temperatura en fahrenheit</param>
	///<param name="tipo">si el "tipo=1" guarda la temperatura del ambiente, si el "tipo =2" guarda la temperatura del objeto</param>
	///<param name = "strNombre">solicita el nobre del archivo</param>
	///</summary>
	void Pes2018SD::GuardarSDTem(float  readTempC, float  readTempF, int tipo, String strNombre);
	///<summary>
	///Valida la conexion del modulo y si tiene una tarjeta SD coneectada
	///</summary>
	void Pes2018SD::validarSD();
	///<summary>
	///Metodo el utilizamos para guardar los datos de temperatura
	///</summary>
	///<param name="strGPS">solicita la cadena a guardar</param>
	///<param name = "strNombre">solicita el nobre del archivo</param>
	///</summary>
	void Pes2018SD::GuardarSDGPS(String strGPS, String strNombre);
		//float flat, float flon, unsigned long age, unsigned short sat, unsigned long  prec, unsigned long chars, unsigned short sentences, unsigned short failed);
	void Pes2018SD::GuardarHR(String srtRateValue, String strNombre);

	void Pes2018SD::GuardarMPU(String mensaje, String strNombre);
private:
	HardwareSerial *_chipSelect;
};

#endif


