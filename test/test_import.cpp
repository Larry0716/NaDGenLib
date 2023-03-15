#include <bits/stdc++.h>
#include "..\genlib.h"
using namespace std;
using namespace Generator;

int main(){
	RedirectToFile("con");
	NoRootTree nrt(10);
	nrt.Generate();
	nrt.output();
	return 0;
}
