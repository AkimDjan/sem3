#include <iostream> 
#include <vector>
using namespace std;

int main() {
    int n; cin>>n;
    vector<int> v;
    for(int i=0; i<n; i++) { 
        int a; cin>>a;
        v.push_back(a);
    }
    int max=-1e9; int min=1e9;
    for (int i=0; i<n; i++) {
        if(v[i]>=max) {max=v[i];}
        if(v[i]<=min) {min=v[i];}
    }
    cout<<max<<" "<<min<<"\n";
}