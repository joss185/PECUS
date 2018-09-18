
/*Biblioteca TinyGPS
TinyGPS, de Mikal Hart , convierte los datos de posicionamiento global en formato NEMA en variables fáciles de usar para Latitud, Longitud, Tiempo y otra información.
Descargar :  Incluido con el instalador Teensyduino
Últimos desarrollos en Github
Requisitos de hardware
TinyGPS solo convierte datos. Debe usar UART Serial o NewSoftSerial o de alguna otra forma para obtener los datos de formato NEMA para alimentar a TinyGPS. Aunque TinyGPS no requiere ningún hardware, en la práctica, necesitará un módulo GPS para la fuente de datos.
 
En esta foto, se utilizó un módulo GPS PMB-648 de Parallax . Dado que estos módulos de GPS pueden funcionar con alimentación de 5 o 3,3 voltios, y tienen señales serie de nivel TTL que se pueden conectar directamente a los pines Teensy's RX y TX, conectarlos a Teensy es muy fácil.

 
Para usar con Teensy 3.0, la mayoría de los módulos de GPS emiten una señal de 3,3 voltios de "nivel de TTL", incluso cuando funcionan con 5 voltios.

 
La salida de nivel TTL es en realidad de 2,8 voltios, incluso con una potencia de 5V.
Uso Básico
TinyGPS gps
Crea el objeto TinyGPS, dándole el nombre que elijas. Si bien podría crear múltiples objetos TinyGPS, es poco probable que necesite decodificar más de un flujo de datos de formato NEMA.

GPS. codificar (c)
Cada byte de datos NEMA debe estar dando a TinyGPS utilizando encode (). True se devuelve cuando los datos nuevos se han descodificado por completo y se pueden usar.

GPS. get_position (& latitude, & longitude, & age)
Obtenga la posición, donde la latitud y la longitud son variables de tipo largo, y los valores se multiplican por 10.000. La variable de edad debe ser unsigned long type.

GPS. f_get_position (& flatitude, & flongitude, & age)
Obtenga la posición, donde la latitud y la longitud son variables de tipo flotante, y se devuelven los valores reales. Los tipos de flotador son más fáciles de usar, pero dan como resultado un código más grande y más lento. La variable de edad debe ser unsigned long type.

Se pueden obtener muchas otras variables. Consulte la página oficial de TinyGPS para obtener la documentación completa o el código de ejemplo para ver cómo se usan.
Programa de ejemplo
 
Este ejemplo es una versión ligeramente modificada del programa de prueba que viene en el directorio de ejemplos con TinyGPS. Utiliza el puerto serie real (UART) de Teensy en lugar de NewSoftSerial. Teensy también puede transmitir datos rápidamente en USB, por lo que se ha eliminado la verificación adicional de los datos de entrada que serían necesarios en el objeto en serie lento de Arduino.
*/
#include < TinyGPS.h>

/ * Este código de muestra demuestra el uso normal de un objeto TinyGPS. * / 
TinyGPS gps;

/ * En Teensy, el UART (puerto serie real) siempre es mejor usar. * / 
/ * A diferencia de Arduino, no es necesario utilizar NewSoftSerial porque * / 
/ * el objeto "Serial" usa el puerto USB, dejando el UART libre. * / 
HardwareSerial  Uart = HardwareSerial ();

void gpsdump ( TinyGPS y gps);
void printFloat ( double f, int digits = 2);

 Configuración de vacío ()
{
  Serial . comenzar (115200);
  Uart . comenzar (4800);
  
  retraso (1000);
  Serial . print ( "Prueba de la biblioteca TinyGPS v." ); Serial . println ( TinyGPS :: library_version ());
  Serial . println ( "por Mikal Hart" );
  Serial . println ();
  Serial . print ( "Sizeof (gpsobject) =" ); Serial . println (sizeof ( TinyGPS ));
  Serial . println ();
}

void  loop ()
{
  bool newdata =  falso ;
  inicio largo sin signo  = millis ();

  // Cada 5 segundos, imprimir una actualización 
  , mientras que ( Millis () - Inicio <5,000) {
     si ( UART . Disponible ()) {
       char c = Uart . leer ();
      // Serial.print (c); // descomentar para ver datos de GPS sin procesar 
      if (gps. encode (c)) {
        newdata =  verdadero ;
        // descanso; // descomentar para imprimir datos nuevos de inmediato!
      }
    }
  }
  
  if (newdata) {
     Serial . println ( "Datos adquiridos" );
    Serial . println ( "-------------" );
    gpsdump (gps);
    Serial . println ( "-------------" );
    Serial . println ();
  }
}

void gpsdump ( TinyGPS y gps)
{
  latitud larga , lon;
  flotar plano, flon;
  edad larga sin firmar  , fecha, hora, caracteres;
  año int .
  byte mes, día, hora, minuto, segundo, centésimas;
  frases cortas sin firmar , fallaron; 

  GPS. get_position (& lat, & lon, & age);
  Serial . imprimir ( "Lat / Long (10 ^ -5 grados):" ); Serial . imprimir (lat); Serial . imprimir ( "," ); Serial . imprimir (lon); 
  Serial . print ( "Edad de reparación:" ); Serial . imprimir (edad); Serial . println ( "ms." );
  
  // En Arduino, los caracteres de GPS pueden perderse durante el largo Serial.print () 
  // En Teensy, el Serial imprime en USB, que tiene un gran buffer de salida y 
  // se ejecuta muy rápido, por lo que no es necesario preocuparse por la falta de 4800 
  // baud caracteres de GPS.

  GPS. f_get_position (& flat, & flon, & age);
  Serial . print ( "Lat / Long (flotante):" ); printFloat (plano, 5); Serial . imprimir ( "," ); printFloat (flon, 5);
  Serial . print ( "Edad de reparación:" ); Serial . imprimir (edad); Serial . println ( "ms." );

  GPS. get_datetime (& date, & time, & age);
  Serial . print ( "Fecha (ddmmyy):" ); Serial . imprimir (fecha); Serial . print ( "Tiempo (hhmmsscc):" );
    Serial . imprimir (tiempo);
  Serial . print ( "Edad de reparación:" ); Serial . imprimir (edad); Serial . println ( "ms." );

  GPS. crack_datetime (& year, & month, & day, & hour, & minute, & second, & hundredths, & age);
  Serial . imprimir ( "Fecha:" ); Serial . print (static_cast < int > (mes)); Serial . imprimir ( "/" ); 
    Serial . print (static_cast < int > (día)); Serial . imprimir ( "/" ); Serial . imprimir (año);
  Serial . imprimir ( "Tiempo:" ); De serie. print (static_cast < int > (hora)); Serial . imprimir ( ":" ); 
    Serial . print (static_cast < int > (minuto)); Serial . imprimir ( ":" ); Serial . print (static_cast < int > (second));
    Serial . imprimir ( "." ); Serial . print (static_cast < int > (centésimas));
  Serial . print ( "Edad de reparación:");  Serial . imprimir (edad); Serial . println ( "ms." );

  Serial . print ( "Alt (cm):" ); Serial . imprimir (gps altitud ()); Serial . print ( "Curso (10 ^ -2 grados):" );
    Serial . imprimir (gps. curso ()); Serial . print ( "Velocidad (10 ^ -2 nudos):" ); Serial . println (gps. velocidad ());
  Serial . print ( "Alt (flotante):" ); printFloat (gps. f_altitude ()); De serie. print ( "Curso (flotante):" );
    printFloat (gps. f_course ()); Serial . println ();
  Serial . print ( "Velocidad (nudos):" ); printFloat (gps. f_speed_knots ()); Serial . imprimir ( "(mph):" );
    printFloat (gps. f_speed_mph ());
  Serial . print ( "(mps):" ); printFloat (gps. f_speed_mps ()); Serial . print ( "(kmph):" );
    printFloat (gps. f_speed_kmph ()); Serial . println ();

  GPS. stats (& chars, & sentence, & failed);
  Serial . print ( "Estadísticas: caracteres:" ); Serial . imprimir (caracteres); Serial . print ( "oraciones:" );
    Serial . imprimir (oraciones); Serial . print ( "suma de verificación fallida:" ); Serial . println (fallido);
}

anular printFloat ( doble número, int dígitos)
{
  // Manejar números negativos 
  si (número <0.0) {
      Serie . imprimir ( '-' );
     number = -number;
  }

  // Redondee correctamente para que la impresión (1.999, 2) se imprima como "2.00" 
  doble redondeo = 0.5;
  para (uint8_t i = 0; i <dígitos; ++ i)
    redondeo / = 10.0;
  
  número + = redondeo;

  // Extrae la parte entera del número e imprímala 
  sin signo  largo int_part = ( sin signo  largo ) número;
  doble resto = número - ( doble ) int_part;
  Serial . imprimir (int_part);

  // Imprime el punto decimal, pero solo si hay dígitos más allá de 
  if (dígitos> 0)
     Serial . imprimir ( "." );

  // Extrae los dígitos del resto uno por uno 
  mientras (dígitos -> 0) {
    resto * = 10.0;
    int toPrint = int (remainder);
    Serial . imprimir (imprimir);
    resto - = toPrint;
  }
}
