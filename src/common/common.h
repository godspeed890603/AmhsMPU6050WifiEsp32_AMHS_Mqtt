
#ifndef _COMMON_H_
#define _COMMON_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Adafruit_MPU6050.h>
#include <WiFi.h>
#include <WiFiSTA.h>

const int ledPin = 2;  // 將LED連接到GPIO 2
const int sencond_1=1000000;
const int mpu_loop_scan_time=1;
void ledDisplay(bool* led_on);
void ledAlarm(int loopCount, int delayTime);

//web data Upload
extern String webserverHtml_Header;

//wifi setting
extern const char* ssid;
extern const char* pwd;
extern IPAddress ip;
extern IPAddress subnet;
extern IPAddress gateway;
extern String webserverHtml_Header;


//mpu setting
extern mpu6050_accel_range_t mpu_accel_range;
extern mpu6050_gyro_range_t mpu_gyro_range;
extern mpu6050_bandwidth_t mpu_bandwidth;
extern mpu6050_highpass_t mpu_highpass;

#endif