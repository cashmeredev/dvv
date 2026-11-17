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
  fflush(stdout);
}

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
  struct http_string_s method = http_request_method(request);
  struct http_string_s target = http_request_target(request);

  if (matches(method, "GET") && matches(target, "/"))
    {
      cstr index = render_index("Don't vouch, VERIFY!");
      respond(request, 200, index);
    }

  // TODO: make user handler generic
  else if (matches(method, "GET") && matches(target, "/@cashmere"))
    {
      const char *projects[] = {"Project 1", "Project 2"};
      cstr user = render_user( "cashmere", "Hello, Sailor!", projects, 2);
      respond(request, 200, user);

      // FIXME: change to secure way to generate and store secret/public key
      uint8_t secret_key[64];
      uint8_t public_key[32];
      if (!generate_keys(secret_key, public_key))
        {
          printf("failed to generate keys\n");
          fflush(stdout);
        }
      print_hex("public_key", public_key, sizeof(public_key));
      print_hex("secret_key", secret_key, sizeof(secret_key));
    }
  else
    {
      /* respond with 404 not found for any other page */
      cstr not_found = render_error(404);
      respond(request, 404, not_found);
    }

}

int main(void)
{
  struct http_server_s *server = http_server_init(8000, handle_request);
  http_server_listen(server);
  return 0;
}
