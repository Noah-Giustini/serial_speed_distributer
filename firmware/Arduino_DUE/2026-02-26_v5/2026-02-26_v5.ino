#define DEBUG 0

String speed_from_obd;
String client1;
String client2;

unsigned long lastOBDRequest = 0;
const unsigned long obdInterval = 1000;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); // Client 1
  Serial2.begin(115200); // Client 2
  Serial3.begin(115200); // OBD

  Serial1.setTimeout(5);
  Serial2.setTimeout(5);
  Serial3.setTimeout(5);

  Serial3.write("ATZ\r\n"); // Initialization
  delay(3000);
}

void loop() {
  if (millis() - lastOBDRequest >= obdInterval) {
    lastOBDRequest = millis();
    speed_from_obd = getSpeed();

    #if DEBUG
    Serial.print("Updated Speed: ");
    Serial.println(speed_from_obd);
    #endif
  }

  if (Serial1.available()){ // Client 1
    client1 = Serial1.readStringUntil('>');

    if (client1.indexOf("010D") >= 0) {
      Serial1.print(speed_from_obd);
    }
  } 

  if (Serial2.available()){ // Client 2
    client2 = Serial2.readStringUntil('>');

    if (client2.indexOf("010D") >= 0) {
      Serial2.print(speed_from_obd);
    }
  }
}

String getSpeed(){
  while (Serial3.available()) {
    Serial3.read();
  }

  Serial3.write("010D\r\n");

  String response = "";
  unsigned long startTime = millis();

  while (millis() - startTime < 200) {
    while (Serial3.available()) {
      char c = Serial3.read();
      if (c == '>') {
        return response;
      }
      response += c;
    }
  }
  return response;
}