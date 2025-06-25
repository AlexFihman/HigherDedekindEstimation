#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "mbf16c.h"
#include "rnd.h"
#include "time_ms.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    cerr << "Usage: " << argv[0] << " <iterations> <MinWeight> <MaxWeight>\n";
    return 1;
  }

  double t1, t2;
  sfmt_init();
  int64_t iter = std::strtoull(argv[1], nullptr, 10);
  mbf16 *m = new mbf16();
  int MinWeight = atoi(argv[2]);
  int MaxWeight = atoi(argv[3]);
  cout << "MinWeight: " << MinWeight << endl;
  cout << "MaxWeight: " << MaxWeight << endl;

  ofstream outfile;
  string s = "mbf.";
  s += argv[2];
  s += ".txt";
  outfile.open(s.c_str(), ios::trunc);
  outfile << scientific;
  outfile.precision(10);

  double pvalL[d1 + 1];
  double pvalR[d1 + 1];
  double pvalS[d1 + 1];
  for (int i = 0; i < (d1 + 1); i++) {
    pvalL[i] = 0;
    pvalR[i] = 0;
    pvalS[i] = 0;
  }

  t1 = TimeMillis();
  for (int cycle = 0; cycle < 30; cycle++) {
    cout << "cycle: " << cycle << endl;
    m->Prepare(MinWeight, MaxWeight);

    for (int i = 0; i < 10000; i++) m->Browse(MinWeight, MaxWeight);

    for (uint64_t i = 0; i < iter; i++) {
      m->Browse(MinWeight, MaxWeight);
      pvalL[m->Weight] += m->MinTP->Count * m->p;
      pvalR[m->Weight] += m->MinFP->Count * m->p;
      pvalS[m->Weight] += m->p;
    }

    for (int i = MinWeight; i <= MaxWeight; i++) {
      outfile << cycle << "\t" << i << "\t" << pvalL[i] << "\t" << pvalR[i] << "\t" << pvalS[i] << "\t" << "\n";
    }

    for (int i = 0; i < (d1 + 1); i++) {
      pvalR[i] = 0;
      pvalL[i] = 0;
      pvalS[i] = 0;
    }
    outfile.flush();
  }
  t2 = TimeMillis();

  outfile << "runtime: \t" << (t2 - t1);
  outfile.close();
  return 0;
}
