#ifdef _WIN32
#include <windows.h>
double TimeMillis() {
  LARGE_INTEGER cc, fq;

  QueryPerformanceCounter(&cc);
  QueryPerformanceFrequency(&fq);
  return 1.0 * cc.QuadPart / (1.0 * fq.QuadPart);
}
#else
#include <sys/time.h>
static struct timeval _tstart, _tend;
static struct timezone tz;

double TimeMillis() {
  gettimeofday(&_tstart, &tz);
  return (double)_tstart.tv_sec + (double)_tstart.tv_usec / (1000 * 1000);
};
#endif
