//esp32\\2.0.10
#ifndef _AMHSMPU6866WIFI_H_
#define _AMHSMPU6866WIFI_H_

// #include <Adafruit_MPU6866.h>
// #include <Adafruit_Sensor.h>
// #include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <esp_heap_caps.h>
// #include <M5AtomS3.h>


#include "./src/common/common.h"
#include "./src/mpu6886/iotmpu6886.h"
// #include "./src/sqllite3db/sqllitedb.h"
#include "./src/wifi/iotwifi.h"
#include "./src/mqtt/MQTTClient.h"
#include "./src/Json/settingJson.h"



// OTA


// 定義兩個核心的Task函式
void mpu6866_task(void *parameter);
void wifiTask_0x68(void *parameter);
// void wifiTask_0x69(void *parameter);

// 定義佇列的變數
#define QUEUE_SIZE 6000
QueueHandle_t dataQueue_0x68;
//QueueHandle_t dataQueue_0x69;
SemaphoreHandle_t mutex;
iotSettingJson* mpuSettingJson;
//mqtt
// WiFiClient espClient;
// PubSubClient mqttClient(espClient);
// 全局 MQTTClient 物件

#endif
