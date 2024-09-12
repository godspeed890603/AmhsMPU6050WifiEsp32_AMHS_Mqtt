#ifndef IOTWEBACTION_H
#define IOTWEBACTION_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>


// #include "ReadSetting.h"


class iotWebAction{
    public:
        
        int httpTimeout=120;

        int uploadData(String webHtml);
        iotWebAction();
        // iotWebAction(ReadSetting tempSetting);
        String getUploadWebHtml();
        ~iotWebAction();
};

#endif /* IOTWEBACTION_H */
