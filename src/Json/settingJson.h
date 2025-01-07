#ifndef SETTINGJSON_H  // 預防重複包含此標頭檔案
#define SETTINGJSON_H

#include <Arduino.h>    // 包含Arduino核心庫，用於串口打印和其他基本功能
#include <ArduinoJson.h> // 包含ArduinoJson庫，用於處理JSON數據
#include <FS.h>         // 包含文件系統（FS）頭文件，用於SPIFFS文件操作
#include <SPI.h>        // 包含SPI庫，用於SPI通訊
#include <stdio.h>      // C標準輸入輸出庫
#include <stdlib.h>     // C標準庫，提供內存管理、隨機數生成、C標準函數
#include "SPIFFS.h"     // SPIFFS文件系統，專門為ESP32設計

// 定義宏：如果SPIFFS格式化失敗則執行格式化
#define FORMAT_SPIFFS_IF_FAILED true

// 定義 iotSettingJson 類別
class iotSettingJson {
 public:
  int rc;  // 用來存儲返回代碼或結果的變數

  // 用來存儲完整文件名稱的字串，用於保存讀寫的設定檔名
  String settingFullFilename;


  // 檢查加速度計偏移量的檔案是否存在 (未實現)
  int checkAccOffset();

  // 寫入加速度計的偏移數據到設定檔案，並存儲為JSON格式
  bool insertAccOffset(float accelOffsetX, float accelOffsetY,
                       float accelOffsetZ);

  // 從設定檔案中讀取加速度計的偏移數據
  bool ReadAccOffset(float* accelOffsetX, float* accelOffsetY,
                     float* accelOffsetZ);

  // 檢查指定 MPU ID 的設定檔案是否存在 (具體實現)
  bool checkSettingJsonExist(String mpu_id);

  // 從 SPIFFS 檔案中讀取加速度計的偏移數據
  void readFileFromSPIFFS(float* accelOffsetX, float* accelOffsetY,
                          float* accelOffsetZ);

  // 寫入 JSON 資料到 SPIFFS 檔案
  void writeFileToSPIFFS(String data);
};

#endif




// #ifndef SETTINGJSON_H
// #define SETTINGJSON_H
// #include <Arduino.h>
// #include <ArduinoJson.h>
// #include <FS.h>
// #include <SPI.h>
// #include <stdio.h>
// #include <stdlib.h>

// #include "SPIFFS.h"
// #define FORMAT_SPIFFS_IF_FAILED true

// class iotSettingJson {  // The class
//  public:
//   int rc;

//   String settingFullFilename;

//   // static int settingJson_open(const char* filename);
//   static bool check_settingJson_exist(String mpu);
//   int checkAccOffset();
//   bool insertAccOffset(float accelOffsetX, float accelOffsetY,
//                        float accelOffsetZ);
//   bool ReadAccOffset(float* accelOffsetX, float* accelOffsetY,
//                      float* accelOffsetZ);
//   bool checkSettingJsonExist(String mpu_id);
//   void readFileFromSPIFFS(float* accelOffsetX, float* accelOffsetY,
//                           float* accelOffsetZ);
//   void writeFileToSPIFFS(String data);
//   // bool dbClose();
// };

// #endif