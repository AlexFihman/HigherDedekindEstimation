#pragma once
#include "fastlist.h"

#define d1 8192  // {2^d3}
#define d3 13    //

#define MASK_01010101 (((unsigned int)(-1)) / 3)
#define MASK_00110011 (((unsigned int)(-1)) / 5)
#define MASK_00001111 (((unsigned int)(-1)) / 17)
#define MASK_00001111_2 (((unsigned int)(-1)) / 257)

#define bitcountMacro(src, n)                                 \
  {                                                           \
    n = src;                                                  \
    n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101);     \
    n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011);     \
    n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111);     \
    n = (n & MASK_00001111_2) + ((n >> 8) & MASK_00001111_2); \
    n = (n & 65535) + ((n >> 16) & 65535);                    \
  }

#define ties(i, j) (i ^ (0x1u << j))

class mbf16 {
 public:
  int LevelOfItem[d1];
  int LevelDepth[d3 + 1];
  int Level[d3 + 1];
  int LinkCnt[d1];
  int levellist[d3 + 1][d1];
  bool MainArray[d1];
  int MainArrayU[d1];
  int MainArrayL[d1];

  int Weight;
  int LowerCnt;
  int UpperCnt;
  int LevelUpDepth;
  double p;

  TFastList* MinTP; //minimal true point
  TFastList* MinFP;

  mbf16();
  ~mbf16();

  void Browse(int MinWeight, int MaxWeight);
  void ChangeItem(int Item);
  void MakeListOfGreys();
  void RefreshMinPoints(int Item, bool wh);
  void Prepare(int MinVal, int MaxVal);
  int LevelTop();
  int LevelBottom();
};