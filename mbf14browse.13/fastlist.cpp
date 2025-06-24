#include "fastlist.h"

#include "rnd.h"
#include "string.h"

TFastList::TFastList() {
  Count = 0;
  for (int i = 0; i < ArrSize; i++) {
    Arr[i] = -1;
    Items[i] = -1;
  }
};

void TFastList::Add(int Item) {
  int pos = Arr[Item];
  if (pos == -1) {
    Items[Count] = Item;
    Arr[Item] = Count;
    Count++;
  }
}

void TFastList::Delete(int Item) {
  int pos = Arr[Item];
  if (pos != -1) {
    Arr[Item] = -1;
    Count--;
    if (pos != Count) {
      Item = Items[Count];
      Items[pos] = Item;
      Arr[Item] = pos;
    }
  }
}

void TFastList::Clear() {
  Count = 0;
  for (int i = 0; i < ArrSize; i++) Arr[i] = -1;
}

int TFastList::RandItem() { return Items[sfmt_genrand_uint32(mt) % (Count)]; }
