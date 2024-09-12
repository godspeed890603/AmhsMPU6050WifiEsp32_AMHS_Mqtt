#include "iotWebAction.h"

// #include "const.h"

iotWebAction::iotWebAction() {}

iotWebAction::~iotWebAction() {
  // delete BYZ08Data;
}



int iotWebAction::uploadData(String webHtml) {
 
  int httpCode;

  WiFiClient client;
  HTTPClient http;

  //Serial.println(webHtml);
  // Serial.println(" ");

  http.setTimeout(httpTimeout);
  client.setTimeout(httpTimeout);
  if (http.begin(client, webHtml)) {  // HTTP
                                      // if (http.begin(webHtml)) {  // HTTP
    //Serial.print("[HTTP] GET start...\n");
    //  start connection and send HTTP header
    httpCode = http.GET();
   // Serial.print("[HTTP] GET end...\n");
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        //Serial.print("http.getString()\n");
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n",
                    http.errorToString(httpCode).c_str());
    }
    // http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  http.end();
  Serial.print(" http.end();\n");
  return httpCode;
}