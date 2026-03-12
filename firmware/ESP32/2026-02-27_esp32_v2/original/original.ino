#include <SoftwareSerial.h>

String speed_from_obd = "";

std::mutex serial_mtx;

HardwareSerial OBD(1);
HardwareSerial Client(2);

void setup() {
  Serial.begin(115200);
  OBD.begin(115200, SERIAL_8N1, 25, 26); //RX, TX
  Client.begin(115200, SERIAL_8N1, 16, 17); //RX, TX

  OBD.setTimeout(100);
  Client.setTimeout(50);

  OBD.write("ATZ\r\n"); //Initialize
  delay(100);

  while (OBD.available()){
    String temp = OBD.readStringUntil('>');
    speed_from_obd = temp;
  }

  //Serial.println("Initialization message: " +speed_from_obd);
  delay(3000);

  xTaskCreatePinnedToCore(Handle_OBD, "Handle_OBD", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(Handle_Client, "Handle_Client", 2048, NULL, 1, NULL, 1);
}

void loop() {
}

void Handle_OBD(void *pvParameters){
  while(true){
    while (OBD.available()){
      OBD.read();
    }
    OBD.write("010D\r\n");

    unsigned long lastTime = millis();
    while (millis() - lastTime < 100){
      if (OBD.available()){
        String speed = OBD.readStringUntil('>');
        {
          std::lock_guard<std::mutex> lck(serial_mtx);
          speed_from_obd = speed;

          //Serial.println("Speed from OBD: " +speed_from_obd);
        }
        break;
      }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void Handle_Client(void *pvParameters){
  while(true){
    if (Client.available()){
      String client = Client.readStringUntil('>');
      //Serial.println("Client message: " +client);
      if (client.indexOf("010D") >= 0){
          {
          std::lock_guard<std::mutex> lck(serial_mtx);
          Client.print(speed_from_obd);
          //Serial.println("Speed Received: " +speed_from_obd);
          }
      }
    }
/////////////////////////////////////////////////////////////////// Extra client here
    if (Serial.available()){
      String extraclient = Serial.readStringUntil('>');
      //Serial.println("Client message: " +client);
      if (extraclient.indexOf("010D") >= 0){
          {
          std::lock_guard<std::mutex> lck(serial_mtx);
          Serial.print(speed_from_obd);
          //Serial.println("Speed Received: " +speed_from_obd);
          }
      }
    }
///////////////////////////////////////////////////////////////////
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
