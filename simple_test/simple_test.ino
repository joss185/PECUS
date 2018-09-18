#include <Pes2018GPS.h>

#include <SoftwareSerial.h>

#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and aSerial3umes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;

Pes2018GPS GP = Pes2018GPS(&Serial3);
//SoftwareSerial Serial3(4, 3);

void setup()
{
  Serial.begin(9600);

  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println();
}

void loop()
{
  float flat, flon;
  unsigned long chars,prec,age;
  unsigned short sentences, failed,sat;
  GP.loopgps(&flat, &flon, &age, &sat,&prec, &chars, &sentences, &failed);
  //gps.f_get_position(&flat, &flon, &age);
  Serial.print("LAT=");
  Serial.print(flat );
  Serial.print(" LON=");
  Serial.print(flon );
  Serial.print(" SAT=");
  Serial.print(sat );
  Serial.print(" PREC=");
  Serial.print(prec );
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);

  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}
