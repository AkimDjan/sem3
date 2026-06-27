#include <iostream>
#include <vector>
using namespace std;

int main() {
    cout<<"Введите размер массива:"; int n; cin>>n; cout<<"\n";
    cout<<"Вводите числа через пробел чтобы заполнить массив:";
    vector<int> v;
    for (int i=0; i<n; i++) {
        int a; cin>>a;
        v.push_back(a);
    }
    cout<<"\n";
    int sum=0;
    for (int i=0; i<n; ++i) {
        if (v[i]%2==0) {
            sum+=v[i];
        }
    }
    cout<<"Сумма четных чиел:"<<sum<<"\n";
}