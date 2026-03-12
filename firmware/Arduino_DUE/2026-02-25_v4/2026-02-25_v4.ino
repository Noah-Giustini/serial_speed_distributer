String speed_from_obd;
String client1;
String client2;

enum State {
  READ_OBD,
  READ_CLIENT,
};

State currentState = READ_OBD;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); // Serial client 1
  Serial2.begin(115200); // Serial client 2
  Serial3.begin(115200); // Serial client OBD

  Serial3.write("ATZ\r\n"); // 1, Transmitting initializaiton
  delay(300);

  while (Serial3.available() > 0){
    char initialize = Serial3.read(); // 2, Receiving initialization message
  }

  delay(3000);
}

void loop() {
  switch (currentState){
    case READ_OBD:
      Serial3.write("010D\r\n"); // 3, Transmitting request speed to OBDII
      Serial3.flush(); // Waits for the write to finish

      speed_from_obd = Serial3.readStringUntil('>'); // 4, Receiving requested speed from OBD

      currentState = READ_CLIENT;
      break;

////////////////////////////////////////////////////////////////////////////////

    case READ_CLIENT:
      if (Serial1.available() > 0){ // CLIENT 1
        client1 = Serial1.readString();
        if (client1.indexOf("010D") >= 0){
          Serial1.print(speed_from_obd);
        }
      }

      if (Serial2.available() > 0){ // CLIENT 2
        client2 = Serial2.readString();
        if (client2.indexOf("010D") >= 0){
          Serial2.print(speed_from_obd);
        }
      }
      
      currentState = READ_OBD;
      break;
  }
}
