#ifndef IOTWIFI_H
#define IOTWIFI_H
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiSTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <SPIFFS.h>
#include "../common/common.h"

class iotWifi {  // The class
 public:         // Access specifier
  float rssiRetryCnt = 0.0;
  int wifiRetryCnt = 0;
  int rssiRertyCnt = 0;
  String ipAdress;
  String macAddress;
  char* _ssid;
  char* _pwd;
  IPAddress _ip;
  IPAddress _gateway;
  IPAddress _subnet;


  int reconnectInterval = 1000;
  bool isDhcp = true;


  String* otaHostname;
  void checkWifiStat();

  void WifiReset();
  void initialWifi();
  void WifiOTA();
  wl_status_t getWifiStatus();
  float getWifiRssi();

  String getIpAddress();
  String getmacAddress();
  iotWifi(const char* tSsid, const char* tPassword, IPAddress _Ip,
          IPAddress tSubnet, IPAddress tGateway);
};

#endif /* IOTWIFI_H */

