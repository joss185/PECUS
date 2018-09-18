
#define DEBUG true

void setup()
{
  
  delay(500);
  //Seta ambas seriais para a velocidade de 9600 
  Serial.begin(9600);
  //(em alguns casos a velocidade do seu Serial2 pode estar diferente desta)
  Serial2.begin(9600); 
  
 
  //Envia o comandos AT
  
  // reseta o modulo
  sendData("AT+RST\r\n",2000,DEBUG);
  // configure as access point e estação (ambos)
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); 
  //conecta ao roteador com a senha  
  //(esta configuração deve ser feita, pois o seu roteador tem nome diferente do meu e senha)
  sendData("AT+CWJAP=\"familiamazovargas\",\"Santy25092007\"\r\n",10000,DEBUG); 
  //Retorna o IP ao qual está conectado e o IP de Station
  sendData("AT+CIFSR\r\n",1000,DEBUG); 
  //Habilita multiplas conexões
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); 
  //Habilita ao servidor a porta 80
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); 
}
 
void loop()
{
  //verifica se o Serial2 esta enviando mensagem e esta disponivel
  IniciarWiFi();
}
 void IniciarWiFi(){
  if(Serial2.available()) 
  {    
    if(Serial2.find("+IPD,"))
    {    
    // subtrai 48, por que o metodo read() retorna os valores ASCII, o primeiro numero decimal começa em 48 
     int connectionId = Serial2.read()-48;      
     //Inicia a montagem da pagina web
     //criando a variavel webpage e armazenando os dados nela
     String webpage = "<head><meta http-equiv=""refresh"" content=""5""></head>"; 
     webpage+="<h1>Ola Mundo</h1><h2> Serial2</h2></br><h3>Temperatura : ";
     //Envia o valor lido do sensor de temperatura
     webpage+= "Paso1"; 
     webpage+=" </br> Iluminosidade : ";
       //Envia o valor lido do sensor de ldr
      webpage+= "paso 2 </h3>";
     
     //cria uma variavel para enviar o comando até
     //que enviará as informações para o modulo Serial2 
     String cipSend = "AT+CIPSEND=";      
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     //Envia os comandos para o modulo
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
     
     //Encerra os comandos
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId;
     closeCommand+="\r\n";
     
     //Envia os comandos de encerramento
     sendData(closeCommand,2000,DEBUG);
     
     delay(5000);
    }
  }
 }
 
 //Metodo que envia os comandos para o Serial2
String sendData(String command, const int timeout, boolean debug)
{
    //variavel de resposta do Serial2
    String response = "";
    
    // send a leitura dos caracteres para o Serial2
    Serial2.println(command); 
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial2.available())
      {
        

        //Concatena caracter por caractere recebido do modulo Serial2 
        char c = Serial2.read(); 
        response+=c;
      }  
    }
    
    //debug de resposta do Serial2
    if(debug)
    {
      //Imprime o que o Serial2 enviou para o arduino
      Serial.println("Arduino : " + response);
    }
    
    return response;
}



//Metodo que converte float em string
long getDecimal(float val)
{
  //converte o float para inteiro 
  int intPart = int(val);
  
  //multiplica por 100
  //precisão de 2 casas decimais
  long decPart = 100*(val-intPart);
  
  //Se o valor for maior que 0 retorna
  if(decPart>0)
  //retorna a variavel decPart 
  return(decPart);           
  
  //caso contrario retorna o valor 0
  else if(decPart=0)
     return(00);           
}
