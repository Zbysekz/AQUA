void ICACHE_FLASH_ATTR setup()
{

  Serial.begin(9600); // pro pripadne ladici vystupy
  Serial.println(F("Starting................................"));

  /////////////////////////////////
  pinMode(ONBOARDLED, OUTPUT);//led
  pinMode(4, OUTPUT);//ch1
  pinMode(5, OUTPUT);//ch2

  analogWrite(4, outVal);
  analogWrite(5, outVal);
  
  lastStamp = millis(); //timer init
  EEPROM.begin(512);
  delay(10);

  readFromEEPROM();
  Serial.println("IP:");
  Serial.println(ip.toString());
  Serial.println("GATEWAY:");
  Serial.println(gateway.toString());
  Serial.println("SUBNET:");
  Serial.println(subnet.toString());
  //nejdrive zkousime pripojit k wifine
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, gateway, subnet);
  WiFi.begin(esid.c_str(), epass.c_str());
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("First attempt of connection failed!");
    delay(5000);
    //ESP.restart();
    WiFi.begin(esid.c_str(), epass.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {//druhy pokus
      Serial.println("Connection Failed! Creating own wifi...");
      delay(5000);

      joinOK = false;
      //nezdarilo se tak vytvarime vlastni testovaci
      WiFi.persistent(false);
      WiFi.mode(WIFI_AP);
      WiFi.softAP(SETUP_SSID);
    }
  }
  Serial.println("Got IP:");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  
  ArduinoOTA.setHostname("APOTA");
  ArduinoOTA.onStart(otaStart);
  ArduinoOTA.onEnd(otaEnd);
  ArduinoOTA.onProgress(otaProgress);
  ArduinoOTA.onError(otaError);
  ArduinoOTA.begin();
  Serial.println(F("Setup A end."));

  webserver.on("/", handleRoot);

  webserver.on("/params", []() {
    webserver.send ( 2500, "text/html", "{\"Success\":\"params saved to eeprom... go back in browser\"}");

    parPower1= webserver.arg("parPower1").toInt();
    parPower2= webserver.arg("parPower2").toInt();
    parPower3= webserver.arg("parPower3").toInt();
    
    parTime1.Hour= webserver.arg("parTime1_h").toInt();
    parTime1.Minute= webserver.arg("parTime1_min").toInt();
    parTime1.Second=0;

    parTime2.Hour= webserver.arg("parTime2_h").toInt();
    parTime2.Minute= webserver.arg("parTime2_min").toInt();
    parTime2.Second=0;

    parTime3.Hour= webserver.arg("parTime3_h").toInt();
    parTime3.Minute= webserver.arg("parTime3_min").toInt();
    parTime3.Second=0;

    parTime4.Hour= webserver.arg("parTime4_h").toInt();
    parTime4.Minute= webserver.arg("parTime4_min").toInt();
    parTime4.Second=0;

    parTime5.Hour= webserver.arg("parTime5_h").toInt();
    parTime5.Minute= webserver.arg("parTime5_min").toInt();
    parTime5.Second=0;

    parTime6.Hour= webserver.arg("parTime6_h").toInt();
    parTime6.Minute= webserver.arg("parTime6_min").toInt();
    parTime6.Second=0;
    
    Serial.println("Writing parPower");

    EEPROM.write(108, parPower1/10);
    EEPROM.write(109, parPower2/10);
    EEPROM.write(110, parPower3/10);
    EEPROM.write(111, parTime1.Hour);
    EEPROM.write(112, parTime1.Minute);
    EEPROM.write(113, parTime2.Hour);
    EEPROM.write(114, parTime2.Minute);
    EEPROM.write(115, parTime3.Hour);
    EEPROM.write(116, parTime3.Minute);
    EEPROM.write(117, parTime4.Hour);
    EEPROM.write(118, parTime4.Minute);
    EEPROM.write(119, parTime5.Hour);
    EEPROM.write(120, parTime5.Minute);
    EEPROM.write(121, parTime6.Hour);
    EEPROM.write(122, parTime6.Minute);
    
    EEPROM.commit();
  });
  webserver.on("/wifisettings", []() {
    webserver.send ( 200, "text/html", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
    esid = webserver.arg("ssid");
    epass = webserver.arg("pass");
    ip.fromString(webserver.arg("ip"));
    gateway.fromString(webserver.arg("gateway"));
    subnet.fromString(webserver.arg("subnet"));
    writeToEEPROM();
  });
  webserver.on("/submit", SubmitHandler);

  webserver.onNotFound(handleNotFound);

  webserver.begin();
  Serial.println("HTTP server started");

  ///////////////////////////////////////////////////NTP

    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });

    SendLogToAdafruit("ESP started");

}
