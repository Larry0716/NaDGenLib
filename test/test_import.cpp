#include "../genlib.h"
#include <bits/stdc++.h>
using namespace Generator;
int g(int n){
	return n*n;
}
void gen(){
	std::ofstream &cout = *rout;
	Sequence<int> seq;
	int n = rand();
	cout<<n<<std::endl;
	seq.SetConfig(SEQ_GEN_CONF<int>(METHOD_CUSTOM, n, g));
	seq.Generate();
}
int main(){
	srand(time(0));
	RegisterStdProgram("std.exe");
	AutoGenerate(".\\data\\test%d.in",1,10,true,gen); 
    return 0;
}
