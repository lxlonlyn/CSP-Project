#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;
typedef long long LL;
typedef unsigned long long uLL;
typedef pair<int, int> pii;
const int mod = 1e9 + 7;
const int maxn = 1e5 + 5;

LL N, n;
LL arr[maxn];    // 题中 A 数组
vector<LL> f;    // 存储每个区间上f的值
vector<pii> pos; // 存储每个区间的边界，是左闭右闭
LL r, ans;       // 题中的 r，ans为计算的答案

// 下面的函数用于计算g(x)在区间上的和
// 这一步比较细，具体可以灵活实现
// 下面的思路还是比较冗杂的
LL totG(LL be, LL ed) {
    // 右边界小于左边界，返回0
    if (ed < be) {
        return 0;
    }
    // 两边界重合，返回一个g值
    if (be == ed) {
        return be / r;
    }
    // 如果两边界g值相同，返回该值乘以区间长度
    if (be / r == ed / r) {
        return (be / r) * (ed - be + 1);
    }
    // 将区间分为三部分，分别累计
    LL tot = 0;
    // 对于左边界，其值为be/r,数目为 r - be % r
    tot += (r - (be % r)) * (be / r);
    // 对于右边界，其值为ed/r，数目为 ed % r + 1
    tot += (ed % r + 1) * (ed / r);
    // 对于不在边界上的g值，我们用等差数列求和公式
    if (ed / r - be / r > 1) {
        be = be / r + 1;
        ed = ed / r - 1;
        tot += r * ((be + ed) * (ed - be + 1) / 2);
    }
    return tot;
}

void solve() {
    // 输入
    scanf("%lld%lld", &n, &N);
    r = N / (n + 1);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }
    // 根据数组，生成f(x)的每个区间，值存入f，区间边界存入pos
    LL last = 0ll; // 记录上一个边界
    // 这里的逻辑参考第一题
    for (int i = 1; i <= n; i++) {
        if (arr[i] > arr[last]) {
            f.push_back(last);
            pos.push_back({arr[last], arr[i] - 1});
            last = i;
        }
    }
    // 单独处理下最后一个区间，即[A[n],N-1]
    f.push_back(n);
    pos.push_back({arr[last], N - 1});
    // 对于每个f区间，将g分成两个小区间
    int si = f.size();
    for (int i = 0; i < si; i++) {
        LL num = f[i];
        LL be = pos[i].first;
        LL ed = pos[i].second;
        LL length = ed - be + 1;
        // 因为be和ed在二分过程其值发生变化，所以下面再存一份
        LL bbe = be, eed = ed;
        // 下面使用二分，在g中寻找分界p
        LL pin = -1;
        while (be <= ed) {
            LL mid = (be + ed) / 2;
            LL cur = mid / r;
            if (cur >= num) {
                pin = mid;
                ed = mid - 1;
            } else {
                be = mid + 1;
            }
        }
        // 如果f的值一直大于g，p值不会被二分的过程赋值，所以还是初值
        if (pin == -1) {
            ans += num * length - totG(bbe, eed);
        } else {
            // 左边的用f-g，右边用g-f。就算g的值一直大于f，即左边的部分长度为0
            ans += num * (pin - bbe) - totG(bbe, pin - 1);
            ans += totG(pin, eed) - num * (eed - pin + 1);
        }
    }
    printf("%lld", ans);
}

int main() {
    int t;
    t = 1;
    while (t--) {
        solve();
    }
    return 0;
}