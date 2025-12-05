#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> findLargestMultipleSubsequence(const vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return {};
    // Создаем копию массива и сортируем для удобства
    vector<int> sortedArr = arr;
    sort(sortedArr.begin(), sortedArr.end());
    // dp[i] - длина наибольшей кратной подпоследовательности, заканчивающейся на arr[i]
    vector<int> dp(n, 1);
    // parent[i] - индекс предыдущего элемента в подпоследовательности
    vector<int> parent(n, -1);
    int maxLength = 1; int lastIndex = 0;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            // Проверяем условие кратности: arr[i] должен делиться на arr[j]
            if (sortedArr[i] % sortedArr[j] == 0) {
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
        }
        if (dp[i] > maxLength) {
            maxLength = dp[i];
            lastIndex = i;
        }
    }
    
    // Восстанавливаем подпоследовательность
    vector<int> result;
    int current = lastIndex;
    while (current != -1) {
        result.push_back(sortedArr[current]);
        current = parent[current];
    }
    
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 6, 8, 12, 24};
    
    cout << "Исходный массив: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    
    vector<int> result = findLargestMultipleSubsequence(arr);
    
    cout << "Наибольшая кратная подпоследовательность: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;
    
    cout << "Длина: " << result.size() << endl;
    
    return 0;
}