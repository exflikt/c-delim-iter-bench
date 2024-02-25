#include "../bench.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char raw_str[] = STRING1;

  for (int i = 0; i < RUNS; i++) {
    char *cloned_str = (char *)malloc(sizeof raw_str);
    strcpy(cloned_str, raw_str);
    char *str = cloned_str;

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

    free(cloned_str);
  }
}
