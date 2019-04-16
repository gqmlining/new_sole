#include <iostream>

using namespace std;

int array[10000];

int main()
{
   for (int i=0;i<10000;i++)
   {
      array[i] = i;
   }
   for (int i=0;i<10000;i++)
   {
      array[i]+=i;
   }
   return 0;
}
