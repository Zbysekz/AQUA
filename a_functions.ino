
void readFromEEPROM() {
  Serial.println("Reading EEPROM ssid");
  esid = "";
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  for (int i = 96; i < 100; ++i)//IPaddress
  {
    ip[i - 96] = char(EEPROM.read(i));
  }
  for (int i = 100; i < 104; ++i)//gateway
  {
    gateway[i - 100] = char(EEPROM.read(i));
  }
  for (int i = 104; i < 108; ++i)//subnet
  {
    subnet[i - 104] = char(EEPROM.read(i));
  }

  //if the datas are not valid
  if (ip[0] == 255 || ip[0] == 0 || subnet[0] == 0 || subnet[3] == 255) {
    ip[0] = 192; ip[1] = 168; ip[2] = 4; ip[3] = 1;
    subnet[0] = 255; subnet[1] = 255; subnet[2] = 255; subnet[3] = 0;
  }

  parPower1 = char(EEPROM.read(108))*10;
  parPower2 = char(EEPROM.read(109))*10;
  parPower3 = char(EEPROM.read(110))*10;
  parTime1.Hour = char(EEPROM.read(111));
  parTime1.Minute = 0;//char(EEPROM.read(112));
  parTime2.Hour = char(EEPROM.read(113));
  parTime2.Minute = 0;//char(EEPROM.read(114));
  parTime3.Hour = char(EEPROM.read(115));
  parTime3.Minute = 0;//char(EEPROM.read(116));
  parTime4.Hour = char(EEPROM.read(117));
  parTime4.Minute = 0;//char(EEPROM.read(118));
  parTime5.Hour = char(EEPROM.read(119));
  parTime5.Minute = 0;//char(EEPROM.read(120));
  parTime6.Hour = char(EEPROM.read(121));
  parTime6.Minute = 0;//char(EEPROM.read(122));
  

  Serial.println("Read parPower and parTime");
}

void writeToEEPROM() {
  if (esid.length() > 0 && epass.length() > 0) {
    Serial.println("clearing eeprom");
    for (int i = 0; i < 96; ++i) {
      EEPROM.write(i, 0);
    }
    Serial.println(esid);
    Serial.println("");
    Serial.println(epass);
    Serial.println("");

    Serial.println("writing eeprom ssid:");
    for (int i = 0; i < esid.length(); ++i)
    {
      EEPROM.write(i, esid[i]);
      Serial.print("Wrote: ");
      Serial.println(esid[i]);
    }
    Serial.println("writing eeprom pass:");
    for (int i = 0; i < epass.length(); ++i)
    {
      EEPROM.write(32 + i, epass[i]);
      Serial.print("Wrote: ");
      Serial.println(epass[i]);
    }
    for (int i = 0; i < 4; ++i)//IPAddress
    {
      EEPROM.write(96 + i, ip[i]);
    }
    for (int i = 0; i < 4; ++i)//gateway
    {
      EEPROM.write(100 + i, gateway[i]);
    }
    for (int i = 0; i < 4; ++i)//subnet
    {
      EEPROM.write(104 + i, subnet[i]);
    }
    //parPower and parTime is on 108-122

    EEPROM.commit();
  }
}


void processSyncEvent (NTPSyncEvent_t e) {
    if (e < 0) {
        Serial.printf ("Time Sync error: %d\n", e);
        if (e == noResponse)
            Serial.println ("NTP server not reachable");
        else if (e == invalidAddress)
            Serial.println ("Invalid NTP server address");
        else if (e == errorSending)
            Serial.println ("Error sending request");
        else if (e == responseError)
            Serial.println ("NTP response error");
    } else {
        if (e == timeSyncd) {
            Serial.print ("Got NTP time: ");
            Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
            NTPsync = true;
        }
    }
}

bool timeElapsed(tmElements_t t) { //true pokud cas t uz uplynul
  if (hour() > t.Hour) {
    return true;
  }
  if (hour() < t.Hour)return false;
  if (hour() == t.Hour) {
    if (minute() > t.Minute) {
      return true;
    }
    if (minute() < t.Minute)return false;
    if (minute() == t.Minute) {
      if (second() > t.Second) {
        return true;
      }
      if (second() < t.Second)return false;
      if (second() == t.Second) {
        return true;
      }
    }
  }
  return false;
}


void SendLogToAdafruit(String str){

   http.begin(client,"http://io.adafruit.com/api/v2/Zbysekz/feeds/feed/data");      //Specify request destination

   http.addHeader("Content-Type", "application/json");

   http.addHeader("X-AIO-Key", "aio_ULqQ231RyQ84B5ANnYKi0TJp8Xuo");   

   String postData = "{\"value\":\"" + str+"\"}";
   int httpCode = http.POST(postData);   //Send the request

   String payload = http.getString();                  //Get the response payload

 
   Serial.println(postData);
   Serial.println(httpCode);   //Print HTTP return code
   if(httpCode<=0)
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    
   Serial.println(payload);    //Print request response payload

 

   http.end();  //Close connection
}
