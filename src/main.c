#include <stdio.h>
#include <stdlib.h>
#define HTTPSERVER_IMPL
#include "httpserver.h"

#define RESPONSE_BODY "Hello, World!"

void handle_request(struct http_request_s *request)
{
  /* build error page template template */
  char *body = NULL;
  size_t size = 0;
  FILE *OUT = open_memstream(&body, &size);
  int code = 404;
#define write(out, data, n) fwrite(data, 1, n, out)
#define INT(x) fprintf(OUT, "%d", x);
#include "error_page_template.h"
#undef INT
#undef write
fclose(OUT);

  struct http_response_s *response = http_response_init();
  http_response_status(response, 200);
  http_response_header(response, "Content-Type", "text/html; charset=utf-8");
  http_response_body(response, body, (int)size);
  http_respond(request, response);
  free(body);
}

int main(void)
{
  struct http_server_s *server = http_server_init(8000, handle_request);
  http_server_listen(server);
  return 0;
}
