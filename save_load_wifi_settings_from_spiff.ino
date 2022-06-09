#include <Arduino.h>

#include "FS.h"
#include "SPIFFS.h"

struct wifi_settings_t {
  char  SSID[32];
  char  Password[64];
};

const char* default_ssid = "default_ssid";
const char* default_password = "default_password";
bool default_format = true;
wifi_settings_t wifi_settings;
 
void save_to_file(fs::FS &fs, const char * fname, byte* memAddress, int datasize) {
  File f = fs.open(fname, FILE_WRITE);
  
  if (f) {
    f.seek(0, SeekSet);
    byte *p = memAddress;
    for (int x = 0; x < datasize ; x++) {
      f.write(*p);
      p++;
    }
    
    f.close();    
  }
}

bool load_from_file(fs::FS &fs, const char * fname, byte* memAddress, int datasize) {
  File f = fs.open(fname);
  
  if (f && f.size() > 0) {    
    f.seek(0, SeekSet);
    byte *p = memAddress;
    for (int x = 0; x < datasize; x++) {
      *p = f.read();
      p++;
    }
    f.close();
    return true;    
  }
  return false;
}
 
void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  } else {
     Serial.println("SPIFFS Mount successful");  
  }

  if(default_format) {
    Serial.println("Formating SPIFFS..");  
     
    if(SPIFFS.format()){
      Serial.println("SPIFFS Success!");
    }else{
      Serial.println("SPIFFS Error!");
    }
  } 
  
  memset (&wifi_settings, 0, sizeof(wifi_settings));
  
  bool success = load_from_file(SPIFFS, "/wificonfig.dat", (byte*)&wifi_settings, sizeof(wifi_settings));
  Serial.printf("Load success ? %s\n", success ? "Yes" : "No. First time?");
  
  if(!success) {
     Serial.println("Save default wifi login!");
     
     strcpy(wifi_settings.SSID, default_ssid);
     strcpy(wifi_settings.Password, default_password);
     
     save_to_file(SPIFFS, "/wificonfig.dat", (byte*)&wifi_settings, sizeof(wifi_settings));
  }

  Serial.println(wifi_settings.SSID);
  Serial.println(wifi_settings.Password);   
}

void loop() {
  // put your main code here, to run repeatedly:

}
