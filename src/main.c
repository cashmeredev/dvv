#include <stdio.h>
#include <stdlib.h>
#define HTTPSERVER_IMPL
#include "httpserver.h"

#define RESPONSE_BODY "Hello, World!"

struct Template {
  char *body;
  size_t template_size;
  int code;
};

struct Template make_template(void)
{
  /* build error page template template */
  struct Template template;
  template.code = 404;

  FILE *OUT = open_memstream(&template.body, &template.template_size);
#define write(out, data, n) fwrite(data, 1, n, out)
#define INT(x) fprintf(OUT, "%d", x);
#include "error_page_template.h"
#undef INT
#undef write
fclose(OUT);
return template;
}

void handle_request(struct http_request_s *request)
{

  struct Template templ = make_template();

  /* debug http method and target */
  struct http_string_s method = http_request_method(request);
  struct http_string_s target = http_request_target(request);
  printf("%s %s\n", method, target);

  struct Template template = make_template();
  struct http_response_s *response = http_response_init();
  http_response_status(response, 200);
  http_response_header(response, "Content-Type", "text/html; charset=utf-8");
  http_response_body(response, template.body, (int)template.template_size);
  http_respond(request, response);
  free(template.body);
}

int main(void)
{
  struct http_server_s *server = http_server_init(8000, handle_request);
  http_server_listen(server);
  return 0;
}
