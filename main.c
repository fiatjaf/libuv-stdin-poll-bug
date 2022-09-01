#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

void cb(uv_poll_t *handle, int status, int events) {
  char *line = NULL;
  size_t len = 0;
  getline(&line, &len, stdin);
  fprintf(stdout, "GOT LINE: `%s`\n", line);
  fflush(stdout);
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
