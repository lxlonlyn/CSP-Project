#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
#define ll long long
const int maxn = 100010;
int n, N;
int a[maxn];
int rat, f, g;
ll ans = 0;
int main() {
    scanf("%d%d", &n, &N);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
    }
    rat = N / (n + 1); // 为了防止冲突，题目中 r 改为 rat
    int cur = 0;       // 用来计算 f(x)
    bool flag = false; // 如果需要更新 f(x) 值，则 flag = true
    for (int l = 0, r; l < N; l = r + 1) {
        flag = false;
        // 利用 f(x) 的值确定 r 的范围
        if (cur < n)
            r = a[cur + 1] - 1;
        else
            r = N - 1;
        // 判断 f(x), g(x) 谁先变化，选择较小的区间
        if ((l / rat + 1) * rat - 1 < r) {
            // 如果 g(x) 先变化，则改为选择 g(x)
            r = (l / rat + 1) * rat - 1;
        } else {
            // 如果 f(x) 先变化，则确定选择 f(x)，计算后更新 f(x)
            flag = true;
        }
        // [l, r] 区间内的值是相等的，可以求和
        ans += 1ll * (r - l + 1) * abs(l / rat - cur);
        // 更新 f(x) 的值
        if (flag)
            ++cur;
    }
    printf("%lld\n", ans);
    return 0;
}