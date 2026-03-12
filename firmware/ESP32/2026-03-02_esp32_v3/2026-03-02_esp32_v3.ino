#include <mutex>

#define TIMING 0

String speed_from_obd = "";
static unsigned long obdTime;
static unsigned long lastTime1;
static unsigned long lastTime2;


std::mutex serial_mtx;

HardwareSerial OBD(1);
HardwareSerial Client(2);

void setup() {
  Serial.begin(115200);
  OBD.begin(115200, SERIAL_8N1, 25, 26); //RX, TX
  Client.begin(115200, SERIAL_8N1, 16, 17); //RX, TX

  Serial.setTimeout(1);
  OBD.setTimeout(100);
  Client.setTimeout(1);

  xTaskCreatePinnedToCore(Handle_OBD, "Handle_OBD", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(Handle_Client, "Handle_Client", 2048, NULL, 1, NULL, 1);

  OBD.write("ATZ\r\n"); //Initialize
  delay(100);

  while (OBD.available()){
    String temp = OBD.readStringUntil('>');
    speed_from_obd = temp;
  }

  //Serial.println("Initialization message: " +speed_from_obd);
  delay(3000);
}

void loop() {
}

void Handle_OBD(void *pvParameters){
  while(true){
    String tempSpeed;
    while (OBD.available()){
      OBD.read();
    }
    OBD.write("010D\r\n");

    unsigned long lastTime = millis();
    while (millis() - lastTime < 100){
      if (OBD.available()){
        tempSpeed = OBD.readStringUntil('\n');
        #if TIMING
        Serial.print("OBD, time dif: ");
        Serial.println(millis() - obdTime);
        obdTime = millis();
        #endif
        break;
      }
    }
    {
    std::lock_guard<std::mutex> lck(serial_mtx);
    speed_from_obd = tempSpeed;
    //Serial.println("Speed from OBD: " +speed_from_obd);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void Handle_Client(void *pvParameters){
  while(true){
    if (Client.available()){
      if (Client.find("010D") >= 0){
        //Serial.println("Client message: " +client);
        {
        std::lock_guard<std::mutex> lck(serial_mtx);
        Client.print(speed_from_obd);
        //Serial.println("Speed Received: " +speed_from_obd);
        }
        #if TIMING
        Serial.print("1one, time dif: ");
        Serial.println(millis() - lastTime1);
        lastTime1 = millis();
        #endif
      }
    }
/////////////////////////////////////////////////////////////////// Extra client here
    if (Serial.available()){
      if (Serial.find("010D") >= 0){
        //Serial.println("Client message: " +client);
        {
        std::lock_guard<std::mutex> lck(serial_mtx);
        Serial.print(speed_from_obd);
        //Serial.println("Speed Received: " +speed_from_obd);
        }
        #if TIMING
        Serial.print("2two, time dif: ");
        Serial.println(millis() - lastTime2);
        lastTime2 = millis();
        #endif  
      }
    }
///////////////////////////////////////////////////////////////////
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}
