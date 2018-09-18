// include the GPS library
#include <TinyGPS.h>
// include the PES2018 library
#include <Pes2018GPS.h>
/*
   Variables GPS:
*/
TinyGPS gps;
String stringVal = "";//Variable con la cual puedes concatenar todo el datos para guardar en la SD
/*
 *       Modulo GPS conectada al bus SPI de la siguiente manera:
         ** RXD-TX3 pin14
         ** TXD-RX3 pin15
 */
Pes2018GPS GP = Pes2018GPS(&Serial3);
void setup() {
  // put your setup code here, to run once:
 
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  Serial.println("Iniciando...");
 // pes.iniciarGSM();
  delay(500);
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
   double flat, flon;
  unsigned long chars, prec, age;
  unsigned short sentences, failed, sat;
  //habilita los comandos de consola si ingresas el 1 se ejecutara el caso 1
if (Serial.available())
    switch (Serial.read())
    {
      case '1':
        Serial.print("Sttring: ");
       //este metodo llena los datos 
        GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed); //captura los datos del GPS
        stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed ; //combining both whole and decimal part in string with a fullstop between them       
        Serial.println(stringVal);
        /*este metodo es para guardar los datos en el SD algo asi ocuparias para tu modelo si vas a guardar
        //algun dato en esta tarjeta 
        */
        //SDC.GuardarSDGPS(stringVal);
        StringConcatenar(stringVal, "char:   ");
        if (chars == 0)
          Serial.println("** No characters received from GPS: check wiring **");
        break;
      case '2':
        break;
    }
}
//metodo que utilizo para enviar mensajes convierte la cadena en un char 
void StringConcatenar(String val, String come)
{
  char sms [val.length() + 1];                    
  stringVal.toCharArray(sms, val.length() + 1);  
  Serial.print(come);
  for (uint8_t i = 0; i < sizeof(sms); i++) Serial.print(sms[i]); //display character array
  //pes.mensaje_sms("AT+CMGS=\"+50670065506\"", sms);
//  sms="";
  Serial.println(); Serial.println();
}
