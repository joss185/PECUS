const uint64_t pipe = 0xE8E8F0F0E1LL;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop() {
  // Codigo de Receptor RF
  if (radio.available()){
    int done = radio.read(msg,16);
    Serial.println(msg);
    
    }
  
}
