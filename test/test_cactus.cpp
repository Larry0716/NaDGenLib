#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;
int main(){
    RedirectToFileFunc("con");
    Cactus c(20);
    c.Generate();
    c.Output(false);
}