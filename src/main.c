#include <stdio.h>
#include <stdlib.h>
#define HTTPSERVER_IMPL
#include "httpserver.h"
#include "string.h"
#define i_import
#include "stc/cstr.h"
#define i_import
#include "stc/csview.h"
#include "template.h"
#include <stdbool.h>
#include "sign.h"
#include "entries.h"

typedef enum {
  HTTP_OK        = 200,
  HTTP_NOT_FOUND = 404,
} http_status;

// NOTE: this function is a helper to print private and public key.
// FIXME: remove or disable when dvv is used in prod
static void print_hex(const char *label, const uint8_t *bytes, size_t length)
{
  printf("%s: ", label);
  for (size_t index = 0; index < length; index++)
    {
      printf("%02x", bytes[index]);
    }
  printf("\n");
  if (fflush(stdout) != 0) {
    printf("ERROR: could not flush to stdout");
  }
}

static void respond(struct http_request_s *request, int status, cstr page)
{
  struct http_response_s *response = http_response_init();
  http_response_status(response, status);
  http_response_header(response, "Content-Type", "text/html; charset=utf-8");
  http_response_header(response, "Cache-Control", "no-store");
  http_response_body(response, cstr_str(&page), (int)cstr_size(&page));
  http_respond(request, response);
  cstr_drop(&page);
}

void handle_request(struct http_request_s *request)
{

  /* get http method and target */
  struct http_string_s raw_method = http_request_method(request);
  struct http_string_s raw_target = http_request_target(request);
  csview method = c_sv(raw_method.buf, raw_method.len);
  csview target = c_sv(raw_target.buf, raw_target.len);

  if (csview_equals(method, "GET") && csview_equals(target, "/"))
    {
      cstr index = render_index("Don't vouch, VERIFY!");
      respond(request, HTTP_OK, index);
    }

  // TODO: make user handler generic
  else if (csview_equals(method, "GET") && csview_equals(target, "/@cashmere"))
    {
      const char *projects[] = {"Project 1", "Project 2"};
      cstr user = render_user( "cashmere", "Hello, Sailor!", projects, 2);
      respond(request, HTTP_OK, user);

      // FIXME: change to secure way to generate and store secret/public key
      uint8_t secret_key[64];
      uint8_t public_key[32];
      if (!generate_keys(secret_key, public_key))
        {
          if (fflush(stdout) != 0) {
            printf("failed to generate keys\n");
          }
        }
      print_hex("public_key", public_key, sizeof(public_key));
      print_hex("secret_key", secret_key, sizeof(secret_key));
    }
  else
    {
      /* respond with 404 not found for any other page */
      cstr not_found = render_error(HTTP_NOT_FOUND);
      respond(request, HTTP_NOT_FOUND, not_found);
    }

}

const int PORT = 8000;

int main(void)
{
  bool conf_file = path_exists("./data/test.conf");
  if (conf_file == true) {
    printf("ERROR: conf file does not exist. please create one.\n");
    fflush(stdout);
    return 1;
  }
  struct http_server_s *server = http_server_init(PORT, handle_request);
  http_server_listen(server);
  return 0;
}
