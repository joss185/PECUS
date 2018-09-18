void setup()
  {  Serial.begin(9600);
     Serial2.begin(9600);
  }

void loop()
  {
     // Repetir lo recibido por el ESP8266 hacia el monitor serial
     if (Serial2.available())
         { char c = Serial2.read() ;
           Serial.print(c);
         }

     // Repetir lo recibido por el monitor serial hacia el ESP8266
     if (Serial.available())
         {  char c = Serial.read();
            Serial2.print(c);
         }
   }
