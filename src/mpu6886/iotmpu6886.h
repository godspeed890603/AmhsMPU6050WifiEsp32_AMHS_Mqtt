#ifndef IOTMPU6886_H
#define IOTMPU6886_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include "esp_chip_info.h"
#include "esp_system.h"



#include <string>
// 指定 SDA 和 SCL 引脚
// #define SDA_PIN 1
// #define SCL_PIN 2
#include "../Json/settingJson.h"
#include "../common/common.h"
#include "./src/uuid/uuidgenerator.h"

class iotMPU6886 {  // The class
 public:            // Access specifier
  const int MAX_ACCELERATION = 10.0;
  const int MAX_ACCELERATION_X = 4.0;
  const int MAX_ACCELERATION_Y = 3.5;
  const int MAX_ACCELERATION_Z = 10.0;
  // imu_data_t data;
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

  
  // m5::M5Unified::config_t cfg;

  m5::imu_data_t imuData;
  

  iotMPU6886(uint8_t i2c_addr);
  void getMPU6886Event();
  void getMPU6886Acc(m5::imu_data_t a);
  void getMPU6886Angle(m5::imu_data_t a);
  void getMPU6886AccMaxMin(float x, float y, float z);
  void getMPU6886AngleMaxMin(float x_z, float y_z);
  void getMPU6886Temperature();
  // String getMPU6886WebHtml();
  void getMPU6886OffsetData();
  void resetMPU6886Data();
  bool getMPU6886Noise(float x, float y, float z);
  void initialChipEsp32(uint8_t i2c_addr);
  void initialChipEsp32SxCx();
  String getMPU6886Json();  // 2024/09/24 FOR MQTT

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

#endif /* IOTMPU6886_H */
