#pragma once
#include "db.h"

typedef struct {
  char* name;
  int name_len;
  char* bio;
  int bio_len;
} User;

// return len
int users_search(User* users, int count);

void users_free(User* users, int count);
