#include <stdio.h>
#define HTTPSERVER_IMPL
#include "httpserver.h"

#define RESPONSE_BODY "Hello, World!"

void handle_request(struct http_request_s *request)
{
  struct http_response_s *response = http_response_init();
  http_response_status(response, 200);
  http_response_header(response, "Content-Type", "text/html; charset=utf-8");
  http_response_body(response, RESPONSE_BODY, sizeof(RESPONSE_BODY));
  http_respond(request, response);
}

int main(void)
{
  struct http_server_s *server = http_server_init(8000, handle_request);
  http_server_listen(server);
  return 0;
}
