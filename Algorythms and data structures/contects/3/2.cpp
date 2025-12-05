#include <iostream>
#include <vector>


using namespace std;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) {
        return 0;
    }

    vector<vector<long long>> a(M, vector<long long>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> a[i][j];
        }
    }

    const long long NEG_INF = (long long)-4e18;

    vector<vector<vector<long long>>> dp(
        M, vector<vector<long long>>(N, vector<long long>(N, NEG_INF))
    );

    dp[0][0][N-1] = a[0][0] + (0 == N-1 ? 0 : a[0][N-1]);

    for (int i = 1; i < M; ++i) {
        for (int j1 = 0; j1 < N; ++j1) {
            for (int j2 = 0; j2 < N; ++j2) {
                long long best_prev = NEG_INF;

                for (int dj1 = -1; dj1 <= 1; ++dj1) {
                    int pj1 = j1 + dj1;
                    if (pj1 < 0 || pj1 >= N) continue;

                    for (int dj2 = -1; dj2 <= 1; ++dj2) {
                        int pj2 = j2 + dj2;
                        if (pj2 < 0 || pj2 >= N) continue;

                        best_prev = max(best_prev, dp[i-1][pj1][pj2]);
                    }
                }

                if (best_prev == NEG_INF) {
                    dp[i][j1][j2] = NEG_INF;
                    continue;
                }

                long long add = a[i][j1];
                if (j1 != j2) add += a[i][j2];

                dp[i][j1][j2] = best_prev + add;
            }
        }
    }

    long long ans = dp[M-1][0][N-1];
    cout << ans << "\n";

    return 0;
}
