#include "./iotwifi.h"

iotWifi::iotWifi(const char* tSsid, const char* tPassword, const IPAddress _Ip,
                 const IPAddress tSubnet, const IPAddress tGateway) {
  //Serial.println("......1");

  // 為 ssid 分配足夠的內存空間
  _ssid = (char*)malloc(strlen(tSsid) + 1);
  //Serial.println("......2");

  // 為 password 分配足夠的內存空間
  _pwd = (char*)malloc(strlen(tPassword) + 1);
  //Serial.println("......3");

  // 賦值 IP 地址
  _ip = _Ip;
  //Serial.println("......4");

  // 修正 tGateway 和 tSubnet 的賦值
  _gateway = tGateway;
  //Serial.println("......5");
  _subnet = tSubnet;
  //Serial.println("......6");

  // 複製 ssid 和 password
  memcpy(_ssid, tSsid, strlen(tSsid) + 1);
  memcpy(_pwd, tPassword, strlen(tPassword) + 1);

  //Serial.println("[] begin..." + _ip.toString());
  Serial.println(_ssid);
  Serial.println(_pwd);
  //Serial.println("[] begin...7" + _ip.toString());
  // Serial.println("......1");

  // // 為 ssid 分配足夠的內存空間
  // _ssid = (char*)malloc(strlen(tSsid) + 1);
  // Serial.println("......2");

  // // 為 password 分配足夠的內存空間
  // _pwd = (char*)malloc(strlen(tPassword) + 1);
  // Serial.println("......3");

  // // 賦值 IP 地址
  // _ip = _Ip;
  // Serial.println("......4");

  // // 修正 tGateway 和 tSubnet 的賦值
  // _gateway = tGateway;
  // Serial.println("......5");
  // _subnet = tSubnet;
  // Serial.println("......6");

  // // 複製 ssid 和 password
  // strcpy(_ssid, tSsid);
  // strcpy(_pwd, tPassword);

  // Serial.println("[] begin...1" + _ip.toString());
  // Serial.println(_ssid);
  // Serial.println(_pwd);
  // Serial.println("[] begin...7" + _ip.toString());
  // Serial.println("......1");
  //  _ssid = (char*)malloc(strlen(tSsid) + 1);
  // //_ssid = new char[strlen(tSsid) + 1];
  // Serial.println("......2");
  // //_pwd = new char[strlen(tPassword) + 1];
  // _pwd = (char*)malloc(strlen(tPassword) + 1);
  // Serial.println("......3");
  // _ip = _Ip;
  // Serial.println("......4");
  // _gateway = tSubnet;
  // Serial.println("......5");
  // _subnet = tGateway;
  // Serial.println(*tSsid);
  // Serial.println(*tPassword);
  // Serial.println("......6");

  // // memcpy(_ssid, tSsid, strlen(tSsid) + 1);
  // // memcpy(_pwd, tPassword, strlen(_pwd) + 1);
  // _ssid=tSsid;
  // _pwd=tPassword;
  // Serial.println("[] begin...1" + ip.toString());
  // Serial.println(_ssid);
  // Serial.println(_pwd);
  // Serial.println("[] begin...7" + ip.toString());
}

void iotWifi::checkWifiStat() {
  char buffer[50];
  int n;
  // sprintf(buffer, "CIM Wifi Rssi=\"%2.1f\"", getWifiRssi());
  // Serial.println(buffer);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".WL_DISCONNECTED...");
    ESP.restart();
    delay(reconnectInterval);
  }
}

/*wifi disconnect & reconnect */
void iotWifi::WifiReset() {
  // ESP.restart();
  Serial.printf("[Disconnct Wifi...]\n");
  WiFi.disconnect();
  delay(reconnectInterval);
  initialWifi();
}

/*wifi reconnect */
void iotWifi::initialWifi() {
  Serial.println("WiFi.mode(WIFI_STA).....");
  WiFi.mode(WIFI_STA);
  //Serial.println("set pwd.....");
  // Static IP Setup Info Here...

  if (isDhcp != true) {
    //Serial.println("[ip.getway] begin..." + ip.toString());
    WiFi.config(
        ip, gateway,
        subnet);  // If you need Internet Access You should Add DNS also...
  }

  //Serial.println("set pwd.....");
  WiFi.begin(_ssid, _pwd);
  // WiFiMulti.addAP(ssid, pwd);
  // ipDisplay();
  // if ((WiFiMulti.run() == WL_CONNECTED)) {
  Serial.println("connection....");
  int i = 0;
  digitalWrite(ledPin, HIGH);  // 將LED引腳設置為高電位（點亮LED）

  // // 獲取並顯示ESP32的MAC地址
  //   String macAddress = WiFi.macAddress();
  //   Serial.print("ESP32 MAC Address: ");
  //   Serial.println(macAddress);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    // ipDisplay();
    // delay(1*Sec1);
    if (i > 10) {
      Serial.println("System 32 restart!!!");
      ESP.restart();
    }
    delay(reconnectInterval);
    i++;
  }
  WiFi.setAutoReconnect(true);
  Serial.println(".");

  digitalWrite(ledPin, LOW);  // 將LED引腳設置為高電位（點亮LED）

  // Start OTA
  WifiOTA();
  // wifiLose =false;
  Serial.println("[WiFi.getway] begin..." + WiFi.localIP().toString());
  Serial.println("[WiFi.getway] begin..." + WiFi.gatewayIP().toString());
  Serial.println("[WiFi.subnet] begin..." + WiFi.subnetMask().toString());
  // wifiRssi = WiFi.RSSI();
  ipAdress = getIpAddress();
  macAddress = getmacAddress();
}

String iotWifi::getIpAddress() {
  char ipAddress1[16];

  sprintf(ipAddress1, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1],
          WiFi.localIP()[2], WiFi.localIP()[3]);
  return String(ipAddress1);
}
String iotWifi::getmacAddress() {
  // char ipAddress1[16];

  // sprintf(ipAddress1, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1],
  //         WiFi.localIP()[2], WiFi.localIP()[3]);
  return String(WiFi.macAddress().c_str());
}

// /*get wifi Status */
// bool iotWifi::getWifiStatus(){

//   if (WiFi.status() != WL_CONNECTED){
//     Serial.println("[WiFi.status()] begin...WiFi.status() != WL_CONNECTED" +
//     String(WiFi.status())); return false;
//   }

//   if (WiFi.status() == WL_CONNECTED){
//     Serial.println("[WiFi.status()] begin...WiFi.status() == WL_CONNECTED" +
//     String(WiFi.status())); return true;
//   }
// }

/*get wifi Status */
wl_status_t iotWifi::getWifiStatus() {
  return WiFi.status();

  // if (WiFi.status() != WL_CONNECTED){
  //   Serial.println("[WiFi.status()] begin...WiFi.status() != WL_CONNECTED" +
  //   String(WiFi.status())); return false;
  // }

  // if (WiFi.status() == WL_CONNECTED){
  //   Serial.println("[WiFi.status()] begin...WiFi.status() == WL_CONNECTED" +
  //   String(WiFi.status())); return true;
  // }
}

/*get wifi Rssi */
float iotWifi::getWifiRssi() { return WiFi.RSSI(); }

// V1.11
void iotWifi::WifiOTA() {
  // // Port defaults to 8266
  // // ArduinoOTA.setPort(8614);

  // // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("AGV_Vibration_Controller01");
  // ArduinoOTA.setHostname(otaHostname);
  //  // No authentication by default
  //ArduinoOTA.setPassword("syscim@888");

  // Password can be set with it's md5 value as well
  // MD5(syscim@888) = 61a98ced8f998f161460e602145f4c9b
  //ArduinoOTA.setPasswordHash("61a98ced8f998f161460e602145f4c9b");
  Serial.println("strat OTA....");
  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else {
          SPIFFS.end();
          type = "filesystem";
        }  // U_SPIFFS

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS
        // using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);

        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");

        // ESP.restart();
      });

  ArduinoOTA.begin();
}