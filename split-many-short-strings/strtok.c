#include "../bench.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char str[] = STRING1;
  const char delim[] = ",";

  for (int i = 0; i < RUNS; i++) {
    char *cloned_str = (char *)malloc(sizeof str);
    strcpy(cloned_str, str);
    char *token = strtok(cloned_str, delim);
    while (token != NULL) {
      printf("%s\n", token);
      token = strtok(NULL, delim);
    }
    free(cloned_str);
  }
}
