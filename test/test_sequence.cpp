#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;
int main()
{
    RedirectToFile("in.in");
    Random rnd;
    int n = rnd.irand(1,1e6);
    cout << n << endl;
    Sequence<int> seq(SEQ_GEN_CONF<int> (METHOD_INTEGER, n, 114514, 1919810));
    seq.Generate();
}