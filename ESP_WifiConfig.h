/*
 * ESP_WiFiConfig
 * A simple interface to help streamline the coding and wifi setup for
 * the ESP line of microcontrollers
 *
 * @author Carlos Martin - pirumpi@gmail.com
 * @version 0.7.0 02/10/29017
 */

#ifndef _ESP_WIFICONFIG_H
#define _ESP_WIFICONFIG_H

#ifndef ESP_WIFICONFIG_ACCESS_POINT_USER
#define ESP_WIFICONFIG_ACCESS_POINT_USER "WifiConfig"
#endif

#ifndef ESP_WIFICONFIG_ACCESS_POINT_PASS
#define ESP_WIFICONFIG_ACCESS_POINT_PASS "123456789"
#endif

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>

class ESP_WifiConfig{
public:

    ESP_WifiConfig(){};

    bool setWifi(String ssid, String password);
    bool startAccessPoint();
    String nearAccessPoints();

    void connectWifi(void (*)(), void(*)());
    void resetWifi();
    void factoryReset();

    String getIPAddressString();
    IPAddress getIPAddress();

private:

    const int static INFO_ADDRESS = 0;
    const int static WAIT_TIMER = 20;
    const int static EEPROM_INIT_ADDRESS = 512;

    String SSID_STR;
    String PASSWORD;
    IPAddress IpAddress;

    bool connectTimer();
    bool loadConfig();
    String ipToString(IPAddress ip);
};

#endif //_ESP_WIFICONFIG_H
