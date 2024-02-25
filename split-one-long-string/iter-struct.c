#include "../bench.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  char *str;
  const char delim;
  bool finish;
} DelimitedIter;

DelimitedIter delimited_new(char *str, const char delim) {
  return (DelimitedIter){.str = str, .delim = delim, .finish = *str == '\0'};
}

char *delimited_next(DelimitedIter *iter) {
  if (*iter->str == '\0') {
    return iter->finish ? iter->finish = false, "" : NULL;
  }

  char *token = iter->str;
  while (*iter->str != '\0') {
    if (*iter->str == iter->delim) {
      *iter->str = '\0';
      iter->finish = *(++iter->str) == '\0';
      break;
    }
    iter->str++;
  }
  return token;
}

int main() {
  char str[] = STRING;
  DelimitedIter delimited = delimited_new(str, ',');
  char *token;
  while ((token = delimited_next(&delimited)) != NULL) {
    printf("%s\n", token);
  }
}
