#include "rnd.h"
extern "C" {
#include "SFMT.h"
}
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

sfmt_t* mt;

uint32_t* randbuf() {
  uint32_t* arr = (uint32_t*)malloc(624 * 4);
  std::ifstream urandom("/dev/urandom", ios::in | ios::binary);
  urandom.read((char*)arr, 624 * 4);
  urandom.close();
  return arr;
}

void rand_init(sfmt_t* mt) {
  uint32_t* arr = randbuf();
  sfmt_init_by_array(mt, arr, 623);
  delete arr;
}

void sfmt_init() {
  mt = new sfmt_t();
  rand_init(mt);
}
