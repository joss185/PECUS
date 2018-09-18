void setup() {
  // put your setup code here, to run once:
   Serial1.begin(9600);  //iniciar la comunicacion serial
 pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(13, HIGH);
  delay(3000); // Wait for 1000 millisecond(s)
  Serial1.print("Rojo: ");

  Serial1.println(". ");
 // delay(500);
 
  digitalWrite(13, LOW);
  delay(1000); // Wait for 1000 millisecond(s)

  
}
