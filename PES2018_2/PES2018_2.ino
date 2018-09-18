 /*
 ** Modulos  para realizar los metodos del proyecto PES2018-ULATINA BOVINE CONTROL NECKLACE PROTOTIPO
  Creada: Yilmar Mazo Vargas
  Febrero 2018 ---
*/

/*
  El circuito:
      Modulo SD conectada al bus ICSP de la siguiente manera:
  ** MOSI - ICSP4 52
  ** MISO - ICSP1 50
  ** SCK - ICSP3, 52
  ** CS -53

       Modulo MLX90614 Temp Sensor conectada al bus SPI de la siguiente manera:
  ** SCL-SCL1
  ** SDA-SDA1
*/
// include the SD library:
#include <SPI.h>
#include <SD.h>
#include <Time.h>

// include the sensor library
#include <Adafruit_MLX90614.h>
// include the GPS library
#include <TinyGPS.h>
// include the PES2018 library
#include <Pes2018GPS.h>
#include <Pes2018MSM.h>
#include <Pes2018SD.h>
/*
   Variables SD Card:
*/
#define DEBUG true
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 53;
/*
   Variables GPS:
*/
TinyGPS gps;
String stringVal = "";
Pes2018GPS GP = Pes2018GPS(&Serial3);
Pes2018SD SDC = Pes2018SD(chipSelect);
/*
   variables MLX90614 Temp Sensor
*/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
/*
   variables SIM900
*/
Pes2018MSM pes = Pes2018MSM(&Serial1);
char _sms[100] = "PES2018! \x1A \r\n";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Configura velocidad del puerto serie del Arduino
  Serial.println("Iniciando...");
  SD.begin(chipSelect); // Iniciamos el módulo SD
  while (!Serial) {
    ; // Espere a que se conecte el puerto serie. Necesario solo para el puerto USB nativo
  }
  Serial.print("\Inicializando SD card...");
  SDC.validarSD();
  pes.iniciarGSM();
  mlx.begin(); //Configura velocidad del MLX90614 Temp Sensor para el Modulo Temp Sensor
  delay(500);
  //Seta ambas seriais para a velocidade de 9600

  //(em alguns casos a velocidade do seu Serial2 pode estar diferente desta)
  Serial2.begin(9600);


  //Envia o comandos AT

  // reseta o modulo



}

void loop() {
  IniciarWiFi();
  // put your main code here, to run repeatedly:
  double flat, flon;
  unsigned long chars, prec, age;
  unsigned short sentences, failed, sat;
  if (Serial1.available())
    switch (pes.recibirSMS())
    {
      case 1:
       // SDC.GuardarSDTem(mlx.readAmbientTempC(), mlx.readAmbientTempF(), 1);
        //SDC.GuardarSDTem(mlx.readObjectTempC(), mlx.readObjectTempF(), 2);
        GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
        //SDC.GuardarSDGPS(flat, flon, age, sat, prec, chars, sentences, failed ); Serial.println();

        Serial.print("LAT=");
        Serial.print(flat );  Serial.print(" LON=");  Serial.print(flon );  Serial.print(" SAT=");  Serial.print(sat );  Serial.print(" PREC=");  Serial.print(prec );  Serial.print(" CHARS=");  Serial.print(chars);  Serial.print(" SENTENCES=");  Serial.print(sentences);  Serial.print(" CSUM ERR=");  Serial.println(failed);
        if (chars == 0)
          Serial.println("** No characters received from GPS: check wiring **");
        break;
      case 2:
        break;
    }

  if (Serial.available())
    switch (Serial.read())
    {
      case '1':
        Serial.print("entro");
        //SDC.GuardarSDTem(mlx.readAmbientTempC(), mlx.readAmbientTempF(), 1);
        //SDC.GuardarSDTem(mlx.readObjectTempC(), mlx.readObjectTempF(), 2);
        GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
        stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed ; //combining both whole and decimal part in string with a fullstop between them      
        
//        SDC.GuardarSDGPS(stringVal);
        //SDC.GuardarSDGPS(flat, flon, age, sat, prec, chars, sentences, failed ); Serial.println();
        //Serial.print("LAT=");
        //Serial.print(flat );  Serial.print(" LON=");  Serial.print(flon );  Serial.print(" SAT=");  Serial.print(sat );  Serial.print(" PREC=");  Serial.print(prec );  Serial.print(" CHARS=");  Serial.print(chars);  Serial.print(" SENTENCES=");  Serial.print(sentences);  Serial.print(" CSUM ERR=");  Serial.println(failed);
        if (chars == 0)
          Serial.println("** No characters received from GPS: check wiring **");
        break;
      case '2':
        break;
    }
 Serial.print("entro");
       // SDC.GuardarSDTem(mlx.readAmbientTempC(), mlx.readAmbientTempF(), 1);
        //SDC.GuardarSDTem(mlx.readObjectTempC(), mlx.readObjectTempF(), 2);
        GP.loopgps(&flat, &flon, &age, &sat, &prec, &chars, &sentences, &failed);
        stringVal += "LAT=" + String(flat / 1000000, 6) + " LON=" + String(flon / 1000000, 6) + " SAT=" + sat + " PREC=" + prec + " CHARS=" + chars + " SENTENCES=" + sentences + " CSUM ERR=" + failed ; //combining both whole and decimal part in string with a fullstop between them      
        
//        SDC.GuardarSDGPS(stringVal);
        //SDC.GuardarSDGPS(flat, flon, age, sat, prec, chars, sentences, failed ); Serial.println();
        //Serial.print("LAT=");
        //Serial.print(flat );  Serial.print(" LON=");  Serial.print(flon );  Serial.print(" SAT=");  Serial.print(sat );  Serial.print(" PREC=");  Serial.print(prec );  Serial.print(" CHARS=");  Serial.print(chars);  Serial.print(" SENTENCES=");  Serial.print(sentences);  Serial.print(" CSUM ERR=");  Serial.println(failed);
        if (chars == 0)
          Serial.println("** No characters received from GPS: check wiring **");

         delay(5000); 
}





void IniciarWiFi() {
  enviarAT("AT+RST", "OK", 1000); //Restablecer el módulo
  enviarAT("AT+CWMODE=3", "OK", 1000); //Establezca la información de AP que se conectará por ESP8266
  //conecta ao roteador com a senha
  enviarAT("AT+CWLAP?", "OK", 1000);
  //(esta configuração deve ser feita, pois o seu roteador tem nome diferente do meu e senha)
  enviarAT("AT+CWJAP=\"familiamazovargas\",\"\"\r\n", "OK", 1000);
  //  enviarAT("AT+CWJAP?","OK",1000);
  //Retorna o IP ao qual está conectado e o IP de Station
  enviarAT("AT+CIFSR\r\n", "OK", 1000);
  //Habilita multiplas conexões
  enviarAT("AT+CIPMUX=1\r\n", "OK", 1000);
  //Habilita ao servidor a porta 80
  enviarAT("AT+CIPSERVER=1,80\r\n", "OK", 1000);
  if (Serial2.available())
  {
    if (Serial2.find("+IPD,"))
    {
      // subtrai 48, por que o metodo read() retorna os valores ASCII, o primeiro numero decimal começa em 48
      int connectionId = Serial2.read() - 48;
      Serial.println(connectionId);
      //Inicia a montagem da pagina web
      //criando a variavel webpage e armazenando os dados nela
      String webpage = "<head><meta http-equiv=""refresh"" content=""5""></head>";
      webpage += "<h1>Ola Mundo</h1><h2> Serial2</h2></br><h3>Temperatura : ";
      //Envia o valor lido do sensor de temperatura
      webpage += "Paso1";
      webpage += " </br> Iluminosidade : ";
      //Envia o valor lido do sensor de ldr
      webpage += "paso 2 </h3>";

      //cria uma variavel para enviar o comando até
      //que enviará as informações para o modulo Serial2
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";

      //Envia os comandos para o modulo
      enviarAT(cipSend.c_str(), 1000, DEBUG);
      enviarAT(webpage.c_str(), 1000, DEBUG);

      //Encerra os comandos
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId;
      closeCommand += "\r\n";

      //Envia os comandos de encerramento
      enviarAT(closeCommand.c_str(), 2000, DEBUG);

      delay(5000);
    }
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
  //Serial.print(ATcommand);
  //Serial.print(resp_correcta);
  //Serial.print(tiempo);
  memset(respuesta, '\0', 100); // Inicializa el string
  delay(100);

  while (Serial2.available() > 0) {
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
      else {
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

  Serial.print(respuesta);


  //Serial.println(ATcommand);
  return correcto;
}
