#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include <mongo.h>

static mongo db[1];
static char* db_name = NULL;

void db_establish_connection(const char* _db_name) {
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
}

void db_init_cursor(void* cursor, const char* documents_name) {
  char* cursor_spec = (char*)malloc(strlen(db_name) + strlen(documents_name) + 2);
  sprintf(cursor_spec, "%s.%s", db_name, documents_name);
  mongo_cursor_init((mongo_cursor*)cursor, db, cursor_spec);
  free(cursor_spec);
}

void db_close_connection(void* conn) {
  mongo_destroy(conn);
}
