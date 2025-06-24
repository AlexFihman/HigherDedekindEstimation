#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "mbf16c.h"
#include "rnd.h"
#include "time_ms.h"

using namespace std;

int main(int argc, char *argv[]) {
  double t1, t2;
  sfmt_init();
  int64_t iter = std::strtoull(argv[1], nullptr, 10);
  mbf16 *m = new mbf16();
  int MinVal = atoi(argv[2]);
  int MaxVal = atoi(argv[3]);
  cout << "minval: " << MinVal << endl;
  cout << "MaxVal: " << MaxVal << endl;

  ofstream outfile;
  string s = "mbf.";
  s += argv[2];
  s += ".txt";
  outfile.open(s.c_str(), ios::trunc);
  outfile << scientific;
  outfile.precision(10);

  double pval[d1 + 1];
  for (int i = 0; i < (d1 + 1); i++) {
    pval[i] = 0;
  }

  t1 = TimeMillis();
  for (int cycle = 0; cycle < 30; cycle++) {
    cout << "cycle: " << cycle << endl;
    m->Prepare(MinVal, MaxVal);

    for (int i = 0; i < 10000; i++) m->Browse(MinVal, MaxVal);

    for (uint64_t i = 0; i < iter; i++) {
      m->Browse(MinVal, MaxVal);
      pval[m->Weight] += m->p;
    }

    for (int i = MinVal; i <= MaxVal; i++) {
      outfile << cycle << "\t" << i << "\t" << pval[i] << "\n";
    }

    for (int i = 0; i < (d1 + 1); i++) {
      pval[i] = 0;
    }
    outfile.flush();
  }
  t2 = TimeMillis();

  outfile << "runtime: \t" << (t2 - t1);
  outfile.close();
  return 0;
}
