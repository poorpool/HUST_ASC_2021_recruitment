#include "stencil.h"
#include <thread>

const int THREAD_NUMBER = 8;
std::thread threads[THREAD_NUMBER];
/*
template<typename P>
void multi_calc(P * in, P * out, int from, int to, int height, int width) {
  for (int ii = from; ii <= to; ii += BLOCK_SIZE) {
    for (int jj = 1; jj < width-1; jj += BLOCK_SIZE) {
      int iiMax = min(ii+BLOCK_SIZE-1, to);
      int jjMax = min(jj+BLOCK_SIZE-1, width-2);
      for (int i = ii; i <= iiMax; ++i) {
        for (int j = jj; j <= jjMax; ++j) {
          int im1jm1 =(i-1)*width + j-1;
          int im1j   =(i-1)*width + j;
          int im1jp1 =(i-1)*width + j+1;
          int ijm1   =(i  )*width + j-1;
          int ij     =(i  )*width + j;
          int ijp1   =(i  )*width + j+1;
          int ip1jm1 =(i+1)*width + j-1;
          int ip1j   =(i+1)*width + j;
          int ip1jp1 =(i+1)*width + j+1;
          P val = 
            -in[im1jm1] -   in[im1j] - in[im1jp1] 
            -in[ijm1]   +   8*in[ij] - in[ijp1] 
            -in[ip1jm1] -   in[ip1j] - in[ip1jp1];

          or

          P val = 0;
          P * point = in + (i-1)*width + j-1;
          val = val - *point; ++point; val = val - *point; ++point; val = val - *point; ++point;
          P * point1 = in + i*width + j-1;
          val = val - *point1; ++point1; val = val + 8 * (*point1); ++point1; val = val - *point1; ++point1;
          P * point2 = in + (i+1)*width + j-1;
          val = val - *point2; ++point2; val = val - *point2; ++point2; val = val - *point2; ++point2;

          val = (val < 0   ? 0   : val);
          val = (val > 255 ? 255 : val);
          out[i*width + j] = val;
        }
      }
    }
  }
}
*/

template<typename P>
void multi_calc(P * in, P * out, int from, int to, int height, int width) {
  for (int i = from; i <= to; i++) {
    for (int j = 1; j < width-1; j++) {
      int im1jm1 =(i-1)*width + j-1;
      int im1j   =(i-1)*width + j;
      int im1jp1 =(i-1)*width + j+1;
      int ijm1   =(i  )*width + j-1;
      int ij     =(i  )*width + j;
      int ijp1   =(i  )*width + j+1;
      int ip1jm1 =(i+1)*width + j-1;
      int ip1j   =(i+1)*width + j;
      int ip1jp1 =(i+1)*width + j+1;
      P val = 
        -in[im1jm1] -   in[im1j] - in[im1jp1] 
        -in[ijm1]   +   8*in[ij] - in[ijp1] 
        -in[ip1jm1] -   in[ip1j] - in[ip1jp1];
      val = (val < 0   ? 0   : val);
      val = (val > 255 ? 255 : val);
      out[i*width + j] = val;
    }
  }
}

template<typename P>
void ApplyStencil(ImageClass<P> & img_in, ImageClass<P> & img_out) {
  const int width  = img_in.width;
  const int height = img_in.height;

  P * in  = img_in.pixel;
  P * out = img_out.pixel;

  int perthread = (height - 2) / THREAD_NUMBER;
  for (int i=0; i < THREAD_NUMBER - 1; i++) {
    threads[i] = std::thread(multi_calc<P>, in, out, i*perthread+1, (i+1)*perthread, height, width);
  }
  threads[THREAD_NUMBER - 1] = std::thread(multi_calc<P>, in, out, (THREAD_NUMBER - 1)*perthread+1, height-2, height, width);
  for (int i = 0; i < THREAD_NUMBER; i++) {
    threads[i].join();
  }
}
/*
template<typename P>
void ApplyStencil(ImageClass<P> & img_in, ImageClass<P> & img_out) {
  
  const int width  = img_in.width;
  const int height = img_in.height;

  P * in  = img_in.pixel;
  P * out = img_out.pixel;


  for (int i = 1; i < height-1; i++) {
    for (int j = 1; j < width-1; j++) {
      int im1jm1 =(i-1)*width + j-1;
      int im1j   =(i-1)*width + j;
      int im1jp1 =(i-1)*width + j+1;
      int ijm1   =(i  )*width + j-1;
      int ij     =(i  )*width + j;
      int ijp1   =(i  )*width + j+1;
      int ip1jm1 =(i+1)*width + j-1;
      int ip1j   =(i+1)*width + j;
      int ip1jp1 =(i+1)*width + j+1;
      P val = 
        -in[im1jm1] -   in[im1j] - in[im1jp1] 
        -in[ijm1]   +   8*in[ij] - in[ijp1] 
        -in[ip1jm1] -   in[ip1j] - in[ip1jp1];

      val = (val < 0   ? 0   : val);
      val = (val > 255 ? 255 : val);

      out[i*width + j] = val;

    }

  }

}*/

template void ApplyStencil<float>(ImageClass<float> & img_in, ImageClass<float> & img_out);
