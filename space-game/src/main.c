#include <stdio.h>

#include <obsidian.h>

int main(void) {
  struct obsidian_context *ctx = NULL;
  ctx = OBcreateContext();
  OBdestroyContext(ctx);

  return 0;
}