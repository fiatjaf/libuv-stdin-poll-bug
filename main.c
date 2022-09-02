#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <sys/epoll.h>

void readstuff() {
  char *line = NULL;
  size_t len = 0;
  getline(&line, &len, stdin);
  fprintf(stdout, "GOT LINE: `%s`\n", line);
  fflush(stdout);
}

void cb(uv_poll_t *handle, int status, int x) {
  readstuff();

  int epfd = epoll_create1(0);
  struct epoll_event event;
  event.events = EPOLLIN;
  if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event)) {
    exit(3);
  }

  struct epoll_event* events = malloc(sizeof event);
  int triggers = epoll_wait(epfd, events, 1, 1000);

  fprintf(stdout, "TRIGGERS: %d\n", triggers);
  fflush(stdout);

  // readstuff();
}

int main() {
  fprintf(stdout, "STARTING\n");
  fflush(stdout);
  uv_poll_t* handle = (uv_poll_t*) malloc(uv_handle_size(UV_POLL));
  if (0 != uv_poll_init(uv_default_loop(), handle, 0)) { return 1; };
  if (0 != uv_poll_start(handle, UV_READABLE, cb)) { return 2; };
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}
