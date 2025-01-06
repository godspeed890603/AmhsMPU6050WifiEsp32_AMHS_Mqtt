
#include "./settingJson.h"

// 檢查是否存在指定 mpu_id 的 JSON 設定檔
bool iotSettingJson::checkSettingJsonExist(String mpu_id) {
  char fullJsonFileName[60];
  // 將 mpu_id 格式化為 JSON 檔案名稱，並存入 fullJsonFileName
  sprintf(fullJsonFileName, "/0x%s.json", mpu_id.c_str());
  Serial.println(fullJsonFileName);

  // 初始化 SPIFFS 檔案系統，若失敗則返回 false
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("Failed to mount file system");
    return false;
  }

  // 檢查指定的檔案是否存在
  settingFullFilename = (String)fullJsonFileName;  // 儲存完整檔案路徑
  if (SPIFFS.exists(fullJsonFileName)) {
    Serial.println("File exists");
    return true;  // 檔案存在，返回 true
  } else {
    Serial.println("File does not exist");
    Serial.println(settingFullFilename);  // 顯示不存在的檔案名稱
    return false;                         // 檔案不存在，返回 false
  }
}

// 將資料寫入 SPIFFS
void iotSettingJson::writeFileToSPIFFS(String data) {
  // 打開 SPIFFS 檔案，若不存在會自動創建
  Serial.print("open filename:");

  File file = SPIFFS.open(settingFullFilename.c_str(), FILE_WRITE);
  Serial.println("open filename");

  // 檢查是否成功打開檔案
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // 寫入 JSON 資料到檔案，並檢查寫入是否成功
  if (file.print(data)) {
    Serial.println("File was written successfully");
  } else {
    Serial.println("Failed to write to file");
  }

  // 關閉檔案
  file.close();
}

// 從 SPIFFS 中讀取資料，並將加速度校正值提取到傳入的參數中
void iotSettingJson::readFileFromSPIFFS(float* accelOffsetX,
                                        float* accelOffsetY,
                                        float* accelOffsetZ) {
  Serial.print("open file for reading:");
  Serial.println(settingFullFilename);

  // 以讀取模式打開檔案
  File file = SPIFFS.open(settingFullFilename.c_str(), FILE_READ);

  // 檢查是否成功打開檔案
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  // 讀取檔案內容到字串中
  String jsonString;
  while (file.available()) {
    jsonString += char(file.read());
  }

  Serial.println("File content:");
  Serial.println(jsonString);

  // 關閉檔案
  file.close();

  // 解析 JSON 字串
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  // 若解析失敗，則返回錯誤
  if (error) {
    Serial.println("Failed to parse JSON");
    return;
  }

  // 提取 JSON 中的加速度校正數據
  if (doc.containsKey("accelOffsetX") && doc.containsKey("accelOffsetY") &&
      doc.containsKey("accelOffsetZ")) {
    *accelOffsetX = doc["accelOffsetX"].as<float>();
    *accelOffsetY = doc["accelOffsetY"].as<float>();
    *accelOffsetZ = doc["accelOffsetZ"].as<float>();
  } else {
    Serial.println("Missing accel offset data in JSON");
  }
}

// 插入加速度校正值並將其寫入到 SPIFFS
bool iotSettingJson::insertAccOffset(float accelOffsetX, float accelOffsetY,
                                     float accelOffsetZ) {
  Serial.println("insertAccOffset Start......");

  // 創建 JSON 文件並加入加速度校正值
  StaticJsonDocument<100> doc;
  doc["accelOffsetX"] = accelOffsetX;
  doc["accelOffsetY"] = accelOffsetY;
  doc["accelOffsetZ"] = accelOffsetZ;

  // 序列化 JSON 到字串
  String jsonOutput;
  serializeJson(doc, jsonOutput);

  // 將序列化的 JSON 寫入到 SPIFFS
  writeFileToSPIFFS(jsonOutput);

  Serial.println("insertAccOffset end......");
  return true;
}

// 讀取加速度校正值
bool iotSettingJson::ReadAccOffset(float* accelOffsetX, float* accelOffsetY,
                                   float* accelOffsetZ) {
  Serial.println("readFileFromSPIFFS");

  // 從 SPIFFS 中讀取並解析加速度校正值
  readFileFromSPIFFS(accelOffsetX, accelOffsetY, accelOffsetZ);

  Serial.println("readFileFromSPIFFS end!!!!");
  return true;
}

void iotSettingJson::DeleteOffsetFile() {
  // 初始化 SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialization failed!");
    return;
  }
  Serial.println("SPIFFS initialized.");

  // 要刪除的檔案路徑
  const char* filePath = "/0x68.json";

  // 檢查檔案是否存在
  if (SPIFFS.exists(filePath)) {
    // 刪除檔案
    if (SPIFFS.remove(filePath)) {
      Serial.printf("File %s deleted successfully!\n", filePath);
    } else {
      Serial.printf("Failed to delete file %s\n", filePath);
    }
  } else {
    Serial.printf("File %s does not exist.\n", filePath);
  }
}

// #include "./settingJson.h"

// bool iotSettingJson::checkSettingJsonExist(String mpu_id) {
//   char fullJsonFileName[60];
//   // Serial.println("mpu_id.c_str():");
//   // Serial.println(mpu_id.c_str());
//   sprintf(fullJsonFileName, "/0x%s.json", mpu_id.c_str());
//   Serial.println(fullJsonFileName);

//   if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
//     Serial.println("Failed to mount file system");
//     return false;
//   }

//   // 檢查檔案是否存在
//   settingFullFilename = (String)fullJsonFileName;
//   if (SPIFFS.exists(fullJsonFileName)) {
//     Serial.println("File exists");
//     return true;
//   } else {
//     Serial.println("File does not exist");
//     Serial.println(settingFullFilename);
//     return false;
//   }
// }

// void iotSettingJson::writeFileToSPIFFS(String data) {
//   // 打開 SPIFFS 檔案，如果檔案不存在會自動創建
//   Serial.print("open filename:");
//   // Serial.println(filename);

//   File file = SPIFFS.open(settingFullFilename.c_str(), FILE_WRITE);
//   Serial.println("open filename");
//   if (!file) {
//     Serial.println("Failed to open file for writing");
//     return;
//   }

//   // 寫入 JSON 資料到檔案
//   if (file.print(data)) {
//     Serial.println("File was written successfully");
//   } else {
//     Serial.println("Failed to write to file");
//   }

//   // 關閉檔案
//   file.close();
// }

// void iotSettingJson::readFileFromSPIFFS(float* accelOffsetX,
//                                         float* accelOffsetY,
//                                         float* accelOffsetZ) {
//   Serial.print("open file for reading:");
//   Serial.println(settingFullFilename);

//   // 打開 SPIFFS 檔案，讀取模式
//   File file = SPIFFS.open(settingFullFilename.c_str(), FILE_READ);

//   if (!file) {
//     Serial.println("Failed to open file for reading");
//     return;
//   }

//   // 讀取整個檔案內容到字串
//   String jsonString;
//   while (file.available()) {
//     jsonString += char(file.read());
//   }

//   Serial.println("File content:");
//   Serial.println(jsonString);

//   // 關閉檔案
//   file.close();

//   // 解析 JSON
//   StaticJsonDocument<200> doc;
//   DeserializationError error = deserializeJson(doc, jsonString);

//   if (error) {
//     Serial.println("Failed to parse JSON");
//     return;
//   }

//   // 提取 JSON 中的數據
//   if (doc.containsKey("accelOffsetX") && doc.containsKey("accelOffsetY") &&
//       doc.containsKey("accelOffsetZ")) {
//     *accelOffsetX = doc["accelOffsetX"].as<float>();
//     *accelOffsetY = doc["accelOffsetY"].as<float>();
//     *accelOffsetZ = doc["accelOffsetZ"].as<float>();
//   } else {
//     Serial.println("Missing accel offset data in JSON");
//   }

//   // // 輸出讀取到的資料
//   // Serial.println("Read JSON from file:");
//   // Serial.println("accelOffsetX: " + (String)*accelOffsetX);
//   // Serial.println("accelOffsetY: " + (String)*accelOffsetY);
//   // Serial.println("accelOffsetZ: " + (String)*accelOffsetZ);
//   return;
// }

// bool iotSettingJson::insertAccOffset(float accelOffsetX, float accelOffsetY,
//                                      float accelOffsetZ) {
//   Serial.println("insertAccOffset Start......");
//   // 創建 JSON 文件
//   StaticJsonDocument<100> doc;
//   doc["accelOffsetX"] = accelOffsetX;
//   doc["accelOffsetY"] = accelOffsetY;
//   doc["accelOffsetZ"] = accelOffsetZ;
//   // 使用 String 將 JSON 序列化
//   String jsonOutput;
//   serializeJson(doc, jsonOutput);
//   // 寫入 JSON 到 SPIFFS
//   writeFileToSPIFFS(jsonOutput);
//   Serial.println("insertAccOffset end......");
//   return true;
// }

// bool iotSettingJson::ReadAccOffset(float* accelOffsetX, float* accelOffsetY,
//                                    float* accelOffsetZ) {
//   Serial.println("readFileFromSPIFFS");
//   readFileFromSPIFFS(accelOffsetX, accelOffsetY, accelOffsetZ);
//   Serial.println("readFileFromSPIFFS end!!!!");
//   return true;
// }
