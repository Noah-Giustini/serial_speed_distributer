String speed_from_obd;
String client;

enum State {
  READ_OBD,
  READ_CLIENT,
  PROCESS_CLIENT
};

State currentState = READ_OBD;

void setup() {
  Serial2.begin(115200); // Serial with OBDII
  Serial3.begin(115200); // Serial client

  Serial2.write("ATZ\r\n"); // 1, Transmitting initializaiton
  delay(300);

  while (Serial.available() > 0){
    char initialize = Serial.read(); // 2, Receiving initialization message
  }
  delay(3000);
}

void loop() {
  switch (currentState){
    case READ_OBD:
      Serial2.write("010D\r\n"); // 3, Transmitting request speed to OBDII
      delay(100);

      while (Serial2.available() > 0){
        char temp = Serial2.read(); // 4, Receiving requested speed from OBDII
        speed_from_obd += temp;
      }   
      currentState = READ_CLIENT;
      break;

////////////////////////////////////////////////////////////////////////////////

    case READ_CLIENT:
      if (Serial3.available()){ // 5, Check if client requested speed
        while(Serial3.available() > 0){
          char temporary = Serial3.read(); 
          client += temporary;
        }
        currentState = PROCESS_CLIENT;
        break;
      }
      else{
        currentState = READ_OBD;
        speed_from_obd = "";
        client = "";
        break;
      }

////////////////////////////////////////////////////////////////////////////////

    case PROCESS_CLIENT:
      if (client.indexOf("010D") >= 0){
        Serial3.print(speed_from_obd); // 6, If yes send speed to client
      }
      speed_from_obd = "";
      client = "";
      currentState = READ_OBD;
      break;
  }
}