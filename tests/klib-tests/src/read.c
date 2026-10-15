#include <am.h>
#include <klib.h>

#define N 32

static int sgn(int x) {
  return (x > 0) - (x < 0);
}

static void fill(uint8_t *a) {
  int i;
  for (i = 0; i < N; i++) {
    a[i] = i + 1;
  }
}

static int unchanged(const uint8_t *a) {
  int i;
  for (i = 0; i < N; i++) {
    if (a[i] != i + 1) {
      return 0;
    }
  }
  return 1;
}

/* s[0..len) = 1, 2, ..., len; s[len] = 0. No zero appears before len. */
static void make_str(char *s, int len) {
  int i;
  for (i = 0; i < len; i++) {
    s[i] = i + 1;
  }
  s[len] = '\0';
}

void test_strlen(void) {
  int l, len;
  uint8_t buf[N];

  for (l = 0; l < N; l++) {
    for (len = 0; l + len < N; len++) {
      int i;
      fill(buf);
      buf[l + len] = 0;
      assert(strlen((char *)buf + l) == (size_t)len);
      for (i = 0; i < l + len; i++) {
        assert(buf[i] == i + 1);
      }
      assert(buf[l + len] == 0);
      for (i = l + len + 1; i < N; i++) {
        assert(buf[i] == i + 1);
      }
    }
  }
}

void test_memcmp(void) {
  int o1, o2, n, k;
  uint8_t lhs[N], rhs[N];

  fill(lhs);
  fill(rhs);
  for (o1 = 0; o1 < N; o1++) {
    for (o2 = 0; o2 < N; o2++) {
      for (n = 0; o1 + n <= N && o2 + n <= N; n++) {
        int expect = 0;
        if (n > 0 && o1 != o2) {
          expect = (o1 + 1) - (o2 + 1);
        }
        assert(sgn(memcmp(lhs + o1, rhs + o2, n)) == sgn(expect));
        assert(unchanged(lhs) && unchanged(rhs));
      }
    }
  }

  /* A later byte differs, including 0 and a value with the high bit set.
     memcmp must not treat 0 as a terminator. */
  for (o1 = 0; o1 < N; o1++) {
    for (n = 1; o1 + n <= N; n++) {
      for (k = 0; k < n; k++) {
        uint8_t saved;
        int mutants[2];
        int m;
        mutants[0] = 0;
        mutants[1] = 0xFF;
        saved = rhs[o1 + k];
        for (m = 0; m < 2; m++) {
          int expect;
          rhs[o1 + k] = mutants[m];
          expect = (o1 + k + 1) - mutants[m];
          assert(sgn(memcmp(lhs + o1, rhs + o1, n)) == sgn(expect));
          assert(sgn(memcmp(rhs + o1, lhs + o1, n)) == sgn(-expect));
          assert(unchanged(lhs));
        }
        rhs[o1 + k] = saved;
      }
    }
  }
  assert(unchanged(rhs));
}

void test_strcmp(void) {
  int len1, len2, d;
  char s1[N + 1], s2[N + 1];

  for (len1 = 0; len1 <= N; len1++) {
    for (len2 = 0; len2 <= N; len2++) {
      int expect = 0;
      make_str(s1, len1);
      make_str(s2, len2);
      if (len1 < len2) {
        expect = -1;
      } else if (len1 > len2) {
        expect = 1;
      }
      assert(sgn(strcmp(s1, s2)) == expect);
      assert(s1[len1] == '\0' && s2[len2] == '\0');
    }
  }

  for (len1 = 1; len1 <= N; len1++) {
    for (d = 0; d < len1; d++) {
      int expect;
      make_str(s1, len1);
      make_str(s2, len1);
      s2[d] = (char)0xFF;
      expect = (d + 1) - 0xFF;
      assert(sgn(strcmp(s1, s2)) == sgn(expect));
      assert(sgn(strcmp(s2, s1)) == sgn(-expect));
      assert((unsigned char)s2[d] == 0xFF);
      assert(s1[len1] == '\0' && s2[len1] == '\0');
    }
  }
}

void test_strncmp(void) {
  int len1, len2, n, d;
  char s1[N + 1], s2[N + 1];

  for (len1 = 0; len1 <= N; len1++) {
    for (len2 = 0; len2 <= N; len2++) {
      int common = len1 < len2 ? len1 : len2;
      make_str(s1, len1);
      make_str(s2, len2);
      for (n = 0; n <= N; n++) {
        int expect = 0;
        if (n > common) {
          if (len1 < len2) {
            expect = -1;
          } else if (len1 > len2) {
            expect = 1;
          }
        }
        assert(sgn(strncmp(s1, s2, n)) == expect);
      }
      assert(s1[len1] == '\0' && s2[len2] == '\0');
    }
  }

  /* Difference at d. n <= d stops before that byte and returns 0. */
  for (len1 = 1; len1 <= N; len1++) {
    for (d = 0; d < len1; d++) {
      make_str(s1, len1);
      make_str(s2, len1);
      s2[d] = (char)0xFF;
      for (n = 0; n <= len1; n++) {
        int expect = 0;
        if (n > d) {
          expect = (d + 1) - 0xFF;
        }
        assert(sgn(strncmp(s1, s2, n)) == sgn(expect));
        assert(sgn(strncmp(s2, s1, n)) == sgn(-expect));
      }
      assert((unsigned char)s2[d] == 0xFF);
      assert(s1[len1] == '\0' && s2[len1] == '\0');
    }
  }
}
