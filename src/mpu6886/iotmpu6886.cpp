#include "iotmpu6886.h"

iotMPU6886::iotMPU6886(uint8_t i2c_addr) {
  // int n;
  char buffer[50];
  // auto cfg = M5.config();
  // cfg = M5.config();
  M5.begin(cfg);

  // mpu = new Adafruit_MPU6886();

  mpu_i2c_addr = (unsigned int)i2c_addr;
  char hex_str[10];                      // 存储十六进制字符串的数组
  sprintf(hex_str, "%X", mpu_i2c_addr);  // 将十进制数转换为十六进制字符串
  int hex_value;
  sscanf(hex_str, "%X", &hex_value);  // 将十六进制字符串转换为整数
  mpu_id = (String)hex_str;

  // esp_chip_info_t chip_info;
  // esp_chip_info(&chip_info);

  // // 檢查 CHip type
  // printf("This is an ESP32 chip with %d CPU cores, WiFi%s%s, ",
  // chip_info.cores,
  //        (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
  //        (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
  // switch (chip_info.model) {
  //   case CHIP_ESP32:
  //     initialChipEsp32(i2c_addr);
  //     break;
  //   case CHIP_ESP32S2:
  //     printf("Chip Model: ESP32-S2\n");
  //     break;
  //   case CHIP_ESP32S3:
  //     initialChipEsp32SxCx();
  //     break;
  //   case CHIP_ESP32C6:
  //     initialChipEsp32SxCx();
  //     break;
  //   default:
  //     printf("Unknown chip model\n");
  //     break;
  // }

  // printf("Revision number: %d\n", chip_info.revision);

  // // 設置量程範圍為±8g和±1000dps
  // mpu->setAccelerometerRange(mpu_accel_range);
  // mpu->setGyroRange(mpu_gyro_range);
  // mpu->setFilterBandwidth(mpu_bandwidth);
  // mpu->setSampleRateDivisor(7);  // 设置采样率为1kHz（取样分频器为7）
  // // mpu->setFilterBandwidth(MPU6886_BAND_44_HZ);
  // // mpu->setHighPassFilter(MPU6886_HIGHPASS_0_63_HZ);
  // mpu->setHighPassFilter(mpu_highpass);

  resetMPU6886Data();
  getMPU6886OffsetData();
}

// void iotMPU6886::initialChipEsp32(uint8_t i2c_addr) {
//   char buffer[50];
//   int n;
//   printf("Chip Model: ESP32\n");
//   if (!mpu->begin(i2c_addr)) {
//     n = sprintf(buffer, "MPU6886 not found i2c_addr\"%x\"",
//                 (unsigned int)i2c_addr);
//     Serial.println(buffer);

//     while (1) {
//       delay(10);
//     }
//   } else {
//     n = sprintf(buffer, "MPU6886  found i2c_addr 0x\"%x\"",
//                 (unsigned int)i2c_addr);
//     Serial.println(buffer);
//   }
// }
// void iotMPU6886::initialChipEsp32SxCx() {
//   printf("Chip Model: ESP32-S3 or ESP32-C6\n");
//   // 初始化 I2C 接口，使用指定的引脚
//   Wire.begin(SDA_PIN, SCL_PIN);
//   // 初始化 MPU6886
//   if (!mpu->begin()) {
//     Serial.println("MPU6886 初始化失败！");
//     while (1) {
//       delay(10);  // 停止运行
//     }
//   }
// }

void iotMPU6886::getMPU6886OffsetData() {
  // sensors_event_t accelEvent, gyroEvent, tempEvent;
  // int i;
  // int count;
  long endTime = 0;
  long ledtime = 0;
  bool* led_on = new bool(true);

  iotSettingJson* mpuSettingJson = new iotSettingJson();
  ledAlarm(2, 100);

  // 累積加速度計和陀螺儀的數據

  if (!(mpuSettingJson->checkSettingJsonExist(mpu_id))) {
    Serial.println("..test2..");
    for (int i = 0; i < numSamples; i++) {
      // mpu->getEvent(&accelEvent, &gyroEvent, &tempEvent);
      bool imu_update = M5.Imu.update();
      if (imu_update) {
        // Serial.println("..test3..");
        imuData = M5.Imu.getImuData();
        accelOffsetX += imuData.accel.x;
        accelOffsetY += imuData.accel.y;
        accelOffsetZ += imuData.accel.z;
      }

      long endTime = micros();
      if (endTime - ledtime > 0.02 * sencond_1) {
        ledDisplay(led_on);
        ledtime = endTime;

        M5Display("Cali-ing",0.8);
        // AtomS3.Display.clear();
        // AtomS3.Display.setTextColor(GREEN);
        // AtomS3.Display.setTextDatum(middle_center);
        // AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
        // AtomS3.Display.setTextSize(1);
        // AtomS3.Display.drawString("Cali-ing", AtomS3.Display.width() / 2,
        //                           AtomS3.Display.height() / 2);
      }

      // ledDisplay(led_on);
      // delay(10);
    }
    delete (led_on);
    Serial.println("..計算平均值..");
    // 計算平均值
    accelOffsetX = accelOffsetX / numSamples;
    accelOffsetY = accelOffsetY / numSamples;
    accelOffsetZ = accelOffsetZ / numSamples;

    // char buffer[500];
    Serial.println(mpu_id);
    // sprintf(buffer,
    //         "[accelOffsetX=%2.2f  accelOffsetY=%2.2f  accelOffsetZ=%2.2f..",
    //         accelOffsetX, accelOffsetY, accelOffsetZ);
    // Serial.println(buffer);
    Serial.println("..Insert Data..");
    bool rtn = mpuSettingJson->insertAccOffset(accelOffsetX, accelOffsetY,
                                               accelOffsetZ);
    // Serial.println("..test4..");
  }

  M5Display("Amhs XYZ",0.8);

  // AtomS3.Display.clear();
  // AtomS3.Display.setTextColor(GREEN);
  // AtomS3.Display.setTextDatum(middle_center);
  // AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
  // AtomS3.Display.setTextSize(0.8);
  // AtomS3.Display.drawString("Amhs XYZ", AtomS3.Display.width() / 2,
  //                           AtomS3.Display.height() / 2);


  Serial.println("..mpuSettingJson->ReadAccOffset..");
  // 讀取加速度補償直
  mpuSettingJson->ReadAccOffset(&accelOffsetX, &accelOffsetY, &accelOffsetZ);
  Serial.println("..mpuSettingJson->ReadAccOffset..end");
  Serial.println("========angleOffsetX_Z end===============");
  Serial.print("accelOffsetX: ");
  Serial.println(accelOffsetX);
  Serial.print("accelOffsetY: ");
  Serial.println(accelOffsetY);
  Serial.print("accelOffsetZ: ");
  Serial.println(accelOffsetZ);
  Serial.println("=======================");

  // 計算角度補償
  Serial.println("========計算角度補償===============");
  angleOffsetY_Z =
      atan2(accelOffsetY, sqrt(pow(accelOffsetX, 2) + pow(accelOffsetZ, 2))) *
      180 / PI;
  Serial.println("========angleOffsetX_Z===============");
  angleOffsetX_Z =
      atan2(-accelOffsetX, sqrt(pow(accelOffsetY, 2) + pow(accelOffsetZ, 2))) *
      180 / PI;
  Serial.println("========angleOffsetX_Z end===============");
  Serial.print("accelOffsetX: ");
  Serial.println(accelOffsetX);
  Serial.print("accelOffsetY: ");
  Serial.println(accelOffsetY);
  Serial.print("accelOffsetZ: ");
  Serial.println(accelOffsetZ);
  Serial.println("=======================");
  // mpuSettingJson->dbClose();
  delete (mpuSettingJson);
  Serial.println("======== delete (mpuSettingJson);===============");
}

void iotMPU6886::getMPU6886Event() {
  // char buffer[50];
  // int n;
  // Serial.print("M5.Imu.update(); ");
  bool imu_update = M5.Imu.update();
  if (imu_update) {
    imuData = M5.Imu.getImuData();
    getMPU6886Acc(imuData);
  }
  // Serial.print("M5.Imu.update(); Complete");
}
void iotMPU6886::getMPU6886Acc(m5::imu_data_t a) {
  char buffer[50];
  int n;

  x_acc = a.accel.x - accelOffsetX;
  // sprintf(buffer, "[x_acc=%2.1f working..", x_acc);
  // Serial.println(buffer);
  y_acc = a.accel.y - accelOffsetY;
  // sprintf(buffer, "[y_acc=%2.1f working..", y_acc);
  // Serial.println(buffer);
  z_acc = a.accel.z - accelOffsetZ;
  // sprintf(buffer, "[z_acc=%2.1f working..", z_acc);
  // Serial.println(buffer);
  temperature = 0;  // temp.temperature;

  //display acc on screen
  M5DisplayAcc(x_acc,y_acc,z_acc);

  // 忽略或过滤异常数据
  if (getMPU6886Noise(x_acc, y_acc, z_acc) == false) {
    return;
  }
  // 累計最大值與最小值
  getMPU6886AccMaxMin(x_acc, y_acc, z_acc);
  // 補償後的值計算角度
  getMPU6886Angle(a);
}

// 忽略或过滤异常数据
bool iotMPU6886::getMPU6886Noise(float x, float y, float z) {
  // 忽略或过滤异常数据
  if (abs(x_acc) > MAX_ACCELERATION || abs(y_acc) > MAX_ACCELERATION ||
      abs(z_acc) > MAX_ACCELERATION) {
    // Serial.println("First Over");
    return false;
  }

  // if (abs(x_acc) > MAX_ACCELERATION_X) {
  //   Serial.println("X is over");
  //   return false;
  // }
  // if (abs(y_acc) > MAX_ACCELERATION_Y) {
  //     Serial.println("Y is over");
  //   return false;
  // }
  // if (abs(z_acc) > MAX_ACCELERATION_Z) {
  //    Serial.println("Z is over");
  //   return false;
  // }
  return true;
}
void iotMPU6886::getMPU6886Angle(m5::imu_data_t a) {
  // char buffer[500];
  // int n;
  // sprintf(buffer, "[Angle]i2c_addr 0x%x working..", mpu_i2c_addr);
  // Serial.println(buffer);
  // 計算傾斜角度
  // pitch
  y_z_ang = (atan2(a.accel.y, sqrt(pow(a.accel.x, 2) + pow(a.accel.z, 2))) *
             180 / PI) -
            angleOffsetY_Z;
  // sprintf(buffer, "[y_z_ang=%2.1f working..", y_z_ang);
  // Serial.println(buffer);
  // roll
  x_z_ang = (atan2(-a.accel.x, sqrt(pow(a.accel.y, 2) + pow(a.accel.z, 2))) *
             180 / PI) -
            angleOffsetX_Z;

  getMPU6886AngleMaxMin(x_z_ang, y_z_ang);
}
void iotMPU6886::getMPU6886AccMaxMin(float x, float y, float z) {
  // char buffer[200];
  // int n;
  // sprintf(buffer, "[getMPU6886AccMaxMin ini]initial_acc_flg=%d ",
  //         initial_acc_flg);
  // Serial.println(buffer);
  if (initial_acc_flg == true) {
    // get max
    max_x_acc = x;
    min_x_acc = x;
    max_y_acc = y;
    min_y_acc = y;
    max_z_acc = z;
    min_z_acc = z;
    initial_acc_flg = false;
    return;
  }

  // get max
  if (x > max_x_acc) max_x_acc = x;
  // get min
  if (x < min_x_acc) min_x_acc = x;

  // get max
  if (y > max_y_acc) max_y_acc = y;
  // get min
  if (y < min_y_acc) min_y_acc = y;

  // get max
  if (z > max_z_acc) max_z_acc = z;
  // get min
  if (z < min_z_acc) min_z_acc = z;
}
void iotMPU6886::getMPU6886AngleMaxMin(float x_z, float y_z) {
  // char buffer[500];
  // int n;
  // Serial.println("test1....");
  //  get max
  if (initial_ang_flg == true) {
    max_y_z_ang = y_z;
    min_y_z_ang = y_z;

    max_x_z_ang = x_z;
    min_x_z_ang = x_z;

    initial_ang_flg = false;
    return;
  }

  // Serial.println("test2....");
  //  get max
  if (x_z > max_x_z_ang) max_x_z_ang = x_z;
  // get min
  if (x_z < min_x_z_ang) min_x_z_ang = x_z;

  // get max
  if (y_z > max_y_z_ang) max_y_z_ang = y_z;
  // get min
  if (y_z < min_y_z_ang) min_y_z_ang = y_z;

  // sprintf(buffer,
  //         "[getMPU6886AngleMaxMin]MPU6886 not found i2c_addr\"%x\"   "
  //         "max_x_z_ang=%2.1f min_x_z_ang=%2.1f "
  //         "max_y_z_ang=%2.1f  min_y_z_ang=%2.1f Working..",
  //         (unsigned int)mpu_i2c_addr, max_x_z_ang, min_x_z_ang,
  //         max_y_z_ang, min_y_z_ang);
  // Serial.println(buffer);
}
void iotMPU6886::getMPU6886Temperature() {}

void iotMPU6886::resetMPU6886Data() {
  // acc
  // x_acc = 0.0;
  // y_acc = 0.0;
  // z_acc = 0.0;
  max_x_acc = 0.0;
  max_y_acc = 0.0;
  max_z_acc = 0.0;
  min_x_acc = 0.0;
  min_y_acc = 0.0;
  min_z_acc = 0.0;
  // angle
  // x_z_ang = 0.0;
  // y_z_ang = 0.0;
  max_x_z_ang = 0.0;
  max_y_z_ang = 0.0;
  min_x_z_ang = 0.0;
  min_y_z_ang = 0.0;
  // temperature
  // temperature = 0.0;
  initial_ang_flg = true;
  initial_acc_flg = true;
}
//--------------------------------------------------------------------------------------

String iotMPU6886::getMPU6886Json() {
  // 創建一個足夠大的 StaticJsonDocument
  StaticJsonDocument<1024> doc;
  UUIDGenerator uuidGen;

  // 將十進制的 I2C 地址轉換為十六進制字符串
  char hex_str[10];
  sprintf(hex_str, "%X", mpu_i2c_addr);  // 將 I2C 地址轉換為十六進制

  // 構建 SENSOR_ID
  String SENSOR_ID = WiFi.macAddress();
  ;

  // Add the mac_address and correlation_id
  doc["mac_address"] = WiFi.macAddress();
  doc["correlation_id"] =
      uuidGen.generateRandomUUID();  // 替換為實際的 correlation ID
  // 創建嵌套在 "data" 下的 JSON 對象
  JsonObject data = doc["data"].to<JsonObject>();
  // JsonObject data = doc.createNestedObject("data");
  // 添加鍵值對到 "data" 中
  // 添加鍵值對到 "data" 中
  data["SENSOR_ID"] = WiFi.macAddress();
  data["machine_ID"] = "AGV";
  data["ip"] = ipAddr;
  data["rssi"] = rssi;
  data["x_acc"] = String(x_acc, 2);
  data["y_acc"] = String(y_acc, 2);
  data["z_acc"] = String(z_acc, 2);
  data["max_x_acc"] = String(max_x_acc, 2);
  data["max_y_acc"] = String(max_y_acc, 2);
  data["max_z_acc"] = String(max_z_acc, 2);
  data["min_x_acc"] = String(min_x_acc, 2);
  data["min_y_acc"] = String(min_y_acc, 2);
  data["min_z_acc"] = String(min_z_acc, 2);
  data["x_z_ang"] = String(x_z_ang, 2);
  data["y_z_ang"] = String(y_z_ang, 2);
  data["max_x_z_ang"] = String(max_x_z_ang, 2);
  data["max_y_z_ang"] = String(max_y_z_ang, 2);
  data["min_x_z_ang"] = String(min_x_z_ang, 2);
  data["min_y_z_ang"] = String(min_y_z_ang, 2);
  data["temperature"] = String(temperature, 2);

  // 使用 String 將 JSON 序列化
  String jsonOutput;
  serializeJson(doc, jsonOutput);

  return jsonOutput;  // 返回 JSON 字符串
}
