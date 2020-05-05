
void loop()
{
  ArduinoOTA.handle();
  webserver.handleClient();


  if((!joinOK || !NTPsync) && uptimein10sec > 60){// kdyz se po zapnuti nedokazal spojit s netem nebo s NTP tak jen vytvori svoji sit a za 10 minut se restartuj 
    ESP.restart();
  }

  if (joinOK && !setupNTP) {
        Serial.println("NTP client started");
        setupNTP = true;
        NTP.setInterval (7200);//sec
        NTP.setNTPTimeout (NTP_TIMEOUT);
        NTP.begin ("pool.ntp.org", 0, true, 0);
    }
  
  if (syncEventTriggered) {
      processSyncEvent (ntpEvent);
      syncEventTriggered = false;

      if(NTPsync)
        NTP.stop();//stop syncing
  }

  if(!NTPsync){
    return;
  }
  
   switch (phase) {
    case 0://tma
      //outVal = 1023;
      gradient=20;//4 je za hodinu
      
      if (timeElapsed(parTime1)&& !timeElapsed(parTime6)) {
        phase = 1;
        Serial.println("TO PHASE 1");
      }
    break;
    case 1://sviti na maly vykon
      gradient=20;//4 je za hodinu
      if (timeElapsed(parTime2)) {
        Serial.println("TO PHASE 2");
        phase = 2;
      }
    break;
    case 2://sviti na velky vykon

      //gradient rozsviceni na velky vykon
      diff = (parTime3.Hour*60+parTime3.Minute - parTime2.Hour*60+parTime2.Minute); // 6:30   7:15
      if(diff<=0)diff=1;
      gradient = 1023 / (diff * 6) +1; 
 
      if (timeElapsed(parTime4)) {
        Serial.println("TO PHASE 3");
        phase = 3;
      }
    break;
    case 3://sviti na maly vykon

      //gradient stmivani z velkeho vykonu
      diff = (parTime5.Hour*60+parTime5.Minute - parTime4.Hour*60+parTime4.Minute);
      if(diff<=0)diff=1;
      gradient = 1023 / (diff * 6) +1; 
    
      if (timeElapsed(parTime6)) {
        Serial.println("TO PHASE 0");
        phase = 0;
      }
    break;
   }
   
  if (automatic) { // jen pokud se podarilo spojeni k netu
    if(joinOK && NTPsync){
      analogWrite(4, outVal);
      analogWrite(5, outVal);
    }else{
      analogWrite(4, 1023);
      analogWrite(5, 1023);
    }
  }

  /*if (millis() > tmrPrint + 3600000L){//each 1h
    tmrPrint = millis();
    Serial.print("Time:");
    Serial.print(hour());
    Serial.print(":");
    Serial.println(minute());
    
  }*/
 
  
  //10sec timer
  if (millis() < lastStamp)lastStamp = millis(); //prevent overflow issue
  if (millis() > lastStamp + 10000) {
    automatic=true;
    lastStamp = millis();

    if (gradient <= 0)gradient = 1;
    if (gradient > 100)gradient = 100;

    if (phase == 1){
      if(outVal>parPower1)
        outVal -= gradient;//rozsviceni
      else
         outVal = parPower1;
    }else if (phase == 2){
      if(outVal>parPower2)
        outVal -= gradient;//rozsviceni na velky
        else
         outVal = parPower2;
    }else if (phase == 3){
      if(outVal<parPower3)
        outVal += gradient;
        else
         outVal = parPower3;
    }else if (phase == 0){//stmivani na tmu
      if(outVal<1023)
        outVal += gradient;
        else
         outVal = 1023;
    }
      
    if (outVal < 0)outVal = 0;//plny vykon
    if (outVal > 1023)outVal = 1023;//tma


    uptimein10sec++;
  }

}
