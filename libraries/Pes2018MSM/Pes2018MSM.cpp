/*
Pes2018MSM.h
* Libreria para realizar los metodos del Modulo MSM
* Creada: Yilmar Mazo Vargas
* Febrero 2018 ---

*/
#include "Arduino.h"
#include "Pes2018MSM.h"

Pes2018MSM::Pes2018MSM(HardwareSerial *serialX)
{
	_portX = serialX;
	_portX->begin(9600);
}
void Pes2018MSM::iniciarGSM()
{
	//código
	enviarAT("AT+CPIN=\"8528\"", "OK", 1000);//se envia el numero de PIN en caso que este lo tenga activado
	Serial.println("Conectando a la red...");
	delay(5000);
	//espera hasta estar conectado a la red movil
	while (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 0)
	{
	}
	Serial.println("Conectado a la red.");	
	enviarAT("AT+CLIP=1\r", "OK", 1000); // Activamos la identificacion de llamadas
	enviarAT("AT+CMGF=1\r", "OK", 1000); //Configura el modo texto para enviar o recibir mensajes
	enviarAT("AT+CNMI=2,2,0,0,0\r", "OK", 1000); //Configuramos el modulo para que nos muestre los SMS recibidos por comunicacion serie
	/*enviarAT("AT+CCLK?", "OK", 1000);
	enviarAT("AT+COPS=0", "OK", 1000);
	enviarAT("AT+CTZU=1", "OK", 1000);
	enviarAT("AT+CLTS =1", "OK", 1000);
	enviarAT("AT+CCLK?", "OK", 1000);*/
}
int Pes2018MSM::enviarAT(char* ATcommand, char* resp_correcta, unsigned int tiempo)
{
	int x = 0;
	bool correcto = 0;
	char respuesta[100];
	unsigned long anterior;
	String mensaje;

	memset(respuesta, '\0', 100); // Inicializa el string
	
	delay(100);
	while (_portX->available() > 0) _portX->read(); // Limpia el buffer de entrada
	_portX->println(ATcommand); // Envia el comando AT
	x = 0;
	anterior = millis();
	// Espera una respuesta
	do {
		// si hay datos el buffer de entrada del UART lee y comprueba la respuesta
		if (_portX->available() != 0)
		{
			//Comprueba que no haya desbordamiento en la capacidad del buffer
			if (x < 99) {
				respuesta[x] = _portX->read();
				x++;
			}
			else Serial.println("Desbordamiento!");
			// Comprueba si la respuesta del modulo es la 1
			if (strstr(respuesta, resp_correcta) != NULL)
			{
				correcto = 1;
			}
		}
	}
	// Espera hasta tener una respuesta
	while ((correcto == 0) && ((millis() - anterior) < tiempo));
	Serial.println(respuesta);

	
	//Serial.println(ATcommand);
	//Serial.println(mensaje.indexOf("Encender LED"));
	//int led_on = mensaje.indexOf("Encender LED");
	//int led_off = mensaje.indexOf("Apagar LED");
	//if (led_on >= 0)
	//{
	//	digitalWrite(13, HIGH);
	//	Serial.println("\nLED ENCENDIDO");
	//	mensaje = ""; //Bórralo para la próxima vez
	//}
	//if (led_off >= 0)
	//{
	//	digitalWrite(13, LOW);
	//	Serial.println("\nLED APAGADO");
	//	mensaje = ""; //Bórralo para la próxima vez
	//}


	return correcto;
}
void Pes2018MSM::mensaje_sms(char* Telefono, char* strMSM)
{
	char _sms[100] = "PES2018! \x1A \r\n";
	//for (uint8_t i = 0; i < sizeof(strMSM); i++) Serial.print(strMSM[i]);
	char _aux_str[100];
	//Serial.print("strMSM "); Serial.print(strMSM); Serial.print("strMSM ");
	if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //comprueba la conexion a la red
	{
		Serial.println("Enviando SMS...");
		enviarAT("AT+CMGF=1\r", "OK", 1000); //Comando AT para mandar un SMS
		sprintf(_aux_str, Telefono,strlen(strMSM)); //Numero al que vamos a enviar el mensaje
																   //Texto del mensaje
		if (enviarAT(_aux_str, ">", 10000) == 1)
		{
			enviarAT(strMSM, "OK", 10000);
		}
		Serial.println("SMS enviado");
	}
}
void Pes2018MSM::recarga()
{
	char _sms[100] = "PES2018! \x1A \r\n";

	char _aux_str[50];
	if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //comprueba la conexion a la red
	{
		Serial.println("Enviando SMS...");
		enviarAT("AT+CMGF=1\r", "OK", 1000); //Comando AT para mandar un SMS
		sprintf(_aux_str, "AT+CMGS=\"606\"", strlen(_sms)); //Numero al que vamos a enviar el mensaje
														  //Texto del mensaje
		if (enviarAT(_aux_str, ">", 10000) == 1)
		{
			enviarAT("HORAWEB", "OK", 10000);
		}
		Serial.println("SMS enviado");
	}

} 
void Pes2018MSM::llamar(char* Telefono)
{
	if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //Comprueba la conexion a la red
	{
		Serial.println("Realizando llamada...");
		enviarAT(Telefono, "OK", 1000);
		delay(30000); // Espera 30 segundos mientras realiza la llamada
		enviarAT("ATH", "OK", 1000); // Cuelga la llamada
		Serial.println("Llamada finalizada");
	}

}
int Pes2018MSM::validarTemp()
{
	int respuesta = 0;
	char clave_TEM[] = { 'T','E','M' };//AQUI CAMBIAMOS TODO EL MENSAJE DE CONTROL
	if (datosSERIAL[0] == clave_TEM[0] && datosSERIAL[1] == clave_TEM[1] && datosSERIAL[2] == clave_TEM[2])
	{
		Serial.println("Buscando temperatura");
		respuesta = 1;
	}
	
	return respuesta;

}
int Pes2018MSM::validarGPS()
{
	char clave_GPS[] = { 'G','P','S' };//AQUI CAMBIAMOS TODO EL MENSAJE DE CONTROL
	int respuesta = 0;
	if (datosSERIAL[0] == clave_GPS[0] && datosSERIAL[1] == clave_GPS[1] && datosSERIAL[2] == clave_GPS[2])
	{
		Serial.println("Buscando temperatura");
		respuesta = 2;
	}

	return respuesta;
}
int Pes2018MSM::recibirSMS() {
	//String texto;
	//while (_portX->available()>0)
	//	{
	//		//Serial.write(_portX->read());
	//		texto = (String)_portX->read();
	//	}
	//Serial.print(texto);
salir:
	 int respuesta;
	int i = 0;
	int j = 0;
	char DAT;
	char DAT_dos;
	boolean condicion = true;
	
	
	if (_portX->available()>0)
	{

		char DAT = _portX->read();
		//Serial.print(DAT);//imprime el mensaje antes del @
		if (DAT == '@')//el arroba hace detectar el inicio del codigo
		{//If arroba						
			while (true)//ingresa en un while para leer solo los codigos futuros que estan por llegar despues de la arroba
			{
				if (_portX->available()>0)
				{//cierre del segundo if
					char DAT_dos = _portX->read();//
					datosSERIAL[j] = DAT_dos;//almacena en cadena de caracteres, suma de caracteres     
					j++;
					//////////////////////////
					if (DAT_dos == '\n')//cuando termine de entregar todos los datos dara un enter
										//garantizando el final del codigo
					{

						Serial.println("Lectura:");//IMPRIME LOS CARACTERES ALMACENADOS PARA VER SI TODO ESTA OK
						for (int i = 0; i <= j; i++)
						{
							Serial.print(datosSERIAL[i]);//IMPRIME TODO EL CODIGO GUARDADO EN EL ARRAY
						}
						delay(500);
						respuesta=validarTemp();//validamos la temperatura y guadamos los datos 
						Serial.println(respuesta);
						//respuesta = validarGPS();//validamos la temperatura y guadamos los datos 
						Serial.println(respuesta);
						for (int i = 0; i <= j; i++)
						{
							datosSERIAL[i] == 0;//borro array
							DAT_dos = 0;
							DAT = 0;
						}
						Serial.println(respuesta);
						return respuesta;
						j = 0;//borra el puntero o acumulador si no se hace esto no detecta los sigueintes codigos
						
						goto salir;//sale de todos los ciclos y va al inicio para volver a leer codigo
						
					}//CIERRA AL /N
					 ////////////////////////     
				} //cierre del segundo if
			
			}//while
		
			
		}//arroba
		
	}//serial available 
	//return respuesta;
	
}
void Pes2018MSM::limpiarBuffer()
{
	while (_portX->read() >= 0)
		; // no hacemos nada :-)
}