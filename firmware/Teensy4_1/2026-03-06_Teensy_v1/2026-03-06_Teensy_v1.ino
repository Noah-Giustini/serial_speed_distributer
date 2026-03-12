#define DEBUG 0

String speed_from_obd = "";

static String client1Speed = "";
static String client2Speed = "";

static String client1Req = "";
static String client2Req = "";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); // Client 1
  Serial2.begin(115200); // Client 2
  Serial3.begin(115200); // OBD

  Serial1.setTimeout(1); // Client 1
  Serial2.setTimeout(1); // Client 2
  Serial3.setTimeout(50); // OBD

  pinMode(13, OUTPUT); // LED on Teensy
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);

  Serial3.write("ATZ\r\n"); // Initialize

  #if DEBUG
  delay(100); 
  while (Serial3.available()){
    String temp = Serial3.readStringUntil('\n'); 
    speed_from_obd = temp;
  } 
  Serial.println("Initialization message: " + speed_from_obd);
  #endif

  delay(3000);
}

void loop() {
  while (Serial1.available()){ // Client 1
    char c = Serial1.read();
    client1Req += c;

    if (client1Req.indexOf("010D") >= 0){
      client1Speed = getSpeed();
      Serial1.print(client1Speed);

      client1Req = "";
      break;
    }
    if (client1Req.length() > 32) {
      client1Req = "";
    }
  }
  //////////////////////////////////////////////////////////////
  while (Serial2.available()){ // Client 2
    char c = Serial2.read();
    client2Req += c;

    if (client2Req.indexOf("010D") >= 0){
      client2Speed = getSpeed();
      Serial2.print(client2Speed);

      client2Req = "";
      break;
    }
    if (client2Req.length() > 32) {
      client2Req = "";
    }
  }
  //////////////////////////////////////////////////////////////
}

String getSpeed(){
  String currentSpeed;
  while (Serial3.available()){
    Serial3.read();
  }
  Serial3.write("010D\r\n");

  unsigned long lastTime = millis();
  while (millis() - lastTime < 50){
    if (Serial3.available()){
      currentSpeed = Serial3.readStringUntil('\n');
    }
  }
  return currentSpeed;
}