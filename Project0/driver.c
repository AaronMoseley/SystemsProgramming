#include <stdio.h>

int streqnocase(const char *a, const char *b);

int main() {
  char *x = "%%##\t\tasDD";
  char *y = "%%##\t\tASdd";

  printf("strings x and y are ");
  if (!streqnocase(x,y))
    printf("NOT ");
  printf("equal.\n");
  return 0;
}
