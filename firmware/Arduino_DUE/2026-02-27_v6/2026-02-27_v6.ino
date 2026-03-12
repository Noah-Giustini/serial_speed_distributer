#define DEBUG 0

String speed_from_obd;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); // Client 1
  Serial2.begin(115200); // Client 2
  Serial3.begin(115200); // OBD

  Serial1.setTimeout(5);
  Serial2.setTimeout(5);
  Serial3.setTimeout(100);

  Serial3.write("ATZ\r\n"); // Initialization
  delay(100);

  #if DEBUG
  if (Serial3.available()){
    String initialMessage = Serial3.readString();
    Serial.print("Initialization message: " +initialMessage);
  }
  #endif
  delay(3000);
}

void loop() {
  if (Serial1.available()){ // Client 1
    String client1 = Serial1.readStringUntil('>');
    if (client1.indexOf("010D") >= 0) {
      speed_from_obd = getSpeed();
      Serial1.print(speed_from_obd);

      #if DEBUG
      Serial.println("Speed 1: " +speed_from_obd);
      #endif
    }
  } 

  if (Serial2.available()){ // Client 2
    String client2 = Serial2.readStringUntil('>');
    if (client2.indexOf("010D") >= 0) {
      speed_from_obd = getSpeed();
      Serial2.print(speed_from_obd);

      #if DEBUG
      Serial.println("Speed 2: " +speed_from_obd);
      #endif
    }
  }
  delay(500);
}

String getSpeed(){
  String currentSpeed;
  while (Serial3.available()) {
    Serial3.read();
  }
  Serial3.write("010D\r\n");

  unsigned long lastTime = millis();
  while (millis() - lastTime < 100) {
    if (Serial3.available()){
      currentSpeed = Serial3.readStringUntil('>');

      #if DEBUG
      Serial.print("From OBD: " +currentSpeed);
      #endif
    }
  }
  return currentSpeed;
}