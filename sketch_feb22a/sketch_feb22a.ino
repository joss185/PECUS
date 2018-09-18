


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
while(Serial.available()){
  char ch =Serial.read();
  Serial.print(ch);
}
while(Serial2.available()){
  char ch =Serial2.read();
  Serial.print(ch);
}
}
