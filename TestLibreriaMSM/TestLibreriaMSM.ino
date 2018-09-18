#include <Pes2018MSM.h>
#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <Pes2018GPS.h>
// include the sensor library
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
String stringVal = "";
char charVal[100];
char charValTA[100];
char charValTO[100];
Pes2018MSM pes = Pes2018MSM(&Serial1);
Pes2018GPS GP = Pes2018GPS(&Serial3);
void setup() {

  // put your setup code here, to run once:
  //Serial1.begin(9600); //Configura velocidad del puerto serie para el Serial1 Modulo GSM
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  delay(1000);
  Serial.println("Iniciando...");
  mlx.begin();
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
  }
  Menu();
  pes.iniciarGSM();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (Serial.read())
  {
    case '1':
      pes.llamar("ATD +50687299857;");
     // Serial.println(); Serial.println();
      //Menu();
      break;
    case '2':
      double flat, flon;
      unsigned long chars, prec, age;
      unsigned short sentences, failed, sat;
      //Serial.println();
      GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
      stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed + "\x1A \r\n"; //combining both whole and decimal part in string with a fullstop between them      
      StringConcatenar(stringVal, "GPS: ");
      delay(1000);
      stringVal = "La temperatura del Ambiente es de: " + String(mlx.readAmbientTempC()) + " *C -- " + String(mlx.readAmbientTempF())  + " *F" + "\x1A \r\n";
      StringConcatenar(stringVal, "Sensor: ");
      delay(1000);
      stringVal = "La temperatura del Objeto es de: " + String(mlx.readObjectTempC()) + " *C -- " + String(mlx.readObjectTempF())  + " *F" + "\x1A \r\n";
      StringConcatenar(stringVal, "Sensor: ");
      Menu();
      break;
    case '3':


      //      pes.recarga();
     // Serial.println(); Serial.println();
      Menu();
      break;
  }
   double flat, flon;
      unsigned long chars, prec, age;
      unsigned short sentences, failed, sat;
      //Serial.println();
      GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
      stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed + "\x1A \r\n"; //combining both whole and decimal part in string with a fullstop between them      
      StringConcatenar(stringVal, "GPS: ");
      delay(1000);
      stringVal = "La temperatura del Ambiente es de: " + String(mlx.readAmbientTempC()) + " *C -- " + String(mlx.readAmbientTempF())  + " *F" + "\x1A \r\n";
      StringConcatenar(stringVal, "Sensor: ");
      delay(1000);
      stringVal = "La temperatura del Objeto es de: " + String(mlx.readObjectTempC()) + " *C -- " + String(mlx.readObjectTempF())  + " *F" + "\x1A \r\n";
      StringConcatenar(stringVal, "Sensor: ");
     // Menu();


  // if (Serial1.available()) {
  // switch (pes.recibirSMS())
  //{

  //case 1:
  //Serial.println("hola"); Serial.println();
  //break;
  //case 2:
  // Serial.println("2"); Serial.println();
  //break;
  //}
  //}
  
}
void Menu() {
  Serial.println("Proyecto PES-2018");
  Serial.println("Menu Modulo GSM");
  Serial.println("Digite el numero segun la opcion que desea implementar:");
  Serial.println("1. Llamadar ");
  Serial.println("2. Enviar Mensaje");
  Serial.println("3. Activar Internet por una hora ");
  Serial.println("Esperando opcion.... ");

}
void StringConcatenar(String val, String come)
{
  char sms [val.length() + 1];                    //initialise character array to store the values
  stringVal.toCharArray(sms, val.length() + 1);  //passing the value of the string to the character array
  //Serial.print(come);
  for (uint8_t i = 0; i < sizeof(sms); i++) Serial.print(sms[i]); //display character array
 // pes.mensaje_sms("AT+CMGS=\"+50670065506\"", sms);  bnnhyujbb   b hyub
  // nju n niiinjm npes.mensaje_sms("AT+CMGS=\"+50671421096\"", sms);
   pes.mensaje_sms("AT+CMGS=\"+50683393953\"", sms);
  *sms=0;
  //Serial.println(); Serial.println();
}
