

#include "UUIDGenerator.h"
#ifdef ESP32
#include <WiFi.h>   // ESP32 平台的 WiFi 庫
#elif defined(ESP8266)
#include <ESP8266WiFi.h>  // ESP8266 平台的 WiFi 庫
#endif

// 構造函數
UUIDGenerator::UUIDGenerator() {}

// 根據 MAC 地址生成 UUID
String UUIDGenerator::generateFromMAC() {
    String mac = getMacAddress();
    mac.toUpperCase();

    // 插入 UUID 格式的破折號
    String uuid = mac.substring(0, 2) + mac.substring(3, 5) + "-" + 
                  mac.substring(6, 8) + "-" + 
                  mac.substring(9, 11) + "-" + 
                  mac.substring(12, 14) + "-" + 
                  mac.substring(15, 17) + mac.substring(18, 20);

    return uuid;
}

// 隨機生成 UUID
String UUIDGenerator::generateRandomUUID() {
    // 生成標準的 UUID 格式: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
    String uuid = randomHexBytes(8) + "-" + 
                  randomHexBytes(4) + "-" + 
                  randomHexBytes(4) + "-" + 
                  randomHexBytes(4) + "-" + 
                  randomHexBytes(12);
    return uuid;
}

// 生成隨機十六進制字符串
String UUIDGenerator::randomHexBytes(int length) {
    String result = "";
    for (int i = 0; i < length; i++) {
        int randByte = random(0, 16);  // 隨機 0 到 15 的數
        result += String(randByte, HEX);  // 轉換為十六進制
    }
    return result;
}

// 獲取設備的 MAC 地址
String UUIDGenerator::getMacAddress() {
    return WiFi.macAddress();  // 獲取設備的 MAC 地址
}
