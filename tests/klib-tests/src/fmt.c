#include <am.h>
#include <klib.h>
#include <limits.h>

/* Expected strings below were produced by glibc sprintf on the host.
 * vsprintf implements %d/%u/%x/%s/%c and field width, including zero pad.
 * Precision and length modifiers are not implemented. */

#define check_fmt(expected, fmt, ...) \
  do { \
    char _buf[64]; \
    int _n = sprintf(_buf, fmt, __VA_ARGS__); \
    assert(strcmp(_buf, expected) == 0); \
    assert(_n == (int)strlen(expected)); \
  } while (0)

static const int data[] = {
  0,
  INT_MAX / 17,
  INT_MAX,
  INT_MIN,
  INT_MIN + 1,
  UINT_MAX / 17,
  (int)UINT_MAX,
};

/* plain, width 12, zero-padded width 12. Indexed like data[]. */
static const char *dec_plain[] = {
  "0",
  "126322567",
  "2147483647",
  "-2147483648",
  "-2147483647",
  "252645135",
  "-1",
};
static const char *dec_width[] = {
  "           0",
  "   126322567",
  "  2147483647",
  " -2147483648",
  " -2147483647",
  "   252645135",
  "          -1",
};
static const char *dec_zero[] = {
  "000000000000",
  "000126322567",
  "002147483647",
  "-02147483648",
  "-02147483647",
  "000252645135",
  "-00000000001",
};

static const unsigned udata[] = {
  0,
  UINT_MAX / 17,
  UINT_MAX,
  (unsigned)INT_MAX,
};

static const char *udec_plain[] = {
  "0", "252645135", "4294967295", "2147483647",
};
static const char *udec_width[] = {
  "           0",
  "   252645135",
  "  4294967295",
  "  2147483647",
};
static const char *udec_zero[] = {
  "000000000000",
  "000252645135",
  "004294967295",
  "002147483647",
};
static const char *hex_plain[] = {
  "0", "f0f0f0f", "ffffffff", "7fffffff",
};
static const char *hex_width[] = {
  "       0",
  " f0f0f0f",
  "ffffffff",
  "7fffffff",
};
static const char *hex_zero[] = {
  "00000000",
  "0f0f0f0f",
  "ffffffff",
  "7fffffff",
};

void test_sprintf(void) {
  int i;

  for (i = 0; i < (int)(sizeof data / sizeof data[0]); i++) {
    check_fmt(dec_plain[i], "%d", data[i]);
    check_fmt(dec_width[i], "%12d", data[i]);
    check_fmt(dec_zero[i], "%012d", data[i]);
  }

  for (i = 0; i < (int)(sizeof udata / sizeof udata[0]); i++) {
    check_fmt(udec_plain[i], "%u", udata[i]);
    check_fmt(udec_width[i], "%12u", udata[i]);
    check_fmt(udec_zero[i], "%012u", udata[i]);
    check_fmt(hex_plain[i], "%x", udata[i]);
    check_fmt(hex_width[i], "%8x", udata[i]);
    check_fmt(hex_zero[i], "%08x", udata[i]);
  }

  check_fmt("", "%s", "");
  check_fmt("hello", "%s", "hello");
  check_fmt("Ab", "%c%c", 'A', 'b');
  check_fmt("num=-42 hex=2a str=hi ch=Z",
            "num=%d hex=%x str=%s ch=%c", -42, 0x2au, "hi", 'Z');
}
