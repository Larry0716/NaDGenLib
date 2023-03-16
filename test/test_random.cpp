#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;
int main()
{
    RedirectToFile("in.in");
    Random rnd;
    int a = rnd.irand(1,INT_MAX);
    int b = rnd.irand(1,INT_MAX);
    cout << a << ' ' << b << endl;
}