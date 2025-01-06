
#include ".\AmhsMPU6886WifiM5Esp32_AMHS_Mqtt.h"

iotWifi* fabIotWifi;
iotMPU6886* mpu_0x68;
// iotMPU6886* mpu_0x69;
MQTTClient mqttClientHandler;
String service = "service_vibration";

unsigned long startTime1 = 0;
unsigned long startTime2 = 0;
unsigned long endTime = 0;
unsigned long timeDiff;
long otaTime = 0;
long otaEndTime = 0;

void taskCreateMPU6886PinnedToCore() {
  // 建立兩個Task，分別指派到核心1和核心2
  xTaskCreatePinnedToCore(MPU6886_task,   // Task函式
                          "Task Core 1",  // 任務名稱
                          10000,          // 堆疊大小 (bytes)
                          NULL,           // 傳遞給Task的參數
                          1,              // 優先順序
                          NULL,           // Task的句柄
                          1               // 核心編號 (0 or 1)
  );
}

void taskCreateWifiPinnedToCore() {
  xTaskCreatePinnedToCore(wifiTask_0x68,      // Task函式
                          "Wi-Fi Task 0x68",  // 任務名稱
                          10000,              // 堆疊大小 (bytes)
                          NULL,               // 傳遞給Task的參數
                          1,                  // 優先順序
                          NULL,               // Task的句柄
                          0                   // 核心編號 (0 or 1)
  );
  delay(500);
  // xTaskCreatePinnedToCore(wifiTask_0x69,      // Task函式
  //                         "Wi-Fi Task_0x69",  // 任務名稱
  //                         10000,              // 堆疊大小 (bytes)
  //                         NULL,               // 傳遞給Task的參數
  //                         1,                  // 優先順序
  //                         NULL,               // Task的句柄
  //                         0                   // 核心編號 (0 or 1)
  // );
}

void setup() {
  Serial.begin(115200);
  // Serial.println("initial WiFi...1");
  pinMode(ledPin, OUTPUT);  // 設置LED引腳為輸出
                            // Serial.println("initial WiFi...");
                            // Serial.println(ssid);
                            // Serial.println(pwd);
                            // Serial.println("initial WiFi...0");
  fabIotWifi = new iotWifi(ssid, pwd, ip, subnet, gateway);
  // Serial.println("initial WiFi...1");
  fabIotWifi->initialWifi();

  // display re-calabration message
  // auto cfg = M5.config();
  cfg = M5.config();
  Serial.println("create cfg completed");
  AtomS3.begin(cfg);
  Serial.println("create AtomS3.begin(cfg) completed");

  M5Display("Amhs XYZ",0.8);

  // AtomS3.Display.clear();
  // AtomS3.Display.setTextColor(GREEN);
  // AtomS3.Display.setTextDatum(middle_center);
  // AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
  // AtomS3.Display.setTextSize(0.8);
  // AtomS3.Display.drawString("Amhs XYZ", AtomS3.Display.width() / 2,
  //                           AtomS3.Display.height() / 2);
  // Serial.println("Click BtnA to Test");

  iotSettingJson* mpuSettingJson = new iotSettingJson();
  // Serial.println("create AtomS3 completed");

  
  //  Serial.println("initial WiFi...2");
  delay(2000);
  mqttClientHandler.setServiceName(service);
  mqttClientHandler.CreatePubSubTopic();
  mqttClientHandler.connect();  // 連接到 MQTT broker
  delay(100);
  // 建立佇列
  mutex = xSemaphoreCreateMutex();
  dataQueue_0x68 = xQueueCreate(QUEUE_SIZE, sizeof(char*));
  // dataQueue_0x69 = xQueueCreate(QUEUE_SIZE, sizeof(char*));

  Serial.println("create task");
  // 雙核心工作分配
  taskCreateMPU6886PinnedToCore();
  Serial.println("create task completed");
}

void loop() {
  // // Serial.println("main loop started");
  // otaEndTime = micros();
  // if (otaEndTime - otaTime > 0.05 * sencond_1) {
  //   xSemaphoreTake(mutex, portMAX_DELAY);
  //   // Serial.println("main loop started");
  //   ArduinoOTA.handle();
  //   xSemaphoreGive(mutex);
  //   otaTime = otaEndTime;
  // }
  // delete file and restart
  //Serial.println("Loop");
  AtomS3.update();
  if (AtomS3.BtnA.wasPressed()) {
    // AtomS3.Display.clear();
    // AtomS3.Display.drawString("Pressed", AtomS3.Display.width() / 2,
    //                           AtomS3.Display.height() / 2);
    // Serial.println("Pressed");
    mpuSettingJson->DeleteOffsetFile();
    // Serial.println("Released");
    AtomS3.Display.clear();
    delay(100);
    ESP.restart();
  }
  // if (AtomS3.BtnA.wasReleased()) {
  //   AtomS3.Display.clear();
  //   AtomS3.Display.drawString("Released", AtomS3.Display.width() / 2,
  //                             AtomS3.Display.height() / 2);
  // }
}

// 核心1的Task函式
void MPU6886_task(void* parameter) {
  // char buffer[50];
  // long otaTime = 0;
  // int n;
  mpu_0x68 = new iotMPU6886(0x68);
  Serial.println("mpu_0x68 initial OK");
  // mpu_0x69 = new iotMPU6886(0x69);

  while (1) {
    if (WiFi.status() == WL_CONNECTED) {
      mpu_0x68->ipAddr = fabIotWifi->ipAdress;
      mpu_0x68->macAddress = fabIotWifi->macAddress;
      // mpu_0x69->ipAddr = fabIotWifi->ipAdress;
      break;
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }

  // MPU6886 0x68 & 0x69初始化後,
  // 啟動上傳程序
  taskCreateWifiPinnedToCore();

  vTaskDelay(pdMS_TO_TICKS(1000));
  bool* bhttp_led = new bool(true);
  long ledtime;
  while (1) {
    // Serial.println("Task running on Core 1");
    endTime = micros();
    mpu_0x68->rssi = fabIotWifi->getWifiRssi();
    mpu_0x68->getMPU6886Event();
    // mpu_0x69->rssi = fabIotWifi->getWifiRssi();
    // mpu_0x69->getMPU6886Event();

    // if (WiFi.status() == WL_CONNECTED) {
    if (endTime - startTime1 >
        mpu_loop_scan_time * sencond_1) {  // upload period
      // 取得互斥鎖
      // Serial.println("Core 1 xSemaphoreTake...");
      xSemaphoreTake(mutex, portMAX_DELAY);
      //   0x68寫入資料到佇列
      // Serial.println("Core 1 Send mpu_0x68 Start...");
      sendDataToWifi(dataQueue_0x68, mpu_0x68);
      // Serial.println("Core 1 Send mpu_0x68 End...");
      MPU6886DataReset(mpu_0x68);
      // vTaskDelay(pdMS_TO_TICKS(2));
      //  0x69寫入資料到佇列
      //  寫入資料到佇列
      // Serial.println("Core 1 Send mpu_0x69 Start...");
      // sendDataToWifi(dataQueue_0x69, mpu_0x69);
      // Serial.println("Core 1 Send mpu_0x69 end...");
      // MPU6886DataReset(mpu_0x69);
      // 釋放互斥鎖

      xSemaphoreGive(mutex);
      // Serial.println("Core 1 xSemaphoreGive...");

      startTime1 = endTime;
      // printMemoryStats();
    }
    //}
    if (endTime - ledtime > 1 * sencond_1) {
      ledDisplay(bhttp_led);
      ledtime = endTime;
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

// 核心0的Task函式,Handle MPU6886_0x68
void wifiTask_0x68(void* parameter) {
  // unsigned long wificheckTime = 0;
  while (1) {
    endTime = micros();
    // Serial.println("Task running on Core 0");
    //  check wifi alive
    checkWifiAlive();
    // check upload data recieve for core0
    if (WiFi.status() == WL_CONNECTED) {
      // getQueuebyMpuAndUploadWeb(dataQueue_0x68);
      getQueuebyMpuAndUploadMqtt(dataQueue_0x68);
      // vTaskDelay(pdMS_TO_TICKS(500));
      // getQueuebyMpuAndUploadWeb(dataQueue_0x69);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// 核心0的Task函式,Handle MPU6886_0x69
// void wifiTask_0x69(void* parameter) {
//   // delay(10000);
//   unsigned long wificheckTime = 0;
//   while (1) {
//     endTime = micros();
//     // Serial.println("Task running on Core 0");
//     //  check wifi alive
//     checkWifiAlive();
//     // check upload data recieve for core0
//     if (WiFi.status() == WL_CONNECTED) {
//       // getQueuebyMpuAndUploadWeb(dataQueue_0x68);
//       //  vTaskDelay(pdMS_TO_TICKS(500));
//       getQueuebyMpuAndUploadWeb(dataQueue_0x69);
//     }
//     vTaskDelay(pdMS_TO_TICKS(500));
//   }
// }

void MPU6886DataReset(iotMPU6886* mpu) {
  // Serial.println("data reset");
  mpu->resetMPU6886Data();
}

void sendDataToWifi(QueueHandle_t queue, iotMPU6886* mpu) {
  // // 複製字串並取得指標

  try {
    // if (WiFi.status() == WL_CONNECTED) {
    // String webHtml = mpu->getMPU6886WebHtml();
    String MPU6886Json = mpu->getMPU6886Json();
    char* data = strdup(MPU6886Json.c_str());  // data 需要被 free 因為是用share
    // memory..所以在接收端free
    xQueueSend(queue, &data, portMAX_DELAY);

    MPU6886DataReset(mpu);

  } catch (const std::exception& e) {
    // std::cerr << e.what() << '\n';
    Serial.println("[]sysytem Error.. ESP.restart();");
    ESP.restart();
  }
}

void checkWifiAlive() {
  // if (endTime - startTime2 > 5 * sencond_1) {
  fabIotWifi->checkWifiStat();
  startTime2 = endTime;

  //}
}

void printMemoryStats() {
  // multi_heap_info_t heapInfo;
  // heap_caps_get_info(&heapInfo, MALLOC_CAP_INTERNAL);

  // Serial.print("Total heap size: ");
  // Serial.println(heapInfo.total_free_bytes + heapInfo.total_allocated_bytes);

  // Serial.print("Free heap size: ");
  // Serial.println(heapInfo.total_free_bytes);

  // Serial.print("Minimum free heap size: ");
  // Serial.println(heapInfo.minimum_free_bytes);

  // Serial.print("Largest free heap block: ");
  // Serial.println(heapInfo.largest_free_block);

  // Serial.print("Allocated heap blocks: ");

  // Serial.println();
}
//+++++++++++++++++++++++Mqtt+++++++++++++++++++++++++++++++++++++++++++++++++++
void getQueuebyMpuAndUploadMqtt(QueueHandle_t queue) {
  char* receivedData;
  BaseType_t pd = pdFALSE;

  mqttClientHandler.loop();  // 處理 MQTT 事件

  delay(500);  // 每 5 秒發布一次資料

  try {
    // Take the mutex to ensure exclusive access
    xSemaphoreTake(mutex, portMAX_DELAY);

    // Receive data from the queue
    pd = xQueueReceive(queue, &receivedData, 0);
    xSemaphoreGive(mutex);  // Release the mutex after receiving data

    // If no data was received, exit
    if (pd != pdTRUE) {
      return;
    }

    // Convert the received data into a string
    String mqttData = String(receivedData);
    Serial.println(mqttData);
    free(receivedData);  // Free the received data from the queue
    if (mqttClientHandler.publishData(mqttData)) {
      Serial.println("MQTT data sent successfully.");
    } else {
      Serial.println("MQTT data sent failture....");
    }

  } catch (const std::exception& e) {
    Serial.println(
        "[getQueuebyMpuAndUploadMqtt] System error, restarting ESP...");
    ESP.restart();  // Restart if an exception occurs
  }
}