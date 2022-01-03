# 序列问题新解

## 思路

初看此题。有以下几个判断点：

- 复杂度$O(n^2)$会TLE
- 提示说error可能很大，保险起见开`long long`

然后，思考本题的解法。

根据第一题的铺垫，我们可以利用乘法快速计算连续且相同的值之和。在第二题中，可以发现，$f(x)$和$g(x)$都满足单调不减，且有大量的相同值，成块分布。

所谓“成块”，即类似分段函数的每个区间，如样例1中：
$$
f(x)=\left\{
\begin{aligned}
0,x\in[0,1]\\
1,x\in[2,4]\\
2,x\in[5,7]\\
3,x\in[8,9]\\
\end{aligned}
\right.\\

g(x)=\left\{
\begin{aligned}
0,x\in[0,1]\\
1,x\in[2,3]\\
2,x\in[4,5]\\
..........
\end{aligned}
\right.\\
$$


可以观察到如下规律：

- $f(x)$区间为$[A[i],A[i+1]-1]$。特殊地，最后一个区间为$[A[n], N-1]$；
- $g(x)$区间长度为$r$，即一个固定值$\lfloor \frac N {n+1}\rfloor$

延申第一题的提示思路，很自然地，如果我们能够对其中一个函数的**每个区间**运用乘法计算，就可以降低复杂度。这里我针对$f(x)$的每一个区间进行整体计算。

观察题目所求。所求的是两函数对应位置上差的绝对值之和。

我们单独考虑每一个$f(x)$的区间，每个区间上$f(x)$的值相同。想办法快速求得一个$f(x)$区间对答案的贡献。可以观察到，对于一个区间上的下标$i$,可能存在$g(i)\ge f(i)$，也可能存在$g(i)<f(i)$。求绝对值时，前者用$g(x)-f(x)$，后者用$f(x)-g(x)$。

观察到，由于$g(x)$**单调不减**的性质，我们可以得到：对于该区间，一定存在一个下标$p$,如同一个分界线，当$i\ge p$时，有$g(i)\ge f(i)$，当$i<p$，有$g(i)<f(i)$。这样，就把该区间分成了两个“小区间”。这样我们就可以用“乘法思想”来加速两个“小区间”的求解了。
$$
本区间对答案的贡献=len(A[i],p-1)\times f(x)-\sum_{x=A[i]}^{p-1}g(x)\\
+\sum_{x=p}^{max}g(x)- len(p,max)\times f(x)
$$


上式中，$f(x)$是一个常数，所以乘以“小区间”的长度即可；$g(x)$的求和，大家可以发挥数学思维：因为$g(x)$其实非常规律，它的每一块是定长的，我们可以通过除法和取余来确定相同值的数量，再利用乘法思想求和。灵活实现，在$O(n)$时间内求出即可。

这个$p$的具体值可以通过在$g(x)$中二分查找，$O(\log n)$时间内求出，n为区间的长度。

举一个栗子：

| x    | ...  | 4    | 5    | 6    | 7    | ...  |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| f(x) | ...  | 2    | 2    | 2    | 2    | ...  |
| g(x) | ...  | 1    | 1    | 2    | 2    | ...  |

上面的表格截取了一个小区间，$f(x)$的值固定2，$p=6$,那么p的左边用$f(x)-g(x)$，$p$的右边用$g(x)-f(x)$。

当然，有一个特殊的边界情况，那就是该区间上有可能所有的$g(x)$都绝对大于$f(x)$，或者小于。这时候$p$可能会在区间外。该情况大家可以对$p$设置初值，然后在写完二分后加以判断即可。

根据上述思路求得每一个区间，然后把所有区间的贡献之和输出，本题得解。思路比较繁杂，但每一步的思维难度不大，希望能帮助大家。

## C++实现

代码比较冗杂。仅供思路的细节参考：

```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<map>
#include<queue>
#include<cmath>
#include<cstring>

using namespace std;
typedef long long LL;
typedef unsigned long long uLL;
typedef pair<int, int> pii;
const int mod = 1e9 + 7;
const int maxn = 1e5 + 5;

LL N, n;
LL arr[maxn];   // 题中 A 数组
vector<LL> f;   // 存储每个区间上f的值
vector<pii> pos;// 存储每个区间的边界，是左闭右闭
LL r, ans;      // 题中的 r，ans为计算的答案

// 下面的函数用于计算g(x)在区间上的和
// 这一步比较细，具体可以灵活实现
// 下面的思路还是比较冗杂的
LL totG(LL be, LL ed) {
    // 右边界小于左边界，返回0
    if(ed < be) {
        return 0;
    }
    // 两边界重合，返回一个g值
    if(be == ed) {
        return be / r;
    }
    // 如果两边界g值相同，返回该值乘以区间长度
    if(be / r == ed / r) {
        return (be / r) * (ed - be + 1); 
    }
    // 将区间分为三部分，分别累计
    LL tot = 0;
    // 对于左边界，其值为be/r,数目为 r - be % r
    tot += (r - (be % r)) * (be / r);
    // 对于右边界，其值为ed/r，数目为 ed % r + 1
    tot += (ed % r + 1) * (ed / r);
    // 对于不在边界上的g值，我们用等差数列求和公式
    if(ed / r - be / r > 1) {
        be = be/r + 1;
        ed = ed /r - 1;
        tot += r * ((be + ed) * (ed - be + 1) / 2);
    }
    return tot;
}

void solve() {
    // 输入
    scanf("%lld%lld", &n, &N);
    r = N / (n + 1);
    for(int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }
    // 根据数组，生成f(x)的每个区间，值存入f，区间边界存入pos
    LL last = 0ll;  // 记录上一个边界
    // 这里的逻辑参考第一题
    for(int i = 1; i <= n; i++) {
        if(arr[i] > arr[last]) {
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
    for(int i = 0; i < si; i++) {
        LL num = f[i];
        LL be = pos[i].first;
        LL ed = pos[i].second;
        LL length = ed - be + 1;
        // 因为be和ed在二分过程其值发生变化，所以下面再存一份
        LL bbe = be, eed = ed;
        // 下面使用二分，在g中寻找分界p
        LL pin = -1;
        while(be <= ed) {
            LL mid = (be + ed) / 2;
            LL cur = mid / r;
            if(cur >= num) {
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
    // 输出
    printf("%lld", ans);
}

int main() {
    int t;
    t = 1;
    while(t--) {
        solve();
    }
    return 0;
}
```

