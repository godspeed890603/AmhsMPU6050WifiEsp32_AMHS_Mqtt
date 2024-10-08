#include "iotmpu6050.h"

iotMPU6050::iotMPU6050(uint8_t i2c_addr) {
  int n;
  char buffer[50];

  // pinMode(ledPin, OUTPUT);  // 設置LED引腳為輸出

  mpu_i2c_addr = (unsigned int)i2c_addr;
  mpu = new Adafruit_MPU6050();

  char hex_str[10];                      // 存储十六进制字符串的数组
  sprintf(hex_str, "%X", mpu_i2c_addr);  // 将十进制数转换为十六进制字符串
  int hex_value;
  sscanf(hex_str, "%X", &hex_value);  // 将十六进制字符串转换为整数
  mpu_id = (String)hex_str;

  if (!mpu->begin(i2c_addr)) {
    n = sprintf(buffer, "MPU6050 not found i2c_addr\"%x\"",
                (unsigned int)i2c_addr);
    Serial.println(buffer);

    while (1) {
      delay(10);
    }
  } else {
    n = sprintf(buffer, "MPU6050  found i2c_addr 0x\"%x\"",
                (unsigned int)i2c_addr);
    Serial.println(buffer);
  }

  // 設置量程範圍為±8g和±1000dps
  mpu->setAccelerometerRange(mpu_accel_range);
  mpu->setGyroRange(mpu_gyro_range);
  mpu->setFilterBandwidth(mpu_bandwidth);
  mpu->setSampleRateDivisor(7);  // 设置采样率为1kHz（取样分频器为7）
  // mpu->setFilterBandwidth(MPU6050_BAND_44_HZ);
  // mpu->setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu->setHighPassFilter(mpu_highpass);

  resetMPU6050Data();
  getMPU6050OffsetData();
}

void iotMPU6050::getMPU6050OffsetData() {
  sensors_event_t accelEvent, gyroEvent, tempEvent;
  // int i;
  // int count;
  long endTime = 0;
  long ledtime = 0;
  bool* led_on = new bool(true);

  iotSettingJson* mpuSettingJson = new iotSettingJson();
  // mpuDB->check_settingJson_exist(mpu_id);
  // Serial.printf(" mpuDB->createTable()");
  // mpuDB->createTable();
  // Serial.println("Check countStart...");
  // count = mpuDB->checkAccOffset();
  // Serial.printf("count=");
  // Serial.println(count);
  // delay(10000);
  ledAlarm(2, 100);

  // 累積加速度計和陀螺儀的數據

  if (!(mpuSettingJson->checkSettingJsonExist(mpu_id))) {
    Serial.println("..test2..");
    for (int i = 0; i < numSamples; i++) {
      mpu->getEvent(&accelEvent, &gyroEvent, &tempEvent);
      // Serial.println("..test3..");
      accelOffsetX += accelEvent.acceleration.x;
      accelOffsetY += accelEvent.acceleration.y;
      accelOffsetZ += accelEvent.acceleration.z;
      // gyroOffsetX += gyroEvent.gyro.x;
      // gyroOffsetY += gyroEvent.gyro.y;
      // gyroOffsetZ += gyroEvent.gyro.z;

      long endTime = micros();
      if (endTime - ledtime > 0.02 * sencond_1) {
        ledDisplay(led_on);
        ledtime = endTime;
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

void iotMPU6050::getMPU6050Event() {
  // char buffer[50];
  // int n;
  sensors_event_t a, g, temp;
  mpu->getEvent(&a, &g, &temp);
  // mpu->reset();

  getMPU6050Acc(a, temp, g);
}
void iotMPU6050::getMPU6050Acc(sensors_event_t a, sensors_event_t temp,
                               sensors_event_t g) {
  // char buffer[50];
  // int n;
  //   sensors_event_t a, g, temp;
  //   mpu->getEvent(&a, &g, &temp);

  x_acc = a.acceleration.x - accelOffsetX;
  // sprintf(buffer, "[y_acc=%2.1f working..", x_acc);
  //  Serial.println(buffer);
  y_acc = a.acceleration.y - accelOffsetY;
  // sprintf(buffer, "[y_acc=%2.1f working..", y_acc);
  // Serial.println(buffer);
  z_acc = a.acceleration.z - accelOffsetZ;
  // sprintf(buffer, "[z_acc=%2.1f working..", z_acc);
  // Serial.println(buffer);
  temperature = temp.temperature;

  // 忽略或过滤异常数据
  if (getMPU6050Noise(x_acc, y_acc, z_acc) == false) {
    return;
  }
  // 累計最大值與最小值
  getMPU6050AccMaxMin(x_acc, y_acc, z_acc);
  // 補償後的值計算角度
  getMPU6050Angle(a, g);
}

// 忽略或过滤异常数据
bool iotMPU6050::getMPU6050Noise(float x, float y, float z) {
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
void iotMPU6050::getMPU6050Angle(sensors_event_t a, sensors_event_t g) {
  // char buffer[500];
  // int n;
  // sprintf(buffer, "[Angle]i2c_addr 0x%x working..", mpu_i2c_addr);
  // Serial.println(buffer);
  // 計算傾斜角度
  // pitch
  y_z_ang = (atan2(a.acceleration.y,
                   sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.z, 2))) *
             180 / PI) -
            angleOffsetY_Z;
  // sprintf(buffer, "[y_z_ang=%2.1f working..", y_z_ang);
  // Serial.println(buffer);
  // roll
  x_z_ang = (atan2(-a.acceleration.x,
                   sqrt(pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2))) *
             180 / PI) -
            angleOffsetX_Z;

  getMPU6050AngleMaxMin(x_z_ang, y_z_ang);
}
void iotMPU6050::getMPU6050AccMaxMin(float x, float y, float z) {
  // char buffer[200];
  // int n;
  // sprintf(buffer, "[getMPU6050AccMaxMin ini]initial_acc_flg=%d ",
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
void iotMPU6050::getMPU6050AngleMaxMin(float x_z, float y_z) {
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
  //         "[getMPU6050AngleMaxMin]MPU6050 not found i2c_addr\"%x\"   "
  //         "max_x_z_ang=%2.1f min_x_z_ang=%2.1f "
  //         "max_y_z_ang=%2.1f  min_y_z_ang=%2.1f Working..",
  //         (unsigned int)mpu_i2c_addr, max_x_z_ang, min_x_z_ang, max_y_z_ang,
  //         min_y_z_ang);
  // Serial.println(buffer);
}
void iotMPU6050::getMPU6050Temperature() {}

// String iotMPU6050::getMPU6050WebHtml() {
//   String webHtml;
//   char buffer[1000];
//   int n;
//   // sprintf(buffer, "[getMPU6050WebHtml]i2c_addr 0x%x working..",
//   // mpu_i2c_addr); Serial.println(buffer);
//   // sprintf(buffer, "0x%x", mpu_i2c_addr);
//   char hex_str[10];  // 存储十六进制字符串的数组
//   // Serial.println("web string start....1 ");
//   sprintf(hex_str, "%X", mpu_i2c_addr);  // 将十进制数转换为十六进制字符串
//   int hex_value;
//   // Serial.println("web string start....2 ");
//   sscanf(hex_str, "%X", &hex_value);  // 将十六进制字符串转换为整数
//   // Serial.println("web string start.... ");

//   // 172.27.17.232
//   String SENSOR_ID = ipAddr + "_0x" + (String)hex_str;
//   webHtml =
//       webserverHtml_Header + "SENSOR_ID=" + macAddress +
//       "&machine_ID=" + "AGV" + "&ip=" + ipAddr +  // +
//       "&rssi=" + (String)rssi + "&x_acc=" + (String)x_acc +
//       "&y_acc=" + (String)y_acc + "&z_acc=" + (String)z_acc +
//       "&max_x_acc=" + (String)max_x_acc + "&max_y_acc=" + (String)max_y_acc +
//       "&max_z_acc=" + (String)max_z_acc + "&min_x_acc=" + (String)min_x_acc +
//       "&min_y_acc=" + (String)min_y_acc + "&min_z_acc=" + (String)min_z_acc +
//       "&x_z_ang=" + (String)x_z_ang + "&y_z_ang=" + (String)y_z_ang +
//       "&max_x_z_ang=" + (String)max_x_z_ang +
//       "&max_y_z_ang=" + (String)max_y_z_ang +
//       "&min_x_z_ang=" + (String)min_x_z_ang +
//       "&min_y_z_ang=" + (String)min_y_z_ang +
//       "&temperature=" + (String)temperature;

//   return webHtml;
// }

void iotMPU6050::resetMPU6050Data() {
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

String iotMPU6050::getMPU6050Json() {
  // 創建一個足夠大的 StaticJsonDocument
  StaticJsonDocument<1024> doc;

  // 將十進制的 I2C 地址轉換為十六進制字符串
  char hex_str[10];
  sprintf(hex_str, "%X", mpu_i2c_addr);  // 將 I2C 地址轉換為十六進制

  // 構建 SENSOR_ID
  String SENSOR_ID = WiFi.macAddress();
  ;

  // Add the mac_address and correlation_id
  doc["mac_address"] = WiFi.macAddress();
  doc["correlation_id"] = "correlation_id";
  // 創建嵌套在 "data" 下的 JSON 對象
  JsonObject data = doc.createNestedObject("data");
  // 添加鍵值對到 "data" 中
  data["SENSOR_ID"] = WiFi.macAddress();
  data["machine_ID"] = "AGV";
  data["ip"] = ipAddr;
  data["rssi"] = rssi;
  data["x_acc"] = x_acc;
  data["y_acc"] = y_acc;
  data["z_acc"] = z_acc;
  data["max_x_acc"] = max_x_acc;
  data["max_y_acc"] = max_y_acc;
  data["max_z_acc"] = max_z_acc;
  data["min_x_acc"] = min_x_acc;
  data["min_y_acc"] = min_y_acc;
  data["min_z_acc"] = min_z_acc;
  data["x_z_ang"] = x_z_ang;
  data["y_z_ang"] = y_z_ang;
  data["max_x_z_ang"] = max_x_z_ang;
  data["max_y_z_ang"] = max_y_z_ang;
  data["min_x_z_ang"] = min_x_z_ang;
  data["min_y_z_ang"] = min_y_z_ang;
  data["temperature"] = temperature;

  // 使用 String 將 JSON 序列化
  String jsonOutput;
  serializeJson(doc, jsonOutput);

  return jsonOutput;  // 返回 JSON 字符串
}
