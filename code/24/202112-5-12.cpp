#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
#define ll long long
#define il inline
const int maxn = 500010;
int n;
int k1, k2;
vector<int> G[maxn];
ll ans = 0;
void dfs(int s, int u, int fa, int minval, int maxval) {
    /*
        s：出发点
        u：目前遍历到的点
        fa：u 从 fa 递归而来
        minval：(s,u) 路径上的最小值
        maxval：(s,u) 路径上的最大值
    */
    for (int i = 0; i < G[u].size(); ++i) {
        // 遍历每一个能达到的节点
        int v = G[u][i];
        if (v == fa)
            // 防止递归时 u -> v -> u 情况发生
            continue;
        if (min(s, v) - k1 <= min(minval, v) &&
            max(s, v) + k2 >= max(maxval, v)) {
            // 满足条件，答案增加
            ++ans;
        }
        // 更新 min,max，继续递归
        dfs(s, v, u, min(minval, v), max(maxval, v));
    }
}
int main() {
    scanf("%d%d%d", &n, &k1, &k2);
    int ru, rv;
    for (int i = 1; i < n; ++i) {
        scanf("%d%d", &ru, &rv);
        G[ru].push_back(rv);
        G[rv].push_back(ru);
    }

    for (int i = 1; i <= n; ++i) {
        // 遍历每一个起始点，不包括 x=y 型
        dfs(i, i, -1, i, i);
    }
    ans /= 2;

    // 加上 x=y 型
    ans += n;

    printf("%lld\n", ans);
    return 0;
}