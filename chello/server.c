#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "users.h"
#include "buffer.h"
#include <microhttpd.h>

Buffer render_index(User* users, int users_len) {
  static const char* init =
    "<!DOCTYPE html><html><head><title>C Hello</title></head>"
    "<body><h1>Home#index</h1>";
  static const char* finl =
    "</body></html>";

  int i;
  Buffer buf = buffer_new(300);

# define append(s, l) buffer_append(&buf, s, l)
  append(init, strlen(init));
  for(i = 0; i < users_len; i++) {
    append("<h3>", 4);
    if(users[i].name){
      append(users[i].name, users[i].name_len);
    }
    append("</h3>", 5);
    append("<p>", 3);
    if(users[i].bio){
      append(users[i].bio, users[i].bio_len);
    }
    append("</p>", 4);
  }
  append(finl, strlen(finl));
# undef append

  return buf;
}

static int serve(void *cls, struct MHD_Connection* http_conn,
  const char* url, const char* method,
  const char* version, const char* upload_data,
  size_t* upload_data_size, void** ptr) {

  // first time only headers are valid
  static int dummy;
  if (&dummy != *ptr) {
    *ptr = &dummy;
    return MHD_YES;
  }

  if (strcmp(method, "GET") == 0) {
    User users[100];
    int len = users_search(users, 100);
    Buffer index = render_index(users, len);
    struct MHD_Response* resp;

    users_free(users, len);
    resp = MHD_create_response_from_buffer(index.len, index.str, MHD_RESPMEM_MUST_FREE);
    MHD_queue_response(http_conn, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);

    return MHD_YES;
  } else {
    return MHD_NO;
  }
}

int main(int argc, char const *argv[]) {
  int port;
  struct MHD_Daemon* d;

  if (argc != 3) {
    printf("usage: ./server <port> <db_name>\n");
    return 1;
  } else {
    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
      printf("port should be ranged in 1-65535\n");
      return 1;
    }
  }

  db_establish_connection(argv[2]);

  // install int actions
  // action.sa_handler = finalize;
  // sigemptyset(&action.sa_mask);
  // action.sa_flags = 0;
  // sigaction (SIGINT, &action, NULL);

  d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, port, NULL, NULL,
                       &serve, NULL, MHD_OPTION_END);
  if (d != NULL) {
    pause();
    MHD_stop_daemon(d);
  }

  db_close_connection();
  return 0;
}
