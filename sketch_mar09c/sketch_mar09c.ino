#include <SoftwareSerial.h>
#include<stdio.h>
//SoftwareSerial Serial2(2,3);
void get_wifi_list();
uint8_t flag = 0;

void setup() {                

    Serial.begin(9600);
 
    Serial2.begin(9600);
    delay(1000);

}

void loop(){

  if(Serial.available()){
    delay(10);  
    String cmd = "";
    
    while(Serial.available()){
        cmd += (char)Serial.read();
        }
    //Serial.print(cmd);
    sendData(cmd,2000,true);
    enviarAT(cmd.c_str(),"OK",2000);
      }
 
}

//Metodo que envia os comandos para o Serial2
int enviarAT(char* ATcommand, char* resp_correcta, unsigned int tiempo)
{
  int x = 0;
  bool correcto = 0;
  char respuesta[100];
  unsigned long anterior;
  String mensaje;
Serial.print(ATcommand);Serial.print("");
//Serial.print(resp_correcta);Serial.print("");

  memset(respuesta, '\0', 100); // Inicializa el string
  delay(100);
  
  while (Serial2.available()>0){
    Serial2.read(); // Limpia el buffer de entrada
  }
  Serial2.println(ATcommand); // Envia el comando AT
  
  x = 0;
  anterior = millis();
  // Espera una respuesta
  do {
    // si hay datos el buffer de entrada del UART lee y comprueba la respuesta
    if (Serial2.available() != 0)
    {
     // Serial.println(ATcommand);
      //Comprueba que no haya desbordamiento en la capacidad del buffer
      if (x < 99) {
        respuesta[x] = Serial2.read();
        x++;
      }
      else{
        Serial.println("Desbordamiento!");
      }
      // Comprueba si la respuesta del modulo es la 1
      if (strstr(respuesta, resp_correcta) != NULL)
      {
        correcto = 1;
      }
    }
  }
  // Espera hasta tener una respuesta
  while ((correcto == 0) && ((millis() - anterior) < tiempo));
Serial.print("respuesta");
    Serial.print(respuesta);

     
  //Serial.println(ATcommand);
  return correcto;
}
String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  Serial2.print(command);
  Serial.print(command);
  long int time = millis();
 while ((time + timeout) > millis())
  { 
    while (Serial2.available()) 
    {
      char c = Serial2.read();
      response += c;
    }
   }
  if (debug) {
    Serial.print(response);
  }
  //Serial.print(response);
  return response;
}
void get_wifi_list(){
  String cmd = "AT+CWLAP";
  Serial2.println(cmd);
}
