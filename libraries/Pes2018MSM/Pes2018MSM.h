
/*
Pes2018MSM.h
* Libreria para realizar los metodos del Modulo MSM
* Creada: Yilmar Mazo Vargas
* Febrero 2018 ---

*/

#ifndef Pes2018MSM_h
#define Pes2018MSM_h
#include "Arduino.h"

class Pes2018MSM {

public:
	///<summary>Constructor de la libreria MSM</summary>
	///<param name="Telefono">solicita el puero serial el cual estar conectado al modulo MSM</param>
	Pes2018MSM(HardwareSerial *serialX);
	///<summary>Ninicia el modulo GSM</summary>	
	void Pes2018MSM::iniciarGSM();
	///<summary>mensaje_sms</summary>
	///<param name="Telefono">solicita el Numero de Telefono</param>
	///<param name="strMSM">solicita mensaje que se desea enviar </param>
	void Pes2018MSM::mensaje_sms(char* Telefono, char* strMSM);
	///<summary>mensaje_sms</summary>
	///<param name="Telefono">solicita el Numero de Telefono</param>
	///<param name="strMSM">solicita mensaje que se desea enviar </param>
	void Pes2018MSM::recarga();
	///<summary>mensaje_sms</summary>
	///<param name="Telefono">solicita el Numero de Telefono</param>
	void Pes2018MSM::llamar(char* Telefono);
	///<summary>Valida si recibe un mensaje</summary>
	int Pes2018MSM::recibirSMS();
	///<summary>mensaje_sms</summary>
	///<param name="ATcommand">solicita el Numero de Telefono</param>
	///<param name="resp_correcta">solicita mensaje que se desea enviar </param>
	///<param name="tiempo">solicita mensaje que se desea enviar </param>
	int Pes2018MSM::enviarAT(char* ATcommand, char* resp_correcta, unsigned int tiempo);

private:
	HardwareSerial *_portX;
	char datosSERIAL[10];
	
	void Pes2018MSM::limpiarBuffer();
	int Pes2018MSM::validarTemp();
	int Pes2018MSM::validarGPS();
	
};

#endif

