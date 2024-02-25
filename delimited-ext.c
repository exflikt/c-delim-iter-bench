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

typedef struct {
  char *str;
  const char *delim;
  bool finish;
} DelimitedIter2;

DelimitedIter2 delimited2_new(char *str, const char *delim) {
  return (DelimitedIter2){.str = str, .delim = delim, .finish = *str == '\0'};
}

char *delimited2_next(DelimitedIter2 *iter) {
  if (*iter->str == '\0') {
    return iter->finish ? iter->finish = false, "" : NULL;
  }

  char *token = iter->str;
  while (*iter->str != '\0') {
    for (const char *delim = iter->delim; *delim != '\0'; delim++) {
      if (*iter->str == *delim) {
        *iter->str = '\0';
        iter->finish = *(++iter->str) == '\0';
        goto exit;
      }
    }
    iter->str++;
  }
exit:
  return token;
}

int main() {
  {
    char str[20] = "||aa|bbb|cccccc|dd||";
    // char str[20] = "";
    // char str[10] = "abcd|a||||";
    // char str[3] = "abc";
    printf("original: %s\n", str);
    DelimitedIter it = delimited_new(str, '|');
    char *token;
    while ((token = delimited_next(&it)) != NULL) {
      printf("%s\n", token);
    }
  }

  {
    char str[20] = "||aa|bbb|cccccc|dd||";
    // char str[20] = "";
    // char str[10] = "abcd|a||||";
    // char str[3] = "abc";
    printf("original2: %s\n", str);
    DelimitedIter2 it = delimited2_new(str, "|");
    char *token;
    while ((token = delimited2_next(&it)) != NULL) {
      printf("%s\n", token);
    }
  }
}
