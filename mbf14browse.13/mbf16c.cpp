#include "mbf16c.h"
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
    truthTable[i] = false;
    MainArrayU[i] = d3 - LevelOfItem[i];
    MainArrayL[i] = 0;
    LinkCnt[i] = 0;
  }

  MinTP = new TFastList;
  MinFP = new TFastList;
  MakeListOfGreys();

  for (i = 0; i < d3 + 1; i++) {
    Level[i] = 0;
  }
}

void mbf16::FlipValueAt(int Item) {
  if (truthTable[Item] = !truthTable[Item]) {
    Level[LevelOfItem[Item]]++;
    Weight++;
    RefreshMinPoints(Item, true);
  } else {
    Level[LevelOfItem[Item]]--;
    Weight--;
    RefreshMinPoints(Item, false);
  }
}

void mbf16::Browse(int MinWeight, int MaxWeight) {
  int GreyCnt;
  int ToChange;

  if (Weight == MinWeight) {
    ToChange = MinFP->RandItem();
    GreyCnt = MinFP->Count;
  } else {
    if (Weight == MaxWeight) {
      ToChange = MinTP->RandItem();
      GreyCnt = MinTP->Count;
    } else {
      GreyCnt = MinTP->Count + MinFP->Count;
      int Item = sfmt_genrand_uint32(mt) % GreyCnt;
      if (Item < MinTP->Count) {
        ToChange = MinTP->Items[Item];
      } else {
        ToChange = MinFP->Items[Item - MinTP->Count];
      };
    }
  };

  FlipValueAt(ToChange);

  if (Weight == MinWeight) {
    GreyCnt = MinFP->Count;
  } else if (Weight == MaxWeight) {
    GreyCnt = MinTP->Count;
  } else {
    GreyCnt = MinFP->Count + MinTP->Count;
  }

  p = 1.0 / GreyCnt;
}

void mbf16::Prepare(int MinWeight, int MaxWeight) {
  while (Weight < MinWeight) {
    int it = MinFP->RandItem();
    FlipValueAt(it);
  };

  while (Weight > MaxWeight) {
    int it = MinTP->RandItem();
    FlipValueAt(it);
  };
}

void mbf16::MakeListOfGreys() {
  MinTP->Clear();
  MinFP->Clear();
  MinFP->Add(d1 - 1);
  Weight = 0;
}

void mbf16::RefreshMinPoints(int Item, bool wh) {
  if (wh) {
    MinFP->Delete(Item);
    MinTP->Add(Item);
    for (int i = 0; i < d3; i++) {
      int f = ties(Item, i);
      if (f > Item) {
        MainArrayL[f]++;
        MinTP->Delete(f);
      } else {
        MainArrayU[f]--;
        if (MainArrayU[f] == 0) {
          MinFP->Add(f);
        }
      };
    }
  } else {
    MinFP->Add(Item);
    MinTP->Delete(Item);
    for (int i = 0; i < d3; i++) {
      int f = ties(Item, i);
      if (f > Item) {
        MainArrayL[f]--;
        if (MainArrayL[f] == 0) {
          MinTP->Add(f);
        }
      } else {
        MainArrayU[f]++;
        MinFP->Delete(f);
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

int mbf16::LevelTop() {
  int x = d3 + 1;
  for (int i = d3; i >= 0; i--) {
    if (Level[i] == LevelDepth[i]) {
      x = i;
    }
  }
  return x;
}