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

  // const char* ssid = "CIM_WIFI";
  // const char* pwd = "hsd@5052880";
  // const char* ssid = "CIM";
  // const char* pwd = "hsd5052880";
  // const char* ssid = "Eason";
  // const char* pwd = "28782878ab";
  int reconnectInterval = 1000;
  bool isDhcp = true;

  // IPAddress ip;
  // IPAddress gateway;
  // IPAddress subnet;
  // IPAddress dns;

  // IPAddress ip = {172, 27, 17,200};  // 设置为你想要的静态IP地址
  // IPAddress ip = {172, 27, 17,6};  // 设置为你想要的静态IP地址
  // IPAddress ip = {172, 27, 17, 233};  // 设置为你想要的静态IP地址
  // IPAddress subnet = {255, 255, 224, 0};   // 设置子网掩码
  // IPAddress gateway = {172, 27, 31, 254};  // 设置网关IP地址
  // IPAddress dns1 = {172, 27, 10, 81};   // 设置子网掩码
  // IPAddress dns2 = {172, 27, 10, 82};   // 设置子网掩码
  // the IP address for the shield:

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
// #endif /* GETTIME_H */
