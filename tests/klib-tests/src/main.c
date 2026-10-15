#include <am.h>
#include <klib.h>
#include <klib-macros.h>

void test_memset(void);
void test_memcpy(void);
void test_memmove(void);
void test_strcpy(void);
void test_strncpy(void);
void test_strcat(void);
void test_strlen(void);
void test_memcmp(void);
void test_strcmp(void);
void test_strncmp(void);
void test_sprintf(void);

int main(const char *args) {
  (void)args;
  test_memset();
  test_memcpy();
  test_memmove();
  test_strcpy();
  test_strncpy();
  test_strcat();
  test_strlen();
  test_memcmp();
  test_strcmp();
  test_strncmp();
  test_sprintf();
  putstr("klib tests passed\n");
  return 0;
}
