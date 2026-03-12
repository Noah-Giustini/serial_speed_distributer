void setup() {
  //Initialize
  Serial.begin(115200); //Main arduino

  Serial.write("ATZ\r\n");
  delay(100);

  while (Serial.available() > 0){
    char message = Serial.read();
    Serial.print(message);
  }
  delay(3000);
}

void loop() {
  Serial.write("010D\r\n");
  delay(100);

  while (Serial.available() > 0){
    char message = Serial.read();
    Serial.print(message);
  }
  delay(1000);
}