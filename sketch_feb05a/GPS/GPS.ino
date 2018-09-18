#include <TinyGPS.h>

TinyGPS gps1;

char numero_cell[] = "+50670065506"; //meter numero de telefono
long latitud, longitud;
unsigned long idInfo;
int year;
byte month, day, hour, minutes, second, hundredths;
unsigned long fix_age;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial3.begin(9600);
  Serial.begin(9600);
  delay(1000);
  // configuracion_inicial();
}

void loop() {
  // put your main code here, to run repeatedly:
  bool recibido = false;
  //Serial.println(recibido);
  while (Serial3.available()) {
    char cIn = Serial3.read();
    //Serial.println("seial ");
    //Serial.print(cIn);
    recibido = gps1.encode(cIn);
Serial.println(TinyGPS::GPS_INVALID_AGE);
/*
    gps1.crack_datetime(&year, &month, &day, &hour, &minutes, &second, &hundredths, &fix_age);
    gps1.get_position(&latitud, &longitud, &idInfo);
    float flat, flon;
    unsigned long fix_age; // returns +- latitude/longitude in degrees
    gps1.f_get_position(&flat, &flon, &fix_age);
    if (fix_age == TinyGPS::GPS_INVALID_AGE)
      Serial.println("No fix detected");
    else if (fix_age > 5000)
      Serial.println("Warning: possible stale data!");
    else
      Serial.println("Data is current.");
    Serial.print(float(longitud) / 10000, 6);
    Serial.print(" , ");
    Serial.print(float(latitud) / 10000, 6);
    Serial.print(" , ");
    Serial.println(idInfo);
    */
    //Serial.print(recibido);
  }

  if (recibido) {
    Serial.print(recibido);
    Serial.println("------------------------------");

    //latitud
    long latitud, longitud;
    unsigned long idInfo;
    gps1.get_position(&latitud, &longitud, &idInfo);
  }
  /*
    Serial.println("esperando recibir mensaje...");
    while (true)
    {

    if (Serial.available() > 0)//validamos si se ingresa algun dato en el cuadro de texto
    {

      switch (Serial.read())
      {
        case 'a':
          envio_mensaje();
          break;
      }
    }
    }*/
}
void configuracion_inicial()//configura los codigos de lectura de mensajes
{
  Serial1.println("AT+IPR=9600");//modo texto
  Serial.println("AT+IPR=9600");//modo texto
  delay(300);
  Serial1.println("AT+CMGF=1");//modo texto
  Serial.println("AT+CMGF=1");//modo texto
  delay(300);
  Serial1.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  Serial.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  delay(300);
  Serial1.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  Serial.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  //Serial1.println("AT+CNMI=3,3,0,0");//ACTIVAMOS PARA VER MENSAJES
  //Serial.println("AT+CNMI=3,3,0,0");//ACTIVAMOS PARA VER MENSAJES
  delay(300);
  if (Serial1.available())// validamos la conexion
  {
    char DAT = Serial1.read();
    Serial.print(DAT);
    /*Este seria el mensaje que devolveria si la configuracion esta bien
       AT+IPR=9600
      OK
      AT+CMGF=1
      OK
      AT+CMGR=?
      OK
      AT+CNMI=2
    */
  }
  Serial.println("configuracion terminada");
  delay(300);

}
void envio_mensaje()
{
  Serial1.println("AT+CMGF=1");//modo texto
  //Serial.println("AT+CMGF=?");//modo texto
  //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
  delay(2000);


  /*Serial.print("AT+CMGS=");// comando de envio de mensaje
    //Imprime los datos al puerto serie como texto ASCII
    Serial.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
    Serial.print(numero_cell);//colocamos numero de telefono
    Serial.println((char)34);//volvemos a poner el caracter "
  */

  Serial1.print("AT+CMGS=");// comando de envio de mensaje
  //Imprime los datos al puerto serie como texto ASCII
  Serial1.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
  Serial1.print(numero_cell);//colocamos numero de telefono
  Serial1.println((char)34);//volvemos a poner el caracter "
  delay(200);//tiempo para que de respuesta el modulo >
  Serial1.println("Hola Mundo");//mensaje que enviare
  Serial1.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino

  Serial.print("Hola Mundo");//mensaje que enviare
}
