
#define real_t float

void test(real_t *a, real_t *b, real_t *c, real_t *d, real_t *e) {
  a[0] += d[0] * e[0];
  if (a[0] < (real_t)0.)
      b[0] += d[0] * e[0];
  c[0] += a[0] * d[0];

  a[1] += d[1] * e[1];
  if (a[1] < (real_t)0.)
      b[1] += d[1] * e[1];
  c[1] += a[1] * d[1];

  a[2] += d[2] * e[2];
  if (a[2] < (real_t)0.)
      b[2] += d[2] * e[2];
  c[2] += a[2] * d[2];

  a[3] += d[3] * e[3];
  if (a[3] < (real_t)0.)
      b[3] += d[3] * e[3];
  c[3] += a[3] * d[3];
}
