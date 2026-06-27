#include <iostream>
#include <cmath>
using namespace std;

long double f(long double x) {
    return pow((4 - x * x), 0.5);
}

int main() {
    double r = 2.0;
    long long int N; cin>>N;
    long double dx = r/N;
    long double pi = 0.0;
    long double last_x = 0;
    for (int i = 0; i < N; i++) {
        pi+=f(last_x)*dx;
        last_x+=dx;
    }
    cout<<"Pi="<<pi<<"\n";
}