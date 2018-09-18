/*
 ** Modulos  para realizar los metodos del proyecto PES2018-ULATINA BOVINE CONTROL NECKLACE PROTOTIPO
  Creada: Yilmar Mazo Vargas
  Febrero 2018 ---
*/

/*
  El circuito:
      Modulo SD conectada al bus ICSP de la siguiente manera:
 ** MOSI - ICSP
 ** MISO - ICSP
 ** CLK - ICSP
 ** CS -53

      Modulo GPS conectada al bus SPI de la siguiente manera:
 ** RXD-TX3 pin14
 ** TX3-RXD pin15
  
        Modulo MLX90614 Temp Sensor conectada al bus SPI de la siguiente manera:
  ** SCL-SCL1
  ** SDA-SDA1

*/
#include <Wire.h>
// include the GPS library:
#include <TinyGPS.h>
// include the sensor library
#include <Adafruit_MLX90614.h>
// include the PES2018 library
#include <Pes2018GPS.h>
#include <Pes2018SD.h>
/*
   variables GPS
*/
TinyGPS gps;
String stringVal = "";
Pes2018GPS GP = Pes2018GPS(&Serial3);
/*
   Variables SD Card:
*/
const int chipSelect = 53;
Pes2018SD SDC = Pes2018SD(chipSelect);
/*
   variables MLX90614 Temp Sensor
*/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  Serial3.begin(9600);
  mlx.begin(); //Configura velocidad del MLX90614 Temp Sensor para el Modulo Temp Sensor
  SDC.iniciarSD(); // Iniciamos el módulo SD
  delay(1000);
  Serial.print("\Inicializando SD card...");
  SDC.validarSD();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  double flat, flon;
  unsigned long chars, prec, age;
  unsigned short sentences, failed, sat;
  GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
  stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed ; //combining both whole and decimal part in string with a fullstop between them
  StringConcatenar(stringVal, "char:   ");
  SDC.GuardarSDGPS(stringVal,"GPS1.txt");
  SDC.GuardarSDTem(mlx.readAmbientTempC(), mlx.readAmbientTempF(), 1,"TempA1.txt");
  SDC.GuardarSDTem(mlx.readObjectTempC(), mlx.readObjectTempF(), 2,"TempO1.txt");
  stringVal = "";
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");

  delay(30000);
}
//metodo que utilizo para enviar mensajes convierte la cadena en un char
void StringConcatenar(String val, String come)
{
  char sms [val.length() + 1];
  stringVal.toCharArray(sms, val.length() + 1);
  Serial.print(come);
  for (uint8_t i = 0; i < sizeof(sms); i++) Serial.print(sms[i]); //display character array
  //pes.mensaje_sms("AT+CMGS=\"+50687299857\"", sms);
  Serial.println(); Serial.println();
}
