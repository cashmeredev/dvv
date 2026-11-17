#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <stdio.h>

struct Page {
  char *body;
  size_t size;
};

/*
  buffer:     the memory the finished page is collected in
  snippet:    one fixed piece of HTML from the .h.tt template
  length:     how many bytes that snippet has
  expression: the value that gets filled in at a %...% spot
*/
#define write(buffer, snippet, length) cstr_append_n(&(buffer), (snippet), (length))
#define INT(expression) cstr_append_fmt(&OUT, "%d", expression);
#define STR(expression) cstr_append(&OUT, expression);

#define TEMPLATE_BEGIN \
cstr OUT = cstr_init();

#define TEMPLATE_END   \
  return OUT;

static cstr render_error(int code)
{
  TEMPLATE_BEGIN
#include "error_page_template.h"
  TEMPLATE_END
}

static cstr render_index(const char *title)
{
  TEMPLATE_BEGIN
#include "index_template.h"
  TEMPLATE_END
}

static cstr render_user(
  const char *username,
  const char *description,
  const char **projects,
  size_t project_count
)
{
  TEMPLATE_BEGIN
#include "user_template.h"
  TEMPLATE_END
}

#undef TEMPLATE_BEGIN
#undef TEMPLATE_END
#undef STR
#undef INT
#undef write

#endif
