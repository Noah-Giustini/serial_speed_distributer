#define DEBUG 0
#define TIMING 0

String speed_from_obd = "";

static String clientSpeed = "";
static String serialSpeed = "";

static String clientBuf = "";
static String serialBuf = "";


static unsigned long lastTime1; 
static unsigned long lastTime2;


HardwareSerial OBD(1);
HardwareSerial Client(2);

void setup() {
  Serial.begin(115200);
  OBD.begin(115200, SERIAL_8N1, 25, 26); //RX, TX
  Client.begin(115200, SERIAL_8N1, 16, 17); //RX, TX

  Serial.setTimeout(1);
  OBD.setTimeout(100);
  Client.setTimeout(1);

  OBD.write("ATZ\r\n"); //Initialize

  #if DEBUG
  delay(100); while (OBD.available()){String temp = OBD.readStringUntil('\n'); speed_from_obd = temp;} Serial.println("Initialization message: " + speed_from_obd);
  #endif

  delay(3000);
}

void loop() {
  while (Client.available()){
    char c = Client.read();
    clientBuf += c;

    if (clientBuf == "0" && c != '1'){
      Serial.printf("DEBUG1: %s, %c\n", clientBuf, c);
    }
    else if (clientBuf == "01" && c != '0'){
      Serial.printf("DEBUG1: %s, %c\n", clientBuf, c);
    }
    else if (clientBuf == "010" && c != 'D'){
      Serial.printf("DEBUG1: %s, %c\n", clientBuf, c);
    }
/*
    if (c != '0' && c != '1' && c != 'D' && c != '\r' && c != '\n'){
      Serial.printf("Unknown char1: %02x\n", c);
    }
*/
    if (clientBuf.indexOf("010D") >= 0){
      clientSpeed = getSpeed();
      Client.print(clientSpeed);

      clientBuf = "";
      break;
    }

    if (clientBuf.length() > 32) clientBuf = "";
  }

//////////////////////////////////////////////////////////////////////

  while (Serial.available()){
    char c = Serial.read();
    serialBuf += c;

    if (serialBuf == "0" && c != '1'){
      Serial.printf("DEBUG2: %s, %c\n", serialBuf, c);
    }
    else if (serialBuf == "01" && c != '0'){
      Serial.printf("DEBUG2: %s, %c\n", serialBuf, c);
    }
    else if (serialBuf == "010" && c != 'D'){
      Serial.printf("DEBUG2 : %s, %c\n", serialBuf, c);
    }
/*
    if (c != '0' && c != '1' && c != 'D' && c != '\r' && c != '\n'){
      Serial.printf("Unknown char2: %02x\n", c);
    }
*/
    if (serialBuf.indexOf("010D") >= 0){
      serialSpeed = getSpeed();
      Serial.print(serialSpeed);

      serialBuf = "";
      break;
    }

    if (serialBuf.length() > 32) serialBuf = "";
  }
}

String getSpeed(){
  String currentSpeed;
  while (OBD.available()) {
    OBD.read();
  }
  OBD.write("010D\r\n");

  unsigned long lastTime = millis();
  while (millis() - lastTime < 100){
    if (OBD.available()){
      currentSpeed = OBD.readStringUntil('\n');
    }
  }
  return currentSpeed;
}