#include <string.h>
#include <stdlib.h>
#include <mongo.h>
#include "users.h"

static char* new_str(const char* str, int len) {
  char* new_s = (char*)malloc(len);
  strcpy(new_s, str);
  return new_s;
}

static const char* get_field_value(bson_iterator* iterator, mongo_cursor* cursor, char* field) {
  if(bson_find(iterator, mongo_cursor_bson(cursor), field)) {
    return bson_iterator_string(iterator);
  }else{
    return NULL;
  }
}

int users_search(User* users, int limit) {
  int pos = 0;
  mongo_cursor cursor[1];
  bson_iterator iterator[1];
  db_init_cursor(cursor, "users");
  // mongo_cursor_set_query
  mongo_cursor_set_limit(cursor, limit);

  while(mongo_cursor_next(cursor) == MONGO_OK) {
    const char* s;

    s = get_field_value(iterator, cursor, "name");
    if (s) {
      users[pos].name_len = strlen(s);
      users[pos].name = new_str(s, users[pos].name_len);
    } else {
      users[pos].name_len = 0;
      users[pos].name = NULL;
    }

    s = get_field_value(iterator, cursor, "bio");
    if (s) {
      users[pos].bio_len = strlen(s);
      users[pos].bio = new_str(s, users[pos].bio_len);
    } else {
      users[pos].bio_len = 0;
      users[pos].bio = NULL;
    }

    pos++;
  }

  mongo_cursor_destroy(cursor);
  return pos;
}

void users_free(User* users, int count) {
  int i;
  for(i = 0; i < count; i++) {
    free(users[i].name);
    free(users[i].bio);
  }
}
