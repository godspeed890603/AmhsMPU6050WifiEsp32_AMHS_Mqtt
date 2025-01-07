#ifndef IOTMPU6050_H
#define IOTMPU6050_H

#include <Adafruit_MPU6050.h>
#include <ArduinoJson.h>
#include <Arduino.h>

#include "esp_chip_info.h"
#include "esp_system.h"
#include <stdio.h>

// #include <ArduinoOTA.h>  //V1.11

#include <string>
// 指定 SDA 和 SCL 引脚
#define SDA_PIN 1
#define SCL_PIN 2
#include "../common/common.h"
#include "../Json/settingJson.h"
#include "./src/uuid/uuidgenerator.h"

class iotMPU6050 {  // The class
 public:            // Access specifier

  const int MAX_ACCELERATION = 10.0;
  const int MAX_ACCELERATION_X = 4.0;
  const int MAX_ACCELERATION_Y = 3.5;
  const int MAX_ACCELERATION_Z = 10.0;

  int mpu_i2c_addr;
  float rssi;
  String ipAddr;
  String macAddress;
  String mpu_id;
  // acc
  float x_acc = 0.0;
  float y_acc = 0.0;
  float z_acc = 0.0;
  float max_x_acc = 0.0;
  float max_y_acc = 0.0;
  float max_z_acc = 0.0;
  float min_x_acc = 0.0;
  float min_y_acc = 0.0;
  float min_z_acc = 0.0;
  // angle
  float x_z_ang = 0.0;
  float y_z_ang = 0.0;
  float max_x_z_ang = 0.0;
  float max_y_z_ang = 0.0;
  float min_x_z_ang = 0.0;
  float min_y_z_ang = 0.0;
  // temperature
  float temperature = 0.0;
  bool initial_ang_flg = true;
  bool initial_acc_flg = true;

  sensors_event_t a, g, temp;
  // mpu->getEvent(&a, &g, &temp);

  // mpu6050
  Adafruit_MPU6050* mpu;  //= new Adafruit_MPU6050();

  iotMPU6050(uint8_t i2c_addr);
  void getMPU6050Event();
  void getMPU6050Acc(sensors_event_t a, sensors_event_t temp,
                     sensors_event_t g);
  void getMPU6050Angle(sensors_event_t a, sensors_event_t g);
  void getMPU6050AccMaxMin(float x, float y, float z);
  void getMPU6050AngleMaxMin(float x_z, float y_z);
  void getMPU6050Temperature();
  // String getMPU6050WebHtml();
  void getMPU6050OffsetData();
  void resetMPU6050Data();
  bool getMPU6050Noise(float x, float y, float z);
  void initialChipEsp32(uint8_t i2c_addr);
  void initialChipEsp32SxCx();
  String getMPU6050Json();//2024/09/24 FOR MQTT

  // 設置校準偏移量為0
  float accelOffsetX = 0.0;
  float accelOffsetY = 0.0;
  float accelOffsetZ = 0.0;
  float gyroOffsetX = 0.0;
  float gyroOffsetY = 0.0;
  float gyroOffsetZ = 0.0;

  float angleOffsetX_Z = 0.0;
  float angleOffsetY_Z = 0.0;

  // 累積數據的次數
  float numSamples = 1000.0;

  // iotSqliteDB* mpuDB;
  iotSettingJson* mpuSettingJson;
};

#endif /* IOTMPU6050_H */
