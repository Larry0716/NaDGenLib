#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;
int main(){
    RedirectToFileFunc("con");
    DAG dag(2,1);
    dag.Generate();
    dag.Output(true);
}