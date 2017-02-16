#include "ESP_WifiConfig.h"

bool ESP_WifiConfig::setWifi(String ssid, String password) {
    //Save credentials in EEPROM
    this->SSID_STR = ssid;
    this->PASSWORD = password;

    EEPROM.begin(this->EEPROM_INIT_ADDRESS);
    //Saving SSID
    for(int i = 0; i < ssid.length(); i++){
        EEPROM.write(i, ssid[i]);
    }
    //Saving password
    for(int i = 0; i < password.length(); i++){
        EEPROM.write(32+i, password[i]);
    }
    EEPROM.commit();
    EEPROM.end();

    //Initiate WIFI connectivity
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->SSID_STR.c_str(), this->PASSWORD.c_str());
    delay(1000);
    ESP.restart();
}

bool ESP_WifiConfig::startAccessPoint(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ESP_WIFICONFIG_ACCESS_POINT_USER, ESP_WIFICONFIG_ACCESS_POINT_PASS);
  this->IpAddress = WiFi.softAPIP();
}

String ESP_WifiConfig::nearAccessPoints(){
  String networkList = "{\"networks\":[";
  int n = WiFi.scanNetworks();
  if(n == 0){
    networkList += "]}";
  }else{
    for(int i = 0; i < n; i++){
      networkList += "\"" + WiFi.SSID(i) + "\"";
      if(n > i+1){ 
        networkList += ",";
      }
    }
    networkList += "]}";
  }
  return networkList;
}

void ESP_WifiConfig::connectWifi(void (*onSuccess)(), void (*onFailure)()){
  //Try to connect to Router automatically
  if(this->connectTimer()){
    (*onSuccess)();
    return;
  }
  this->loadConfig();
  if(this->connectTimer()){
    (*onSuccess)();
    return;
  }
  (*onFailure)();
}

void ESP_WifiConfig::resetWifi(){
  WiFi.disconnect();
}

void ESP_WifiConfig::factoryReset() {
    WiFi.disconnect();
    EEPROM.begin(512);
    for(uint8_t i = 0; i < 512; i++){
        EEPROM.write(i, 0);
    }
    EEPROM.end();
    ESP.restart();
}

IPAddress ESP_WifiConfig::getIPAddress(){
  return this->IpAddress != NULL ? this->IpAddress : WiFi.localIP();
}

String ESP_WifiConfig::getIPAddressString(){
  return this->ipToString(this->getIPAddress());
}

//Attempt a connection to router
bool ESP_WifiConfig::connectTimer(){
   int timer = this->WAIT_TIMER;
   while(WiFi.status() != WL_CONNECTED){
    if(timer < 1) {
      break;
    }
    delay(1000);
    timer--;
  }
  return timer > 1;
}

bool ESP_WifiConfig::loadConfig() {
    byte val;
    EEPROM.begin(this->EEPROM_INIT_ADDRESS);
    val = EEPROM.read(this->INFO_ADDRESS);

    if(val != 255 && val != 0){
        for(int i = 0; i < 32; i++){
            if(EEPROM.read(i) > 0)
                this->SSID_STR += char(EEPROM.read(i));
        }
        for(int i = 32; i < 132; i++){
            if(EEPROM.read(i) > 0)
                this->PASSWORD += char(EEPROM.read(i));
        }
        EEPROM.end();
        return true;
    }
    EEPROM.end();
    return false;
}

String ESP_WifiConfig::ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
