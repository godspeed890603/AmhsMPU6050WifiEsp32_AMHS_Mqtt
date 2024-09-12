//esp32\\2.0.10
#ifndef _AMHSMPU6050WIFI_H_
#define _AMHSMPU6050WIFI_H_

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <esp_heap_caps.h>

// #include <task.h>
// #include <semphr.h>
#include "./src/common/common.h"
#include "./src/mpu6050/iotmpu6050.h"
#include "./src/sqllite3db/sqllitedb.h"
#include "./src/webAction/iotWebAction.h"
#include "./src/wifi/iotwifi.h"

// OTA


// 定義兩個核心的Task函式
void mpu6050_task(void *parameter);
void wifiTask_0x68(void *parameter);
void wifiTask_0x69(void *parameter);

// 定義佇列的變數
#define QUEUE_SIZE 6000
QueueHandle_t dataQueue_0x68;
//QueueHandle_t dataQueue_0x69;
SemaphoreHandle_t mutex;

#endif
