#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;

int main() {
    int n; cin>>n;
    vector<int> v;
    for(int i=0; i<n; i++) { 
        int a; cin>>a;
        v.push_back(a);
    }
    int targ; cin>>targ;
    unordered_map<int,int> map;
    for (int i=0; i<n; i++) {
        map[v[i]]=i;
    }
    for (int i=0; i<n; i++) {
        int second; second=targ-v[i];
        if (map.count(second)) {
            cout<<"Нашел пару:"<<v[i]<<" и "<<second;
            return 0;
        }
    }
    cout<<"Нету пар\n";
}