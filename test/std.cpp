#include <bits/stdc++.h>
using namespace std;
int main(){
	int n;
	cin>>n;
	int ans = 0;
	while(n--){
		int buffer;
		cin>>buffer;
		ans += buffer; 
	}
	cout<<ans<<endl; 
	return 0;
} 
