#include <iostream>

using namespace std;
int main()
{
  long long a = -24;
  unsigned long long b = a;
  unsigned long long d = 9223372036854775152;
  unsigned long long c = b ^ d;
  cout << a << " "  << b << endl;
  cout << c << endl;
}
