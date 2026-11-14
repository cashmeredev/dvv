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
#define write(buffer, snippet, length) fwrite(snippet, 1, length, buffer)
#define INT(expression) fprintf(OUT, "%d", expression);
#define STR(expression) fputs(expression, OUT);

#define TEMPLATE_BEGIN \
struct Page page = {0}; \
FILE *OUT = open_memstream(&page.body, &page.size);
#define TEMPLATE_END \
fclose(OUT); \
  return page;

static struct Page render_error(int code)
{
  TEMPLATE_BEGIN
#include "error_page_template.h"
  TEMPLATE_END
}

static struct Page render_index(const char *title)
{
  TEMPLATE_BEGIN
#include "index_template.h"
  TEMPLATE_END
}

static struct Page render_user(
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
