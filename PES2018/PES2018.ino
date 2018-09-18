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

      Modulo SIM900 conectada al bus SPI de la siguiente manera:
 ** RXD-TX1 pin18
 ** TX1-RXD pin19

      Modulo GPS conectada al bus SPI de la siguiente manera:
 ** RXD-TX3 pin14
 ** TX3-RXD pin15

      Modulo MLX90614 Temp Sensor conectada al bus SPI de la siguiente manera:
 ** SCL-SCL1
 ** SDA-SDA1
*/

#include <Wire.h>
// include the sensor library
#include <Adafruit_MLX90614.h>
// include the GPS library:
#include <TinyGPS.h>
// include the SD library:
#include <SPI.h>
#include <SD.h>

/*
   variables GSM
*/
int respuesta;
char aux_str[50];
//Contenido del sms que enviamos. \x1A corresponde al caracter de finalizacion
char sms[100] = "PES2018! \x1A \r\n";
char smsF[] = "mundo\x1A \r\n";
char datosSERIAL[10];
/*
   variables GPS
*/
TinyGPS gps;
//SoftwareSerial Serial3(4, 3);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
/*
   variables MLX90614 Temp Sensor
*/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

/*
   Variables SD Card:
*/
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 53;
void setup() {
  Serial1.begin(9600); //Configura velocidad del puerto serie para el Serial1 Modulo GSM
  Serial3.begin(9600); //Configura velocidad del puerto serie para el Serial3 Modulo GPS
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  mlx.begin(); //Configura velocidad del MLX90614 Temp Sensor para el Modulo Temp Sensor
  delay(1000);
  Serial.println("Iniciando...");
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
  }
  Serial.print("\Inicializando SD card...");
  validarSD();
  iniciarGSM();
  Menu();

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available())
    switch (Serial.read())
    {
      case '1':
        llamar();
        Serial.println(); Serial.println();
        Menu();
        break;
      case '2':
        recarga();
        mensaje_sms();
        
        Serial.println(); Serial.println();
        Menu();
        break;
      case '3':
        recarga();
        Serial.println(); Serial.println();
        Menu();
        break;
      case '4':
        iniciarGPS();
        loopgps();loopgps();loopgps();
        Serial.println(); Serial.println();
        Menu();
        break;
      case '5':
        iniciarTempA();
        Serial.println(); Serial.println();
        Menu();
        break;
      case '6':
        iniciarTempO();
        Serial.println(); Serial.println();
        Menu();
        break;
      case '7':
        probarSD();
        Menu();
        break;
    }
  if (Serial1.available()) {
    recibirMSM();


  }
}
void recibirMSM() {
String txtMsg =Serial1.readStringUntil('\n'); 
  //char DAT = (char)Serial1.read();
   //char inChar = Serial1.read();
 //ile (Serial1.available() > 0) {
//   if(strstr(txtMsg, "@")){
    //Serial.write(DAT);
  // }
//   else{
   Serial.print(txtMsg);
  // }
   
    //txtMsg += inChar;
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






void Menu() {
  Serial.println("Proyecto PES-2018");
  Serial.println("Menu");
  Serial.println("Digite el numero segun la opcion que desea implementar:");
  Serial.println("1. Llamadar ");
  Serial.println("2. Enviar Mensaje");
  Serial.println("3. Activar Internet por una hora ");
  Serial.println("4. Utilizar GPS");
  Serial.println("5. Temperatura Ambiente");
  Serial.println("6. Temperatura Objeto");
  Serial.println("7. Visualizar Informacion de SD Card");
  Serial.println("Esperando opcion.... ");

}
/* Modulo GSM
    -Utilizamos el (Metodo enviarAT) para enviar los datos AT y validar que esten correctos
    -En este espacio se inicializa los comandos para el AT (Metodo iniciar)
    -Utilizamos el (Metodo mensaje_sms)para enviar mensajes al telefono indicado
    -Utilizamos el (Metodo recarga)para enviar mensajes a Movistar y podremos utilizar internet por una hora
    -Utilizamos el (Metodo llamar)para realizar la llamada al telefono indicado
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
/*fin MODULO GSM*/
///////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
void iniciarGPS() {


  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
}
void loopgps() {
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  print_date(gps);
  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
  print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
  print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();

  smartdelay(1000);
}


static void smartdelay(unsigned long ms)
{
  TinyGPS gps;
  unsigned long start = millis();
  do
  {
    while (Serial3.available())
      gps.encode(Serial3.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
            month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
  smartdelay(0);
}
/////////////////////////////////////
/////////////////////////////////////
////////////////////////////////////

void iniciarTempA() {

  Serial.print("La Temperatura del Ambient es = "); Serial.print(mlx.readAmbientTempC()); Serial.println("*C");
  Serial.print("La Temperatura del Ambient es = "); Serial.print(mlx.readAmbientTempF()); Serial.println("*F");
  delay(500);

}
void iniciarTempO() {
  Serial.print("La Temperatura del Object es= "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("La Temperatura del Object es= "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  delay(500);
}
void validarSD() {

  // usaremos el código de inicialización de las bibliotecas de utilidades
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("error de inicialización. Cosas que verificar:");
    Serial.println("* ¿está insertada una tarjeta?");
    Serial.println("* ¿su cableado es correcto?");
    Serial.println("* ¿Cambió el chipSelect pin para que coincida con su escudo o módulo?");
    return;
  } else {
    Serial.println("El cableado es correcto y hay una tarjeta presente.");
  }

}
void probarSD() {
  // Imprimimos el tipo de tarjeta
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;

  }
  //  imprime el tipo y el tamaño del primer volumen de tipo FAT
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);


  Serial.println("\nArchivos encontrados en la tarjeta (nombre, fecha y tamaño en bytes): ");
  root.openRoot(volume);

  // lista todos los archivos en la tarjeta con fecha y tamaño
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


/*void enviarTempAMsm{
   char smsC[] = "La Temperatura del Ambient es ";
  if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //comprueba la conexion a la red
  {
    Serial.println("Enviando SMS...");
    enviarAT("AT+CMGF=1\r", "OK", 1000); //Comando AT para mandar un SMS
    sprintf(aux_str, "AT+CMGS=\"+50670065506\"", strlen(sms)); //Numero al que vamos a enviar el mensaje
    //Texto del mensaje
    if (enviarAT(aux_str, ">", 10000) == 1)
    {
      enviarAT(smsC, "OK", 10000);
    }
    Serial.println("SMS enviado");
  }
  delay(300);
  dtostrf(mlx.readAmbientTempC(), 10, 2, smsC);
  if (enviarAT("AT+CREG?", "+CREG: 0,1", 1000) == 1) //comprueba la conexion a la red
  {
    Serial.println("Enviando SMS...");
    enviarAT("AT+CMGF=1\r", "OK", 1000); //Comando AT para mandar un SMS
    sprintf(aux_str, "AT+CMGS=\"+50670065506\"", strlen(sms)); //Numero al que vamos a enviar el mensaje
    //Texto del mensaje
    if (enviarAT(aux_str, ">", 10000) == 1)
    {
      enviarAT(smsC, "OK", 10000);
    }
    Serial.println("SMS enviado");
  }
  }
  }
*/
