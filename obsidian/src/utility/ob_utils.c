#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool __ob_util_openfile(FILE **fp, const char *const path,
                        const char *const mode) {
  (*fp) = fopen(path, mode);
  if ((*fp) == NULL) {
    // TODO: replace with error handler messaage
    printf("[OBSIDIAN]: Could not locate file: '%s'", path);
    return false;
  }
  return true;
}

const char *__ob_util_readfile(const char *const path) {
  FILE *fp = NULL;
  size_t size = 0;
  char *buffer = NULL;

  if (__ob_util_openfile(&fp, path, "r") == false)
    return NULL;

  fseek(fp, 0l, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  buffer = calloc(size + 1, sizeof(char));
  if (buffer == NULL) {
    // TODO: replace with error handler message
    printf("[OBSIDIAN]: Could not ");
    return NULL;
  }

  fread(buffer, 1, size, fp);
  printf("%s\n", buffer);
  return buffer;
}