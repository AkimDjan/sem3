#include <iostream>
#include <cmath>
using namespace std;


int main() {
    int num=8; int k =2025;
    int last_num=num;
    for (int i = 2; i<k+1; i++) {
        last_num = last_num*num %10;
    }
    cout << last_num;
}