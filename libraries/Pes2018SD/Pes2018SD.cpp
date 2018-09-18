/*
Pes2018SD.h
* Libreria para realizar los metodos del Modulo SD
* Creada: Yilmar Mazo Vargas
* Febrero 2018 ---

CONEXION SEGUN LA TARJETA 

	Modelo Arduino		MOSI			MISO			SDK			SS SLAVE	SS		MASTER
		UNO				11,ICSP-4	12,ICSP-1	13,ICSP-3		10			Z
		MEGA				51,ICSP-4	50,ICSP-1	52,ICSP-3		53			Z
		LEONARDO			ICSP-4		ICSP-1		ICSP-3			Z			Z
		DUE				ICSP-4		ICSP-1		ICSP-3			Z			4,10,52

*/
/*
	Librerias 
*/
#include "Arduino.h"
#include "Pes2018SD.h"
#include <SPI.h>//tarjeta SD
#include <SD.h>//Tarjeta SD

/*
<summary>
	Construtor 
		Modelo Arduino		SS SLAVE
			UNO				  10
			MEGA				  53
</summary>
<param name="serialTe"=solicita el puero o PI en el cual se conecto el modulo SD SS SLAVE/>
*/
Pes2018SD::Pes2018SD(HardwareSerial *serialTe)
{
	_chipSelect = serialTe;
	_chipSelect->begin(9600);

}
/*
<summary>
	inici el serial del modulo SD
</summary>
*/
void Pes2018SD::iniciarSD()
{
	//código
	SD.begin(_chipSelect); // Iniciamos el módulo SD
	while (!Serial) {
		; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
	}
	
}
/*
<summary>
	Valida la conexion del modulo y si tiene una tarjeta SD coneectada
</summary>
*/
void Pes2018SD:: validarSD() {
	Sd2Card card;
	// usaremos el código de inicialización de las bibliotecas de utilidades
	if (!card.init(SPI_HALF_SPEED, _chipSelect)) {
		Serial.println("error de inicialización. Cosas que verificar:");
		Serial.println("* ¿está insertada una tarjeta?");
		Serial.println("* ¿su cableado es correcto?");
		Serial.println("* ¿Cambió el chipSelect pin para que coincida con su escudo o módulo?");
		return;
	}
	else {
		Serial.println("El cableado es correcto y hay una tarjeta presente.");
	}

}
/*
<summary>
	Metodo el utilizamos para guardar los datos de temperatura
</summary>
<param name="readTempC"=solicita la temperatura en Celsius />
<param name="readTempF"=solicita la temperatura en fahrenheit/>
<param name="tipo"=si el "tipo=1" guarda la temperatura del ambiente, si el "tipo =2" guarda la temperatura del objeto/>
<param name="strNombre"=solicita el nobre del archivo/>
*/
void Pes2018SD::GuardarSDTem(float  readTempC, float  readTempF, int tipo, String strNombre) {
		if (tipo==1) {
		File dataFile = SD.open(strNombre, FILE_WRITE); //Crea el archivo DHT11 o si ya existe escribe dentro de el (Podeis cambiarlo)
		if (dataFile)
		{
			Serial.println("Guardando Temperatura Ambiente...");
			dataFile.print("La temperatura es de: ");   dataFile.print(readTempC); dataFile.println("*C");
			dataFile.print("La temperatura es de: ");   dataFile.print(readTempF); dataFile.println("*F");
			Serial.print("La temperatura es de: ");  Serial.print(readTempC); Serial.println("*C");
			Serial.print("La temperatura es de: ");  Serial.print(readTempF); Serial.println("*F");
			dataFile.close();
		}
		else
		{
			Serial.print("Error al guardar...");
		}
	}
	else
	{
		File dataFile = SD.open(strNombre, FILE_WRITE); //Crea el archivo DHT11 o si ya existe escribe dentro de el (Podeis cambiarlo)
		if (dataFile)
		{
			Serial.println("guardando Temperatura Objeto...");
			dataFile.print("La temperatura es de: ");  dataFile.print(readTempC); dataFile.println("*C");
			dataFile.print("La temperatura es de: "); dataFile.print(readTempF); dataFile.println("*F");
			Serial.print("La temperatura es de: ");   Serial.print(readTempC); Serial.println("*C");
			Serial.print("La temperatura es de: ");  Serial.print(readTempF); Serial.println("*F");
			dataFile.close();
		}
		else
		{
			Serial.print("Error al guardar...");
		}
	}
}
/*
<summary>
Metodo el utilizamos para guardar los datos de temperatura
</summary>
<param name="strGPS"=solicita la cadena a guardar/>
<param name="strNombre"=solicita el nobre del archivo/>
*/
void Pes2018SD::GuardarSDGPS(String strGPS, String strNombre){
		File dataFile = SD.open(strNombre, FILE_WRITE); //Crea el archivo GPS o si ya existe escribe dentro de el (Podeis cambiarlo)
		if (dataFile)
		{
			Serial.println("Guardando GPS...");
			dataFile.println(strGPS);
			Serial.print(strGPS);
			dataFile.close();	
		}
		else
		{
			Serial.print("Error al guardar...");
		}
	}

void Pes2018SD::GuardarHR(String srtRateValue, String strNombre){
		File dataFile = SD.open(strNombre, FILE_WRITE); //Crea el archivo HearRate o si ya existe escribe dentro de el (Podeis cambiarlo)
		if (dataFile)
		{
			Serial.println("Guardando HearRate...");
			dataFile.println(srtRateValue);
			Serial.print(srtRateValue);
			dataFile.close();	
		}
		else
		{
			Serial.print("Error al guardar HearRate...");
		}
	}
void Pes2018SD::GuardarMPU(String mensaje, String strNombre){
		File dataFile = SD.open(strNombre, FILE_WRITE); //Crea el archivo HearRate o si ya existe escribe dentro de el (Podeis cambiarlo)
		if (dataFile)
		{
			Serial.println("Guardando HearRate...");
			dataFile.println(mensaje);
			Serial.print(mensaje);
			dataFile.close();	
		}
		else
		{
			Serial.print("Error al guardar HearRate...");
		}		
}
