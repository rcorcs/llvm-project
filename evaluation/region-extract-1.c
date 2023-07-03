

float example(float *A, float *B, float factor, float delta) {
  float tmp0 = 0;
  float tmp1 = 0;
  float tmp2 = 0;
  float tmp3 = 0;
  if (B[0] > (float)0.) {
    tmp0 = B[0]+delta;
    A[0] = tmp0*factor ;
  }
  if (B[1] > (float)0.) {
    tmp1 = B[1]+delta;
    A[1] = tmp1*factor ;
  }
  if (B[2] > (float)0.) {
    tmp2 = B[2]+delta;
    A[2] = tmp2*factor ;
  }
  if (B[3] > (float)0.) {
    tmp3 = B[3]+delta;
    A[3] = tmp3*factor ;
  }
  return tmp0+tmp1+tmp2+tmp3;
}
