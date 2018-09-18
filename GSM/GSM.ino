/*
 ** Modulo GSM  para realizar los metodos del proyecto PES2018-ULATINA BOVINE CONTROL NECKLACE PROTOTIPO
  Creada: Yilmar Mazo Vargas
  Febrero 2018 ---
*/

/*
  El circuito:
      Modulo SIM900 conectada al bus SPI de la siguiente manera:
 ** RXD-TX1 pin18
 ** TX1-RXD pin19

*/

/*
   variables GSM
*/
int respuesta;
char aux_str[50];
//Contenido del sms que enviamos. \x1A corresponde al caracter de finalizacion
char sms[100] = "PES2018! \x1A \r\n";
char smsF[] = "mundo\x1A \r\n";
char datosSERIAL[10];


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600); //Configura velocidad del puerto serie para el Serial1 Modulo GSM
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  delay(1000);
  Serial.println("Iniciando...");
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
  }
  Menu();
  iniciarGSM();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (Serial.read())
  {
    case '1':
      llamar();
      Serial.println(); Serial.println();
      Menu();
      break;
    case '2':
      mensaje_sms();
      Serial.println(); Serial.println();
      Menu();
      break;
    case '3':
      recarga();
      Serial.println(); Serial.println();
      Menu();
      break;
  }
  if (Serial1.available()) {
    Serial.write(Serial1.read());
   // recibirMSM();
  }
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
/* Modulo GSM
    -Utilizamos el (Metodo enviarAT) para enviar los datos AT y validar que esten correctos
    -En este espacio se inicializa los comandos para el AT (Metodo iniciar)
    -Utilizamos el (Metodo mensaje_sms)para enviar mensajes al telefono indicado
    -Utilizamos el (Metodo recarga)para enviar mensajes a Movistar y podremos utilizar internet por una hora
    -Utilizamos el (Metodo llamar)para realizar la llamada al telefono indicado
    -Utilizamos el (Metodo recibirMSM) para recibir cualquier dato del Serial1 
*/
int enviarAT(char* ATcommand, char* resp_correcta, unsigned int tiempo)
{

  int x = 0;
  bool correcto = 0;
  char respuesta[100];
  unsigned long anterior;

  memset(respuesta, '\0', 100); // Inicializa el string
  delay(100);
  while ( Serial1.available() > 0) Serial1.read(); // Limpia el buffer de entrada
  Serial1.println(ATcommand); // Envia el comando AT
  x = 0;
  anterior = millis();
  // Espera una respuesta
  do {
    // si hay datos el buffer de entrada del UART lee y comprueba la respuesta
    if (Serial1.available() != 0)
    {
      //Comprueba que no haya desbordamiento en la capacidad del buffer
      if (x < 99) {
        respuesta[x] = Serial1.read();
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

  return correcto;
}
void iniciarGSM()
{
  enviarAT("AT+CPIN=\"8528\"", "OK", 1000);//se envia el numero de PIN en caso que este lo tenga activado
  Serial.println("Conectando a la red...");
  delay (5000);

  //espera hasta estar conectado a la red movil
  while ( enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 0 )
  {
  }
  Serial.println("Conectado a la red.");
  enviarAT("AT+CLIP=1\r", "OK", 1000); // Activamos la identificacion de llamadas
  enviarAT("AT+CMGF=1\r", "OK", 1000); //Configura el modo texto para enviar o recibir mensajes
  enviarAT("AT+CNMI=2,2,0,0,0\r", "OK", 1000); //Configuramos el modulo para que nos muestre los SMS recibidos por comunicacion serie
  Serial.println("Preparado.");
}

void mensaje_sms()
{
  if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //comprueba la conexion a la red
  {
    Serial.println("Enviando SMS...");
    enviarAT("AT+CMGF=1\r", "OK", 1000); //Comando AT para mandar un SMS
    sprintf(aux_str, "AT+CMGS=\"+50670065506\"", strlen(sms)); //Numero al que vamos a enviar el mensaje
    //Texto del mensaje
    if (enviarAT(aux_str, ">", 10000) == 1)
    {
      enviarAT(sms, "OK", 10000);
    }
    Serial.println("SMS enviado");
  }
}

void recarga()
{
  if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //comprueba la conexion a la red
  {
    Serial.println("Enviando SMS...");
    enviarAT("AT+CMGF=1\r", "OK", 1000); //Comando AT para mandar un SMS
    sprintf(aux_str, "AT+CMGS=\"606\"", strlen(sms)); //Numero al que vamos a enviar el mensaje
    //Texto del mensaje
    if (enviarAT(aux_str, ">", 10000) == 1)
    {
      enviarAT("HORAWEB", "OK", 10000);
    }
    Serial.println("SMS enviado");
  }

} void llamar()
{
  if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //Comprueba la conexion a la red
  {
    Serial.println("Realizando llamada...");
    enviarAT("ATD +50670065506;", "OK", 1000);
    delay(30000); // Espera 30 segundos mientras realiza la llamada
    enviarAT("ATH", "OK", 1000); // Cuelga la llamada
    Serial.println("Llamada finalizada");
  }

}
void recibirMSM() {
String txtMsg =Serial1.readStringUntil('\n'); 
  char DAT = Serial1.read();
   char inChar = Serial1.read();
 //if (Serial1.available() > 0) {
//   if(strstr(txtMsg, "@")){
   // Serial.write(DAT);
   //}
//   else{
  // Serial.write("hola");
  // }
   
    txtMsg += inChar;
  //}
  //Serial.println("Mensaje recibido ");//para verificar si lo detecto
//  if (DAT == '@') //el arroba hace detectar el inicio del codigo
  //{
  //Serial.print(txtMsg);
  
    
    //Serial.write(DAT);//Imprimimos en consola mensaje enviado desde el mobil
    //Serial.write(data);//Imprimimos en consola mensaje enviado desde el mobil
    //     }
  //}
}





















