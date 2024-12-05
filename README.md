## 目的說明
1.使用在AGV、Stocker Crane走行、伺服馬達震動資料收集<br>
2.根據收集資料的物件分析資料特性，確定收集地的區間<br>
如，AGV走行只要是為了解高架地板的健康度，其中有兩種不同的地板設置，因此需要資料取樣區間需要很短，<br>
藉以分界兩地板的對AGV走行的影響，這一種行為比較像是測量<br>
Stocker Crane走行、伺服馬達震動資料收集屬於監控行為!只要區間利用資料壓縮的方式，即可以得到需要的異常值!!
或是可以將資料收集!透過大數據分析得到一個震動值的趨勢，藉以預測佈件是否開始老化異常

## 架構說明
1.採用RTOS的方式<br><br>
2.xTaskCreatePinnedToCore mpu6050_task、wifiTask_0x68，將讀取MPU6050與連結IOTView(mqtt)方開，藉以更有資料收集與傳輸<br><br>
3.兩個TASK QueueHandle_t與SemaphoreHandle_t來協調作業<br><br>
4.mpu6050需要做水平補正<br><br>
6.原本只用SQLLITE儲存補正值，後續改為JSON<br><br>
7.Mqtt subject<br>
serviceName在IOTView(mqtt)會有對應的程式(相同名稱)來服務<br><br>
const String mqtt_request_topic_prefix = "request/iot/";<br><br>
const String mqtt_subscribe_topic_prefix = "response/iot/";<br><br>
mqtt_subscribe_topic =<
      mqtt_subscribe_topic_prefix + WiFi.macAddress() + serviceName;<br><br>
mqtt_request_topic =
      mqtt_request_topic_prefix + WiFi.macAddress() + serviceName;<br><br>
8.ESP32透過同一UUID發送與接收來識別data pair<br>
doc["correlation_id"] =  uuidGen.generateRandomUUID();  // 替換為實際的 correlation ID<br><br>
9.device<br><br>
https://github.com/godspeed890603/AmhsMPU6050WifiEsp32_AMHS_Mqtt/tree/master/Device<br><br>
10.result<br><br>
https://github.com/godspeed890603/AmhsMPU6050WifiEsp32_AMHS_Mqtt/tree/master/Result<br><br>





