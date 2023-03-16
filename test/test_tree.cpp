#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;
int main()
{
    RedirectToFile("in.in");
    Random rnd;
    int n = rnd.irand(1,100);
    cout << n << endl;
    NoRootTree nrt = n;
    nrt.Generate();
    nrt.Output();
}