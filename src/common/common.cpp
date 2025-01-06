#include "common.h"
// Wifi & Upload String
// const char* ssid = "Ktweety";
// const char* pwd = "28782878ab";
// IPAddress ip(172, 20, 10, 7);       // 设置为你想要的静态IP地址
// IPAddress subnet(255, 255, 255, 240);   // 设置子网掩码
// IPAddress gateway(172, 20, 10,1);  // 设置网关IP地址

// const char* ssid = "AMHS_WIFI";
// const char* pwd = "hsd5052880";
// IPAddress ip(172, 27, 63, 157);       // 设置为你想要的静态IP地址
// IPAddress subnet(255, 255, 254, 0);   // 设置子网掩码
// IPAddress gateway(172, 27, 63, 254);  // 设置网关IP地址

// fab use
// const char* ssid = "CIM";
// const char* pwd = "hsd5052880";
// IPAddress ip(172, 27, 17, 200);       // 设置为你想要的静态IP地址
// IPAddress subnet(255, 255, 224, 0);   // 设置子网掩码
// IPAddress gateway(172, 27, 31, 254);  // 设置网关IP地址

// // // //OFFICE use
const char* ssid = "CIM_WIFI";
const char* pwd = "hsd@5052880";
// char* ssid = "CIM_WIFI";
// char* pwd = "hsd@5052880";
IPAddress ip(172, 27, 17, 234);       // 设置为你想要的静态IP地址
IPAddress subnet(255, 255, 224, 0);   // 设置子网掩码
IPAddress gateway(172, 27, 31, 254);  // 设置网关IP地址

// /**
//  * @brief Accelerometer range options
//  *
//  * Allowed values for `setAccelerometerRange`.
//  */
// typedef enum {
//   MPU6050_RANGE_2_G = 0b00,  ///< +/- 2g (default value)
//   MPU6050_RANGE_4_G = 0b01,  ///< +/- 4g
//   MPU6050_RANGE_8_G = 0b10,  ///< +/- 8g
//   MPU6050_RANGE_16_G = 0b11, ///< +/- 16g
// } mpu6050_accel_range_t;
// mpu6050_accel_range_t mpu_accel_range = MPU6050_RANGE_8_G;

// /**
//  * @brief Gyroscope range options
//  *
//  * Allowed values for `setGyroRange`.
//  */
// typedef enum {
//   MPU6050_RANGE_250_DEG,  ///< +/- 250 deg/s (default value)
//   MPU6050_RANGE_500_DEG,  ///< +/- 500 deg/s
//   MPU6050_RANGE_1000_DEG, ///< +/- 1000 deg/s
//   MPU6050_RANGE_2000_DEG, ///< +/- 2000 deg/s
// } mpu6050_gyro_range_t;
// mpu6050_gyro_range_t mpu_gyro_range = MPU6050_RANGE_1000_DEG;

// /**
//  * @brief Digital low pass filter bandthwidth options
//  *
//  * Allowed values for `setFilterBandwidth`.
//  */
// typedef enum {
//   MPU6050_BAND_260_HZ, ///< Docs imply this disables the filter
//   MPU6050_BAND_184_HZ, ///< 184 Hz
//   MPU6050_BAND_94_HZ,  ///< 94 Hz
//   MPU6050_BAND_44_HZ,  ///< 44 Hz
//   MPU6050_BAND_21_HZ,  ///< 21 Hz
//   MPU6050_BAND_10_HZ,  ///< 10 Hz
//   MPU6050_BAND_5_HZ,   ///< 5 Hz
// } mpu6050_bandwidth_t;
// mpu6050_bandwidth_t mpu_bandwidth = MPU6050_BAND_5_HZ;

// /**
//  * @brief Accelerometer high pass filter options
//  *
//  * Allowed values for `setHighPassFilter`.
//  */
// typedef enum {
//   MPU6050_HIGHPASS_DISABLE,
//   MPU6050_HIGHPASS_5_HZ,
//   MPU6050_HIGHPASS_2_5_HZ,
//   MPU6050_HIGHPASS_1_25_HZ,
//   MPU6050_HIGHPASS_0_63_HZ,
//   MPU6050_HIGHPASS_UNUSED,
//   MPU6050_HIGHPASS_HOLD,
// } mpu6050_highpass_t;
// mpu6050_highpass_t mpu_highpass = MPU6050_HIGHPASS_0_63_HZ;

m5::M5Unified::config_t cfg = {};  // 定義全域變數，初始化為空

void ledDisplay(bool* led_on) {
  if (*led_on == true) {
    digitalWrite(ledPin, HIGH);
    *led_on = false;
  } else {
    digitalWrite(ledPin, LOW);
    *led_on = true;
  }
}

void ledAlarm(int loopCount, int delayTime) {
  int i;
  for (i = 0; i < loopCount; i++) {
    digitalWrite(ledPin, HIGH);  // 將LED引腳設置為高電位（點亮LED）
    delay(delayTime);            // 延遲1秒
    Serial.println("....");
    digitalWrite(ledPin, LOW);  // 將LED引腳設置為低電位（關閉LED）
    delay(delayTime);           // 延遲1秒
  }
}

void M5Display(String text, float fontRatio) {
  AtomS3.Display.clear();
  AtomS3.Display.setTextColor(GREEN);
  AtomS3.Display.setTextDatum(middle_center);
  AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
  AtomS3.Display.setTextSize(fontRatio);
  AtomS3.Display.drawString(text, AtomS3.Display.width() / 2,
                            AtomS3.Display.height() / 2);
}

void M5DisplayAcc(float x, float y, float z) {
  M5Display("Amhs XYZ", 0.9);

  AtomS3.Display.setTextColor(RED);
  AtomS3.Display.setTextSize(0.6);
  // M5.Lcd.setCursor(0, AtomS3.Display.height()-20);
  // M5.Lcd.printf("x:%0.2f y:%0.2f z:%0.2f\r\n", x, y, z);
  M5.Lcd.setCursor(0, AtomS3.Display.height() - 30);
  M5.Lcd.printf("x:%0.2f    y:%0.2f \r\n", x, y);
  // M5.Lcd.printf("x:%0.2f\r\n", x);
  // M5.Lcd.setCursor(0, AtomS3.Display.height()-20);
  // M5.Lcd.printf("y:%0.2f \r\n", y);
  M5.Lcd.setCursor(0, AtomS3.Display.height() - 15);
  M5.Lcd.printf("z:%0.2f\r\n", z);
}
