#include <stdio.h>
#include <stdlib.h>
#define HTTPSERVER_IMPL
#include "httpserver.h"
#include "string.h"
#include "template.h"
#include <stdbool.h>


static bool matches(struct http_string_s received, const char *expected)
{
  size_t expected_length = strlen(expected);

  if ((size_t)received.len != expected_length)
    {
      return false;
    }

  if(memcmp(received.buf, expected, expected_length) != 0)
    {
      return false;
    }
  return true;
}

static void respond(struct http_request_s *request, int status, struct Page page)
{
  struct http_response_s *response = http_response_init();
  http_response_status(response, status);
  http_response_header(response, "Content-Type", "text/html; charset=utf-8");
  http_response_body(response, page.body, (int)page.size);
  http_respond(request, response);
  free(page.body);
}

void handle_request(struct http_request_s *request)
{

  /* get http method and target */
  struct http_string_s method = http_request_method(request);
  struct http_string_s target = http_request_target(request);

  if (matches(method, "GET") && matches(target, "/"))
    {
      struct Page index = render_index("Don't vouch, VERIFY!");
      respond(request, 200, index);
    }
  else
    {
      /* respond with 404 not found for any other page */
      struct Page not_found = render_error(404);
      respond(request, 404, not_found);
    }

}

int main(void)
{
  struct http_server_s *server = http_server_init(8000, handle_request);
  http_server_listen(server);
  return 0;
}
