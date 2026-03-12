String speed_from_obd;
String client;

void setup() {
  //Initialize
  Serial.begin(115200); // Serial with OBDII
  Serial3.begin(115200); // Serial client

  Serial.write("ATZ\r\n"); // 1, Transmitting initializaiton
  delay(100);

  while (Serial.available() > 0){
    char initialize = Serial.read(); // 2, Receiving initialization message
    Serial.print(initialize);
  }
  Serial.print("------------------------\n");
  delay(3000);
}

void loop() {
  Serial.print("To OBD: ");
  Serial.write("010D\r\n"); // 3, Transmitting request speed to OBDII
  delay(100);

  while (Serial.available() > 0){
    char temporary = Serial.read(); // 4, Receiving requested speed from OBDII
    speed_from_obd += temporary;
  }
  Serial.print("From OBD: ");
  Serial.println(speed_from_obd);

  delay(1000);


////////////////////////////////////////////////////////////////////////////////


  Serial.println("Checking..."); // 5, Check if client requested speed
  if (Serial3.available()){
    while(Serial3.available() > 0){
    char temporary = Serial3.read(); 
    client += temporary;
    }

    Serial.print("Message from client: ");
    Serial.println(client);

    if (client == "010D"){
      Serial.println("Sending speed\n");
      Serial3.print(speed_from_obd); // 6, If yes send speed to client
      }
    else{
      Serial.println("Garbage\n");
      }
  }

  else{
    Serial.println("Not available\n");
  }


  speed_from_obd = "";
  client = "";
  delay(1000); // 7, loop
}

