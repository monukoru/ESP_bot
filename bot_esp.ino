#include <WiFi.h>
#include <SPI.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "ESP_race_bot"
#define REMOTEXY_WIFI_PASSWORD "lewis_hamilton_7"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_MODE__WIFI_POINT

#define SS_PIN 5

#pragma pack(push, 1)  
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] =   
{ 255,4,0,0,0,53,0,19,0,0,0,0,31,1,106,200,1,1,4,0,
  1,50,12,24,24,0,2,8,62,0,1,9,13,24,24,0,2,24,60,0,
  1,30,110,24,24,0,2,24,94,0,1,30,151,24,24,0,2,24,118,0 };

struct {
  uint8_t button_01;
  uint8_t button_02;
  uint8_t button_03;
  uint8_t button_04;
  uint8_t connect_flag;
} RemoteXY;   
#pragma pack(pop)


void setup() 
{
  RemoteXY_Init();

  SPI.begin(18, 19, 23, SS_PIN);

  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void loop() 
{ 
  RemoteXYEngine.handler();   

  uint8_t controlByte = 0;

  if(RemoteXY.button_01) controlByte |= (1 << 0);
  if(RemoteXY.button_02) controlByte |= (1 << 1);
  if(RemoteXY.button_03) controlByte |= (1 << 2);
  if(RemoteXY.button_04) controlByte |= (1 << 3);

  digitalWrite(SS_PIN, LOW);
  SPI.transfer(controlByte);
  digitalWrite(SS_PIN, HIGH);

  RemoteXYEngine.delay(20);
}