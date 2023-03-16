#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;
const int vmin = 114514;
const int vmax = 1919810;
void CustomFunction(){
    AntiSPFA atspfa(10,1e5/10);
    atspfa.AutoMode(vmin,vmax);
}
int main(){
    AutoGenerate(".\\data\\test%d.in",1,10,CustomFunction);
    return 0;
}