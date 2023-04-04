
#define real_t float

void test(real_t *a, real_t *b, real_t *c) {
  if (b[0] > (real_t)0.) {
    a[0] += b[0] * c[0];
  }
  if (b[1] > (real_t)0.) {
    a[1] += b[1] * c[1];
  }
  if (b[2] > (real_t)0.) {
    a[2] += b[2] * c[2];
  }
  if (b[3] > (real_t)0.) {
    a[3] += b[3] * c[3];
  }
}
