/*
This code compatible for ESP32
Test run with ThaiEasyElec's ESPino32
Coding : J.Isaranu | IoTtweet.com
*/

#include <WiFi.h>
#include <WiFiUdp.h>

const char * ssid = "your-ssid";
const char * passw = "your-passw";

const char * udpAddress = "IP address"; // Google Cloud Server IP
const int udpPort = 41234; // Google Cloud UDP opened port, need open firewall in console before use

bool conn = false;

WiFiUDP udp;

void setup(){
  Serial.begin(115200);

  WiFi.disconnect(true);
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(ssid, passw);

  Serial.println("Waiting for WIFI connect...");
  delay(3000);
}

void loop(){
  
  if(conn){
    uint8_t testRand = random(20,30);
    udp.beginPacket(udpAddress,udpPort);
    udp.printf("%u", testRand);
    udp.endPacket();
  }

  delay(1000);
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          udp.begin(WiFi.localIP(),udpPort);
          conn = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          conn = false;
          break;
    }
}
