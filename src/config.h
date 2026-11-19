#include <sys/stat.h>
#include <stdio.h>

bool config_path_exists(const char *path) {
    struct stat buf;
    if (stat(path, &buf) == -1) {
      return false;
    }
    else {
      return true;
    }
}

cstr config_read_file(const char *path)
{
  cstr config = cstr_from_file(path);
  return config;
}
