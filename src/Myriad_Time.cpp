#include <Myriad_Time.h>
#include <time.h>

Myriad_Time::Myriad_Time(){
  // Timeserver values
  timezone = -7; 
  daysavetime = 1;
  sleepstarthour;
  sleepstartminute;
  sleependhour;
  sleependminute;
  sleeping;
  struct tm tmstruct;
  initialize();
}

void Myriad_Time::initialize(){
  configTime(timezone*3600, daysavetime*3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
}

void Myriad_Time::gettime(){               
  EVERY_N_SECONDS(60){      // gets the time from the internet
    getLocalTime(&tmstruct);
    Serial.println(&tmstruct, "%A, %B %d %Y %H:%M:%S");
    //Serial.printf("%d-%02d-%02d %02d:%02d:%02d",(tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
  }
}

void Myriad_Time::sleepcheck(){
  /*
  if sleephour
  
  start is higher than end, then add a day

  else if start is lower than end, just simple compare

  if(tmstruct.tm_hour && tmstruct.tm_min){
    TimeSpan()
  }
  String(mtime.tmstruct.tm_hour) + ":" + String(mtime.tmstruct.tm_min)
  */
}

/*
// Start making a big string to send the time
String year = String(tmstruct.tm_year+1900);
String month = String(tmstruct.tm_mon);
if(tmstruct.tm_mon < 10){         // Format it to show 0s since you cant use printf
  month = String("0"+ month);
}
String day = String(tmstruct.tm_mday);        
if(tmstruct.tm_mday < 10){
  day = String("0"+day);
}
String hr = String(tmstruct.tm_hour);
if(tmstruct.tm_hour < 10){
  hr = String("0"+ hr);
}
String mini = String(tmstruct.tm_min);
if(tmstruct.tm_min < 10){
  mini = String("0"+ mini);
}
String sec = String(tmstruct.tm_sec);
if(tmstruct.tm_sec < 10){
  sec = String("0"+ sec);
}
String date = String("t," + year + "-" + month+ "-" + day+ " " + hr + ":" + mini + ":" + sec);   // concatenate a big string with the time
//"%d-%02d-%02d %02d:%02d:%02d",(tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec";
//String looops = String((tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
//Serial.println(date);  

String zonesync = String("u," + String(timezone));   // also send the timezone to the webpage
*/