#include <mongo.h>

typedef struct {
  char* name;
  int name_len;
  char* bio;
  int bio_len;
} User;

// return len
int users_search(mongo* conn, User* users, int count);

void users_free(User* users, int count);
