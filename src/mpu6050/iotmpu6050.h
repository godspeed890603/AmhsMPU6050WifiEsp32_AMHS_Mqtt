#ifndef IOTMPU6050_H
#define IOTMPU6050_H

#include <Adafruit_MPU6050.h>
#include <ArduinoJson.h>
#include <Arduino.h>
// #include <ArduinoOTA.h>  //V1.11

#include <string>

#include "../common/common.h"
#include "../sqllite3db/sqllitedb.h"

class iotMPU6050 {  // The class
 public:            // Access specifier
  // http://172.20.10.4:2878/amhsSensor/amhsVibration.php?SENSOR_ID=CIMPARTICLE&DATA_CNT=6&DATA_001=163&DATA_002=52&DATA_003=1&DATA_004=0&DATA_005=0&DATA_006=0

  // String webserverHtml_Header =
  //     "http://172.27.17.4:2878/amhsSensor/amhsVibration.php?";
  // String webserverHtml_Header =
  //     "http://172.27.51.73:8614/amhsSensor/amhsVibration.php?";
  // String webserverHtml_Header =
  //     "http://172.27.51.73:8614/amhsSensor/amhsVibration_cim.php?";

  // String webserverHtml_Header =
  //      "http://172.28.78.154:80/amhsSensor/amhsVibration_cim.php?";
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
  String getMPU6050WebHtml();
  void getMPU6050OffsetData();
  void resetMPU6050Data();
  bool getMPU6050Noise(float x, float y, float z);
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

  iotSqliteDB* mpuDB;
};

#endif /* IOTMPU6050_H */
