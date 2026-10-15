#include <am.h>
#include <klib.h>

#define N 32

uint8_t data[N];
/* Distinct from data[i] == i + 1, so a missed copy cannot pass. */
uint8_t src[N];

void reset(void) {
  int i;
  for (i = 0; i < N; i++) {
    data[i] = i + 1;
  }
}

static void reset_src(void) {
  int i;
  for (i = 0; i < N; i++) {
    src[i] = 0x80 + i;
  }
}

/* [l, r) holds val, val + 1, val + 2, ... */
void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i++) {
    assert(data[i] == val + i - l);
  }
}

/* [l, r) holds val in every byte. */
void check_eq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i++) {
    assert(data[i] == val);
  }
}

void test_memset(void) {
  int l, r;
  for (l = 0; l < N; l++) {
    for (r = l; r <= N; r++) {
      reset();
      uint8_t val = (l + r) / 2;
      assert(memset(data + l, val, r - l) == data + l);
      check_seq(0, l, 1);
      check_eq(l, r, val);
      check_seq(r, N, r + 1);
    }
  }

  /* The fill byte is converted to unsigned char. */
  reset();
  assert(memset(data, 256, N) == data);
  check_eq(0, N, 0);
  reset();
  assert(memset(data, -1, N) == data);
  check_eq(0, N, 255);
}

void test_memcpy(void) {
  int d, s, n;
  reset_src();
  for (d = 0; d < N; d++) {
    for (s = 0; s < N; s++) {
      for (n = 0; d + n <= N && s + n <= N; n++) {
        int i;
        reset();
        assert(memcpy(data + d, src + s, n) == data + d);
        check_seq(0, d, 1);
        check_seq(d, d + n, 0x80 + s);
        check_seq(d + n, N, d + n + 1);
        for (i = 0; i < N; i++) {
          assert(src[i] == 0x80 + i);
        }
      }
    }
  }
}

void test_memmove(void) {
  int d, s, n;
  for (d = 0; d < N; d++) {
    for (s = 0; s < N; s++) {
      for (n = 0; d + n <= N && s + n <= N; n++) {
        reset();
        assert(memmove(data + d, data + s, n) == data + d);
        check_seq(0, d, 1);
        /* Original data[s + k] was (s + k) + 1. */
        check_seq(d, d + n, s + 1);
        check_seq(d + n, N, d + n + 1);
      }
    }
  }
}

void test_strcpy(void) {
  int l, len;
  for (l = 0; l < N; l++) {
    for (len = 0; l + len < N; len++) {
      int i;
      char from[N + 1];
      char *dst;
      reset();
      for (i = 0; i < len; i++) {
        from[i] = i + 1;
      }
      from[len] = '\0';
      dst = (char *)data + l;
      assert(strcpy(dst, from) == dst);
      check_seq(0, l, 1);
      check_seq(l, l + len, 1);
      assert(data[l + len] == 0);
      check_seq(l + len + 1, N, l + len + 2);
      for (i = 0; i < len; i++) {
        assert((uint8_t)from[i] == i + 1);
      }
      assert(from[len] == '\0');
    }
  }
}

void test_strncpy(void) {
  int l, n, src_len;
  for (l = 0; l < N; l++) {
    for (n = 0; l + n <= N; n++) {
      for (src_len = 0; src_len <= N; src_len++) {
        int i;
        int copied;
        char from[N + 1];
        char *dst;
        reset();
        for (i = 0; i < src_len; i++) {
          from[i] = i + 1;
        }
        from[src_len] = '\0';
        dst = (char *)data + l;
        assert(strncpy(dst, from, n) == dst);
        check_seq(0, l, 1);
        copied = src_len < n ? src_len : n;
        check_seq(l, l + copied, 1);
        if (src_len < n) {
          check_eq(l + copied, l + n, 0);
        }
        check_seq(l + n, N, l + n + 1);
      }
    }
  }
}

void test_strcat(void) {
  int l, dst_len, src_len;
  for (l = 0; l < N; l++) {
    for (dst_len = 0; l + dst_len < N; dst_len++) {
      for (src_len = 0; l + dst_len + src_len < N; src_len++) {
        int i;
        char from[N + 1];
        char *dst;
        reset();
        data[l + dst_len] = 0;
        for (i = 0; i < src_len; i++) {
          from[i] = i + 1;
        }
        from[src_len] = '\0';
        dst = (char *)data + l;
        assert(strcat(dst, from) == dst);
        check_seq(0, l, 1);
        check_seq(l, l + dst_len, l + 1);
        check_seq(l + dst_len, l + dst_len + src_len, 1);
        assert(data[l + dst_len + src_len] == 0);
        check_seq(l + dst_len + src_len + 1, N, l + dst_len + src_len + 2);
      }
    }
  }
}
