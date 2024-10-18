#ifndef UUIDGENERATOR_H
#define UUIDGENERATOR_H

#include <Arduino.h>

class UUIDGenerator {
public:
    // 構造函數
    UUIDGenerator();

    // 根據 MAC 地址生成 UUID
    String generateFromMAC();

    // 隨機生成 UUID
    String generateRandomUUID();

private:
    // 生成隨機的 UUID 部分
    String randomHexBytes(int length);

    // 獲取設備 MAC 地址
    String getMacAddress();
};

#endif  // UUIDGENERATOR_H
