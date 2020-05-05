#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "configuration.h"
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP8266HTTPClient.h>

#define ONBOARDLED 2 // Built in LED on ESP-12/ESP-07
#define NTP_TIMEOUT 1500

char body[2500];
bool joinOK = true;//pokud je ve vlastni SETUP siti nebo pripojen k pracovni
IPAddress ip(10, 10, 2, 121);
IPAddress gateway(10, 10, 2, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer webserver(80);
WiFiClient client;

HTTPClient http;    //Declare object of class HTTPClient


const unsigned long BAUD_RATE = 9600;                 // serial connection speed
bool automatic = true;
int outVal = 1023, gradient =1, phase = 0,diff;
unsigned long lastStamp;
String esid = JOIN_SSID, epass = JOIN_PSWD;
unsigned long uptimein10sec=0;
unsigned long tmrPrint;

tmElements_t parTime1,parTime2,parTime3,parTime4,parTime5,parTime6;
int parPower1,parPower2,parPower3;

bool NTPsync,setupNTP;

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event
bool wifiFirstConnected= true;

void otaError(ota_error_t error)
{
  Serial.println(F("OTA Error."));
  switch (error) {
    case OTA_AUTH_ERROR:
      Serial.println("1"); break;
    case OTA_BEGIN_ERROR:
      Serial.println("2"); break;
    case OTA_CONNECT_ERROR:
      Serial.println("3"); break;
    case OTA_RECEIVE_ERROR:
      Serial.println("4"); break;
    case OTA_END_ERROR:
      Serial.println("5"); break;
  }
  Serial.println("6");
}

void otaProgress(unsigned int actual, unsigned int maxsize)
{
  if (actual == maxsize - (maxsize / 10)) {
    Serial.println("OTA progress: 90%");
  }
  if (actual == maxsize) {
    Serial.println("OTA progress: 100%");
  }
}

void otaStart()
{
  Serial.println(F("Start."));
}

void otaEnd()
{
  Serial.println(F("End."));
}





// EOF
