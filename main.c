#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

void cb(uv_poll_t *handle, int status, int events) {
  char *line = NULL;
  size_t len = 0;

  fprintf(stdout, " ~ %d / %d ~", status, events);
  fflush(stdout);

  getline(&line, &len, stdin);
  fprintf(stdout, "GOT LINE (%zu): `%s`\n", len, line);
  fflush(stdout);

  if (line[1] == 'w') {
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    fprintf(stdout, "GOT LINE (%zu): `%s`\n", len, line);
    fflush(stdout);
  }
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
