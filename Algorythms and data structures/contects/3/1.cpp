#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }
    vector<int> dp(n + 1);
    dp[0] = 0;
    for (int i = 1; i < n; i++) {
        if (v[i] == 0) {
            dp[i] = 0;
        }
        else {
            dp[i] = dp[i - 1] + v[i];
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        if (i - 1 > 0) {
            if (v[i - 1] == 1) {
                dp[i - 1] = dp[i];
            }
        }
        else {
            if (v[i] == 1) {
                dp[i] = dp[i + 1];
            }
        }
    }
    int res  = 0;
    int res_ind = 0;
    for (int i = 0; i < n; i++) {
        if (v[i] == 0 && (i > 0 && i < n)) {
            if (res < dp[i - 1] + dp[i + 1] + 1) {
                res = dp[i - 1] + dp[i + 1] + 1;
                res_ind = i;
            }
        }
        if (v[0] == 0) {
            if (res < dp[1]) {
                res = dp[1] + 1;
                res_ind = 0;
            }
        }
    }
    cout << res_ind << endl;
}
