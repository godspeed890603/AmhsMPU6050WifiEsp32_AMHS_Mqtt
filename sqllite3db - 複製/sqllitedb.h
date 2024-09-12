#ifndef IOTSQLITEDB_H
#define IOTSQLITEDB_H
#include <FS.h>
#include <SPI.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

class iotSqliteDB {  // The class
 public:
  // static char *zErrMsg;
  // static const char *data;
  sqlite3 *db1;
  sqlite3 *db2;
  int rc;
  // const char *data = "Callback function called";

  static int sdb_open(const char *filename, sqlite3 **db);
  static int dbcallback(void *data, int argc, char **argv, char **azColName);
  static String* dbAcccallback(void *data, int argc, char **argv, char **azColName);
  static int sdb_exec(sqlite3 *db, const char *sql);
  static int s1db_exec(sqlite3 *db, const char *sql);
  bool createTable();
  int checkAccOffset();
  bool insertAccOffset(float accelOffsetX, float accelOffsetY,
                       float accelOffsetZ);
  bool ReadAccOffset(float* accelOffsetX, float* accelOffsetY, float* accelOffsetZ);
  bool sinitialSqliteDB(String mpu_id);
  bool dbClose();
};

#endif