#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 500010;
const int maxm = 10010;
int n;
int a[maxn];
int sum[maxm];
// sum[i] 表示 p = i 时的贡献
// 注意当 a[i] 发生变化时，p = a[i] + 1
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
    }

    // 去掉数组中相同元素段
    int tot = 0;
    a[++tot] = a[1];
    for (int i = 2; i <= n; ++i) {
        if (a[i] == a[i - 1])
            continue;
        a[++tot] = a[i];
    }
    n = tot;

    // 对非两侧元素进行处理
    for (int i = 2; i < n; ++i) {
        if (a[i] < a[i - 1] && a[i] < a[i + 1]) {
            // 中间小两侧大，贡献 +1
            ++sum[a[i] + 1];
        }
        if (a[i] > a[i - 1] && a[i] > a[i + 1]) {
            // 中间大两侧小，贡献 -1
            --sum[a[i] + 1];
        }
    }

    // 处理两侧元素
    if (n > 1 && a[1] > a[2])
        --sum[a[1] + 1];
    if (n > 1 && a[n] > a[n - 1])
        --sum[a[n] + 1];

    // 计算初始情况 p = 1 时的非零段数 cur
    int cur = 0;
    for (int i = 1; i <= n; ++i) {
        if (a[i] == 0 && a[i - 1] != 0)
            ++cur;
    }
    if (a[n])
        ++cur;

    // 计算并得到最大的非零段数
    int ans = cur;
    for (int i = 2; i <= 10001; ++i) {
        cur += sum[i];
        if (cur > ans) {
            ans = cur;
        }
    }

    printf("%d", ans);
    return 0;
}