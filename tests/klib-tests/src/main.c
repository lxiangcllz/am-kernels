#include <am.h>
#include <klib.h>
#include <klib-macros.h>

void test_memset(void);
void test_memcpy(void);
void test_memmove(void);
void test_strcpy(void);
void test_strncpy(void);
void test_strcat(void);

int main(const char *args) {
  (void)args;
  test_memset();
  test_memcpy();
  test_memmove();
  test_strcpy();
  test_strncpy();
  test_strcat();
  putstr("klib write tests passed\n");
  return 0;
}
