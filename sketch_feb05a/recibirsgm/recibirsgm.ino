
char numero_cell[] = "+50670065506"; //meter numero de telefono
char datosSERIAL[10];
int i = 0;
int j = 0;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  delay(1000);
  configuracion_inicial();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print(Serial1.read());
  //char DAT = Serial1.available();
  //Serial.print(DAT);
  //Serial.print(Serial1.available());
  Serial.println("esperando recibir mensaje...");
  while (true)
  {

    if (Serial1.available() > 0)//validamos si se ingresa algun dato en el cuadro de texto
    {
      ValidarConexion();

   //   leer_mensaje();
      switch (Serial.read())
      {
        case 'a':
          //envio_mensaje();
          break;
      }
    }
  }
}


void ValidarConexion() {
  char CValida = Serial1.read();
  Serial.print(CValida);
}
void configuracion_inicial()//configura los codigos de lectura de mensajes
{
  Serial1.println("AT+IPR=9600");//modo texto
  Serial.println("AT+IPR=9600");//modo texto
  delay(300);
  Serial1.println("AT+CMGF=1");//modo texto
  Serial.println("AT+CMGF=1");//modo texto
  delay(300);
  Serial1.println("AT+CMGR=1");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  Serial.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  delay(300);
  Serial1.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  Serial.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  //Serial1.println("AT+CNMI=3,3,0,0");//ACTIVAMOS PARA VER MENSAJES
  //Serial.println("AT+CNMI=3,3,0,0");//ACTIVAMOS PARA VER MENSAJES
  delay(300);
  Serial.println("configuracion terminada");
  delay(300);

}
void leer_mensaje()
{
if (Serial1.available() > 0)// validamos la conexion
  {
    char DAT = Serial1.read();
    Serial.print(DAT);
    delay(500);
    if (DAT == '@') //el arroba hace detectar el inicio del codigo
    { //If arroba

      char DAT_dos = Serial1.read(); //@LEDON enter
      Serial.print("DAT_dos: ");
      Serial.println(DAT_dos);
      datosSERIAL[j] = DAT; //almacena en cadena de caracteres, suma de caracteres
      j++;
      Serial.println("Lectura:");//IMPRIME LOS CARACTERES ALMACENADOS PARA VER SI TODO ESTA OK
      for (int i = 1; i <= j; i++)
      {
        Serial.print(datosSERIAL[i]);//IMPRIME TODO EL CODIGO GUARDADO EN EL ARRAY
      }
      // led_on();//llama a la la funcion y verifica codigo
      //   led_off();//llama ala funcion para ver si es de apagar el led
      delay(500);
      //digitalWrite(13,LOW);

      for (int i = 0; i <= j; i++)
      {
        datosSERIAL[i] == 0; //borro array
        DAT = 0;
      }
      j = 0;
    }//arroba
  }
  
}
