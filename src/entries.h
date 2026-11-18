#include <sys/stat.h>

bool path_exists(const char *path) {
    struct stat buf;
    if (stat(path, &buf) == 1) {
      return false;
    }
    else {
      return true;
    }
}
