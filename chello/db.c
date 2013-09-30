#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include <mongo.h>

static char* db_name = NULL;

void db_establish_connection(const char* _db_name) {
  static mongo db[1];
  uint64_t status = mongo_client(db, "127.0.0.1", 27017);
  if(status != MONGO_OK) {
    printf("failed to connect mongo\n");
    exit(-1);
  }
  if(!_db_name) {
    printf("require db name\n");
    exit(-1);
  }
  db_name = (char*)_db_name;
  mongo_destroy(db);
}

static __thread mongo db[1];
static __thread int db_init = 0;

void db_init_cursor(void* cursor, const char* documents_name) {
  if (!db_init) {
    uint64_t status = mongo_client(db, "127.0.0.1", 27017);
    if (status != MONGO_OK) {
      printf("connection fail\n");
      exit(-1);
    } else {
      // printf("connected\n");
    }
    db_init = 1;
  }

  char* cursor_spec = (char*)malloc(strlen(db_name) + strlen(documents_name) + 2);
  sprintf(cursor_spec, "%s.%s", db_name, documents_name);
  mongo_cursor_init((mongo_cursor*)cursor, db, cursor_spec);
  free(cursor_spec);
}
