#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
#define ll long long
#define il inline
const int maxn = 110;
int a[maxn], b[maxn];
int n;
int sum;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &b[i]);
    }
    // 计算最大值：满足 a[i]=b[i]
    sum = 0;
    for (int i = 1; i <= n; ++i)
        sum += b[i];
    printf("%lld\n", sum);
    // 计算最小值：在变化时 a[i]=b[i]，其余为 0
    sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (i == 1 || b[i] != b[i - 1])
            sum += b[i];
    }
    printf("%d\n", sum);
    return 0;
}