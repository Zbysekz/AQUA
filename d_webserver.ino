
void handleRoot() {
  unsigned long sec = millis() / 1000;
 
  char timestr [20];
  NTP.getTimeStr().toCharArray(timestr, 20);

  snprintf ( body, 2500,
             "<html>\
  <head>\
    <title>ESP8266 - AQUA</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <p> v1.2\
    <p>Uptime: %lu seconds , %lu hours </p>\
    <p>UTC time now:%s\
    <p>value 1023 = 0%% power ; value 0 = 100%% power \
    <p>parPower1,parPower2,parPower3: %d ; %d ; %d</p>\
    <p>parTime1 %d; parTime2 %d ; parTime3 %d ; parTime4 %d; parTime5 %d ; parTime6 %d </p>\
    <p>Actual gradient:%d</p>\
    <p>Act.out:%d   phase:%d</p>\
  </body>\
  </p><form method='get' action='params'><label>parPower1: </label><input name='parPower1' length=3><label>parPower2: </label><input name='parPower2' length=3><label>parPower3: </label><input name='parPower3' length=3>\
  </p><label>parTime1[h]: </label><input name='parTime1_h' length=3>\
  </p><label>parTime2[h]: </label><input name='parTime2_h' length=3>\
  </p><label>parTime3[h]: </label><input name='parTime3_h' length=3>\
  </p><label>parTime4[h]: </label><input name='parTime4_h' length=3>\
  </p><label>parTime5[h]: </label><input name='parTime5_h' length=3>\
  </p><label>parTime6[h]: </label><input name='parTime6_h' length=3>\
  </p><input type='submit'></form>\
  </p><form method='get' action='submit'><label>Channel1,2: </label><input name='ch1' length=32><input name='ch2' length=64><input type='submit'></form>\
  </p>\
  </p>\
  </p><form method='get' action='wifisettings'><label>SSID</label><input name='ssid' length=32><label>PASS</label><input name='pass' length=64>\
  </p><label>IP</label><input name='ip' length=20><label>GATEWAY</label><input name='gateway' length=20><label>SUBNET MASK</label><input name='subnet' length=20><input type='submit'></form>\
</html>",

             sec , uptimein10sec /360, timestr,parPower1,parPower2,parPower3,parTime1.Hour,parTime2.Hour,parTime3.Hour,parTime4.Hour,parTime5.Hour,parTime6.Hour,gradient, outVal, phase
           );

  webserver.send ( 2500, "text/html", body);

}/*

  <p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>\
  <form action='http://10.10.2.121/submit' method='POST'>\
    Channel1: <input type='text' name='ch1'><br>\
    Channel2: <input type='text' name='ch2'><br>\
    <input type='submit' value='Submit'>\
  </form>\
  <form action='http://10.10.2.121/submit' method='POST'>\
    Channel1: <input type='text' name='ch1'><br>\
    Channel2: <input type='text' name='ch2'><br>\
    <input type='submit' value='Submit'>\
  </form>\
*/
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i = 0; i < webserver.args(); i++) {
    message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
  }
  webserver.send(404, "text/plain", message);
}

void SubmitHandler() {
  automatic = false;
  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      if (webserver.argName(i) == "ch1") {
        analogWrite(4, webserver.arg(i).toInt());
      }
      if (webserver.argName(i) == "ch2") {
        analogWrite(5, webserver.arg(i).toInt());
      }
    }
  }
  webserver.send(200, "text/html", body);
}
