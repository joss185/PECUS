#include <TinyGPS.h>

#include <Pes2018GPS.h>

#include <Pes2018SD.h>

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
/*
   Variables SD Card:
*/
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 53;
TinyGPS TGPS;
Pes2018SD SDC = Pes2018SD(chipSelect,&Serial3);
//Pes2018GPS GPS = Pes2018GPS(&Serial3);
/*
   variables MLX90614 Temp Sensor
*/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

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

  mlx.begin(); //Configura velocidad del MLX90614 Temp Sensor para el Modulo Temp Sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    switch (Serial.read())
    {
      case '1':
        //LeerSD();
        break;
      case '2':
        iniciarTempA();
        break;
      case '3':
        iniciarTempO();
        break;
      case '4':
        SDC.GuardarSD(mlx.readAmbientTempC(), mlx.readAmbientTempF(), 1);
        SDC.GuardarSD(mlx.readObjectTempC(), mlx.readObjectTempF(), 2);
        break;
      case '5':
        iniciarGPS();
       SDC.GuardarSDGPS();Serial.println();
       SDC.GuardarSDGPS();Serial.println();
       SDC.GuardarSDGPS();Serial.println();
        break;
    }

  }

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

void LeerSD() {
  // Imprimimos el tipo de tarjeta
  Serial.print("\nCard type: qq");
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


void iniciarGPS() {


  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
}




