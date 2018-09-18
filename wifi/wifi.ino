#define DEBUG true
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
Serial.print("ini..");
  sendData("AT+CWMDE=?\r\n", 1000, DEBUG);
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
  delay(1000);
  //sendData("AT+CWJAP=\"familiamazovargas\",\"Santy25092007\"r\n",10000,DEBUG); 
  sendData("AT+CWJAP=\"Mate9\",""\"\r\n", 1000, DEBUG);
  delay(9000);
  sendData("AT+CIFRS\r\n", 1000, DEBUG);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()) {
    if (Serial2.find("+IPD,")) {
      delay(1000);


      int connectionId =Serial2.read()-48; 
      Serial.print(connectionId);
      String webpage ="<head><meta http-equiv=""refresh"" content=""5""></head>";
       webpage += "<h1>Tenemos conexion</h1></br></h3>";
      webpage += "<h2>Tenemos wifi</h2>";
      sendData("AT+CIPSEND=" + String(connectionId) + "," + webpage.length() + "\r\n",500, DEBUG);
      sendData(webpage, 1000, DEBUG);
      sendData("AT+CIPCLOSE=" + String(connectionId) + "\r\n", 2000, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  Serial2.print(command);
  //Serial.print(command);
  long int time = millis();

    while (Serial2.available()>0) 
    {
      char c = Serial2.read();
      response += c;
    }
  while ((time + timeout) > millis())
  {   }
  if (debug) {
    Serial.print(response);
  }
  Serial.print(response);
  return response;
}

