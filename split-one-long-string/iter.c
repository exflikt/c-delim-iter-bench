#include "../bench.h"
#include <stdbool.h>
#include <stdio.h>

int main() {
  char raw_str[] = STRING;

  char *str = (char *)raw_str;
  const char delim = ',';
  bool finish = *str == '\0';

  while (1) {
    char *token = str;
    if (*str == '\0') {
      token = finish ? finish = false, "" : NULL;
    } else {
      while (*str != '\0') {
        if (*str == delim) {
          *str = '\0';
          finish = *(++str) == '\0';
          break;
        }
        str++;
      }
    }

    if (token == NULL) {
      break;
    }

    printf("%s\n", token);
  }
}
