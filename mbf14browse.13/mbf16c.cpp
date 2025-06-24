#ifndef _mbf16cH_
#include "mbf16c.h"
#define _mbf16cH_
#endif
#include "rnd.h"

mbf16::mbf16() {
  int i, j;

  for (i = 0; i < d1; i++) {
    bitcountMacro(i, j);
    LevelOfItem[i] = j;
  }

  for (i = 0; i < d3 + 1; i++) {
    LevelDepth[i] = 0;
  }

  for (i = 0; i < d1; i++) {
    int lo = LevelOfItem[i];
    int ld = LevelDepth[lo];
    levellist[lo][ld] = i;
    LevelDepth[lo]++;
  }

  for (i = 0; i < d1; i++) {
    MainArray[i] = false;
    MainArrayU[i] = d3 - LevelOfItem[i];
    MainArrayL[i] = 0;
    LinkCnt[i] = 0;
  }

  BlackList = new TFastList;
  WhiteList = new TFastList;
  MakeListOfGreys();

  for (i = 0; i < d3 + 1; i++) {
    Level[i] = 0;
  }
}

void mbf16::ChangeItem(int Item) {
  if (MainArray[Item] = !MainArray[Item]) {
    Level[LevelOfItem[Item]]++;
    Weight++;
    FastMakeListOfGreys(Item, true);
  } else {
    Level[LevelOfItem[Item]]--;
    Weight--;
    FastMakeListOfGreys(Item, false);
  }
}

void mbf16::Browse(int MinVal, int MaxVal) {
  int GreyCnt;
  int ToChange;

  if (Weight == MinVal) {
    ToChange = BlackList->RandItem();
    GreyCnt = BlackList->Count;
  } else {
    if (Weight == MaxVal) {
      ToChange = WhiteList->RandItem();
      GreyCnt = WhiteList->Count;
    } else {
      GreyCnt = WhiteList->Count + BlackList->Count;
      int Item = sfmt_genrand_uint32(mt) % GreyCnt;
      if (Item < WhiteList->Count) {
        ToChange = WhiteList->Items[Item];
      } else {
        ToChange = BlackList->Items[Item - WhiteList->Count];
      };
    }
  };

  ChangeItem(ToChange);

  if (Weight == MinVal) {
    GreyCnt = BlackList->Count;
  } else if (Weight == MaxVal) {
    GreyCnt = WhiteList->Count;
  } else {
    GreyCnt = BlackList->Count + WhiteList->Count;
  }

  p = 1.0 / GreyCnt;
}

void mbf16::Prepare(int MinVal, int MaxVal) {
  while (Weight < MinVal) {
    int it = BlackList->RandItem();
    ChangeItem(it);
  };

  while (Weight > MaxVal) {
    int it = WhiteList->RandItem();
    ChangeItem(it);
  };
}

void mbf16::MakeListOfGreys() {
  WhiteList->Clear();
  BlackList->Clear();
  BlackList->Add(d1 - 1);
  Weight = 0;
}

void mbf16::FastMakeListOfGreys(int Item, bool wh) {
  if (wh) {
    BlackList->Delete(Item);
    WhiteList->Add(Item);
    for (int i = 0; i < d3; i++) {
      int f = ties(Item, i);
      if (f > Item) {
        MainArrayL[f]++;
        WhiteList->Delete(f);
      } else {
        MainArrayU[f]--;
        if (MainArrayU[f] == 0) {
          BlackList->Add(f);
        }
      };
    }
  } else {
    BlackList->Add(Item);
    WhiteList->Delete(Item);
    for (int i = 0; i < d3; i++) {
      int f = ties(Item, i);
      if (f > Item) {
        MainArrayL[f]--;
        if (MainArrayL[f] == 0) {
          WhiteList->Add(f);
        }
      } else {
        MainArrayU[f]++;
        BlackList->Delete(f);
      };
    }
  }
}

int mbf16::LevelBottom() {
  int x = -1;
  for (int i = 0; i <= d3; i++) {
    if (Level[i] == 0) {
      x = i;
    }
  }
  return x;
}

/** Returns the lowest filled level   */
int mbf16::LevelTop() {
  int x = d3 + 1;
  for (int i = d3; i >= 0; i--) {
    if (Level[i] == LevelDepth[i]) {
      x = i;
    }
  }
  return x;
}