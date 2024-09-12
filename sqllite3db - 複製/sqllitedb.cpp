#include "./sqllitedb.h"

char *zErrMsg;
const char *data = "Callback function called";

int iotSqliteDB::dbcallback(void *data, int argc, char **argv,
                            char **azColName) {
  int i;
  Serial.printf("%s: ", (const char *)data);
  for (i = 0; i < argc; i++) {
    Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  Serial.printf("\n");
  return 0;
}

int iotSqliteDB::sdb_open(const char *filename, sqlite3 **db) {
  int rc = sqlite3_open(filename, db);
  if (rc) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
    return rc;
  } else {
    Serial.printf("Opened database successfully\n");
  }
  Serial.println(rc);
  return rc;
}

int iotSqliteDB::sdb_exec(sqlite3 *db, const char *sql) {
  Serial.println(sql);
  long start = micros();
  int rc = sqlite3_exec(db, sql, dbcallback, (void *)data, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    Serial.printf("Operation done successfully\n");
  }
  Serial.print(F("Time taken:"));
  Serial.println(micros() - start);
  return rc;
}

int iotSqliteDB::s1db_exec(sqlite3 *db, const char *sql) {
  Serial.println(sql);
  long start = micros();
  int rc = sqlite3_exec(db, sql, dbcallback, (void *)data, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    Serial.printf("Operation done successfully\n");
  }
  Serial.print(F("Time taken:"));
  Serial.println(micros() - start);
  return rc;
}

bool iotSqliteDB::sinitialSqliteDB(String mpu_id) {
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("Failed to mount file system");
    return false;
  }

  char fullDBFileName[50];
  sprintf(fullDBFileName, "/spiffs/%s.db", mpu_id.c_str());
  // list SPIFFS contents
  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("- failed to open directory");
    return false;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return false;
  }
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());

      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
    Serial.println("root.openNextFile();");
  }
  // remove existing file
  // SPIFFS.remove("/test1.db");
  sqlite3_initialize();

  if (sdb_open(fullDBFileName, &db1) == 0) {
  }
  return true;
}

bool iotSqliteDB::createTable() {
  // 如果TABLE不存在就create table
  Serial.println("CREATE TABLE..");
  rc = sdb_exec(db1,
                "CREATE TABLE IF NOT EXISTS ACCELOFFSET (accelOffsetX "
                "CHAR(25), accelOffsetY CHAR(25), accelOffsetZ CHAR(25));");
  Serial.println(rc);
  if (rc != SQLITE_OK) {
    sqlite3_close(db1);
    return false;
  }
  return true;
}

int iotSqliteDB::checkAccOffset() {
  sqlite3_stmt *stmt;
  int count;
  rc =
      sqlite3_prepare_v2(db1, "SELECT COUNT(*) FROM ACCELOFFSET", -1, &stmt, 0);

  if (rc != SQLITE_OK) return rc;
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) return rc;

  count = sqlite3_column_int(stmt, 0);
  Serial.print("Total rows: ");
  Serial.println(count);
  sqlite3_finalize(stmt);
  return count;
  // if (rc == SQLITE_OK) {
  //   rc = sqlite3_step(stmt);
  //   if (rc == SQLITE_ROW) {
  //     count = sqlite3_column_int(stmt, 0);
  //     Serial.print("Total rows: ");
  //     Serial.println(count);
  //   }
  //   sqlite3_finalize(stmt);
  // } else {
  //   Serial.println("Db Error");
  //   return -99999;
  // }
  // return count;
}

bool iotSqliteDB::insertAccOffset(float accelOffsetX, float accelOffsetY,
                                  float accelOffsetZ) {
  // char sqlBuf[500];

  // sprintf(sqlBuf, "INSERT INTO ACCELOFFSET VALUES ('%f', '%f','%f');",
  //         accelOffsetX, accelOffsetY, accelOffsetZ);

  // Serial.println(sqlBuf);

  // 預備 INSERT 語句
  const char *insertSql =
      "INSERT INTO ACCELOFFSET (accelOffsetX, accelOffsetY, accelOffsetZ) "
      "VALUES (?, ?, ?)";
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db1, insertSql, -1, &stmt, 0);
  if (rc == SQLITE_OK) {
    // 設定參數值
    // float floatValue = 3.14159;
    char floatString_X[10];
    char floatString_Y[10];
    char floatString_Z[10];
    dtostrf(accelOffsetX, 6, 4, floatString_X);
    const char *value1 = floatString_X;
    Serial.print("*value1");
    Serial.println(floatString_X);

    dtostrf(accelOffsetY, 6, 4, floatString_Y);
    const char *value2 = floatString_Y;
    Serial.print("*value2");
    Serial.println(floatString_Y);

    dtostrf(accelOffsetZ, 6, 4, floatString_Z);
    const char *value3 = floatString_Z;
    Serial.print("*value3");
    Serial.println(floatString_Z);

    sqlite3_bind_text(stmt, 1, value1, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, value2, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, value3, -1, SQLITE_STATIC);

    // 執行 INSERT 語句
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
      Serial.println("插入成功");
    } else {
      Serial.println("插入失敗");
    }

    // 重置語句以便再次使用
    sqlite3_reset(stmt);
  } else {
    Serial.println("預備語句時發生錯誤");
  }

  // 釋放語句資源
  sqlite3_finalize(stmt);
  return true;
}

bool iotSqliteDB::ReadAccOffset(float *accelOffsetX, float *accelOffsetY,
                                float *accelOffsetZ) {
  // 執行SELECT查詢
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db1, "SELECT * FROM ACCELOFFSET", -1, &stmt, 0);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char *column1 = sqlite3_column_text(stmt, 0);
    const unsigned char *column2 = sqlite3_column_text(stmt, 1);
    const unsigned char *column3 = sqlite3_column_text(stmt, 2);

    // 進行需要的操作，例如列印值
    Serial.print("Column1: ");
    Serial.println((const char *)column1);
    Serial.print("Column2: ");
    Serial.println((const char *)column2);
    Serial.print("Column3: ");
    Serial.println((const char *)column3);

    *accelOffsetX = atof((const char *)column1);
    *accelOffsetY = atof((const char *)column2);
    *accelOffsetZ = atof((const char *)column3);
    Serial.print("accelOffsetX: ");
    Serial.println(*accelOffsetX);
    Serial.print("accelOffsetY: ");
    Serial.println(*accelOffsetY);
    Serial.print("accelOffsetZ: ");
    Serial.println(*accelOffsetZ);
  }

  return true;
}

bool iotSqliteDB::dbClose() {
  sqlite3_close(db1);
  return true;
}
