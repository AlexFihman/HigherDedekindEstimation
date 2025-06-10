#include <stdlib.h>
//#include "mtrand.h"
#include "SFMT.h"

#ifndef fastlist
#define fastlist
#define ArrSize 16384
class TFastList{
   public:
   TFastList();
   int           Count;
   int  Arr[ArrSize];
   int  Items[ArrSize];

   void  Add(int Item);
   void  Delete(int Item);
   void  Clear();
   int  RandItem();
   void  Assign(TFastList* lst);
};
#endif
