#define DEBUG 0
#define TIMING 0

String speed_from_obd = "";

static String clientSpeed = "";
static String serialSpeed = "";

static String clientBuf = "";
static String serialBuf = "";

static unsigned long lastClientTime;
static unsigned long lastSerialTime;
static unsigned long lastInterval;

#if TIMING
static unsigned long lastTime1;
static unsigned long lastTime2;
static unsigned long lastTime3;
#endif


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
  delay(100);
  while (OBD.available()){
    String temp = OBD.readStringUntil('\n');
    speed_from_obd = temp;
  }
  Serial.println("Initialization message: " +speed_from_obd);
  #endif

  delay(3000);
}

void loop() {

  lastClientTime = millis();

  while(millis() - lastClientTime < 100){
    if (Client.available()){
      char c = Client.read();
      clientBuf += c;

      if (clientBuf.indexOf("010D") >= 0){
        clientSpeed = getSpeed();
        Client.print(clientSpeed);

        #if TIMING
        Serial.print("1one, time dif: "); if ((millis() - lastTime1) >= 1500){Serial.println("Client Buffer: " +clientBuf);} else {Serial.println(millis() - lastTime1);} lastTime1 = millis();
        #endif

        clientBuf = "";
        break;
      }
    if (clientBuf.length() > 32) clientBuf = "";  
    }
  }

/////////////////////////////////////////////////////////////////
  lastSerialTime = millis();

  while(millis() - lastSerialTime < 100){
    if (Serial.available()){
      char c = Serial.read();
      serialBuf += c;

      if (serialBuf.indexOf("010D") >= 0){
        serialSpeed = getSpeed();
        Serial.print(serialSpeed);

        #if TIMING
        Serial.print("2two, time dif: "); if ((millis() - lastTime2) >= 1500){Serial.println("Serial Buffer: " +serialBuf);} else {Serial.println(millis() - lastTime2);} lastTime2 = millis();
        #endif

        serialBuf = "";
        break;
      }
    if (serialBuf.length() > 32) serialBuf = "";  
    }
  }
}


String getSpeed(){
  String currentSpeed;
  while (OBD.available()) {
    OBD.read();
  }
  OBD.write("010D\r\n");

  unsigned long lastTime = millis();
  while (millis() - lastTime < 100) {
    if (OBD.available()){
      #if TIMING
      lastTime3 = millis();
      #endif

      currentSpeed = OBD.readStringUntil('\n');

      #if TIMING
      Serial.print("Speed read time: ");
      Serial.println(millis() - lastTime3);
      #endif
    }
  }
  return currentSpeed;
}