#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int maxn = 200010;
const int INF = 1e9 + 10;
int n, m, k;
#define ls o << 1
#define rs ls | 1
struct treenode {
    // 当前节点的值，若不唯一则为 INF；lazy 为 INF 表示无延迟更新
    int val, lazy_val;
    // 当前占用 id，若存在除 0 以外两种 id 则为 -1；lazy 为 -1 表示无延迟更新
    int id1, lazy_id1;
    // 当前占用 id，若存在两种 id 则为 -1；lazy 为 -1 表示无延迟更新
    int id2, lazy_id2;
    // 上次占用 id，若存在两种 id 则为 -1；lazy 为 -1表示无延迟更新
    int lid, lazy_lid;
} tree[maxn << 5];

void pushup(int o) {
    // 线段树上传操作，合并左右子树结果
    // val 的合并
    tree[o].val = (tree[ls].val == tree[rs].val) ? tree[ls].val : INF;
    // id1 的合并
    if (tree[ls].id1 == -1 || tree[rs].id1 == -1) {
        tree[o].id1 = -1;
    } else if (tree[ls].id1 == tree[rs].id1) {
        tree[o].id1 = tree[ls].id1;
    } else if (tree[ls].id1 == 0) {
        tree[o].id1 = tree[rs].id1;
    } else if (tree[rs].id1 == 0) {
        tree[o].id1 = tree[ls].id1;
    } else {
        tree[o].id1 = -1;
    }
    // id2 的合并
    if (tree[ls].id2 == -1 || tree[rs].id2 == -1) {
        tree[o].id2 = -1;
    } else if (tree[ls].id2 == tree[rs].id2) {
        tree[o].id2 = tree[ls].id2;
    } else {
        tree[o].id2 = -1;
    }
    // lid 的合并
    if (tree[ls].lid == -1 || tree[rs].lid == -1) {
        tree[o].lid = -1;
    } else if (tree[ls].lid == tree[rs].lid) {
        tree[o].lid = tree[ls].lid;
    } else {
        tree[o].lid = -1;
    }
}

void pushdown(int o) {
    // 线段树标记下传操作
    if (tree[o].lazy_val != INF) {
        tree[ls].val = tree[rs].val = tree[o].lazy_val;
        tree[ls].lazy_val = tree[rs].lazy_val = tree[o].lazy_val;
        tree[o].lazy_val = INF;
    }
    if (tree[o].lazy_id1 != -1) {
        tree[ls].id1 = tree[rs].id1 = tree[o].lazy_id1;
        tree[ls].lazy_id1 = tree[rs].lazy_id1 = tree[o].lazy_id1;
        tree[o].lazy_id1 = -1;
    }
    if (tree[o].lazy_id2 != -1) {
        tree[ls].id2 = tree[rs].id2 = tree[o].lazy_id2;
        tree[ls].lazy_id2 = tree[rs].lazy_id2 = tree[o].lazy_id2;
        tree[o].lazy_id2 = -1;
    }
    if (tree[o].lazy_lid != -1) {
        tree[ls].lid = tree[rs].lid = tree[o].lazy_lid;
        tree[ls].lazy_lid = tree[rs].lazy_lid = tree[o].lazy_lid;
        tree[o].lazy_lid = -1;
    }
}

void build(int o, int l, int r) {
    // 线段树初始化建树
    if (l == r) {
        tree[o].val = 0;
        tree[o].lazy_val = INF;
        tree[o].id1 = tree[o].id2 = tree[o].lid = 0;
        tree[o].lazy_id1 = tree[o].lazy_id2 = tree[o].lazy_lid = -1;
        return;
    }
    int mid = (l + r) >> 1;
    build(ls, l, mid);
    build(rs, mid + 1, r);
    tree[o].lazy_val = INF;
    pushup(o);
}

#define ALLOK -2
int find_right(int o, int l, int r, int ql, int qid) {
    // 操作一中，固定左端点，寻找右端点可能的最大值
    // 这里没有考虑和右端点的比较，直接寻找了最大的可能值
    pushdown(o);
    if (r < ql || tree[o].id1 == qid || tree[o].id1 == 0) {
        // 全部符合条件
        return ALLOK;
    } else if (tree[o].id2 != -1) {
        // 不符合条件，返回该区域左边第一个
        return l - 1;
    } else {
        // 需要寻找确切位置
        // 先查找左区间，如果左区间全满足则再寻找右区间
        int mid = (l + r) >> 1;
        int leftPart = (ql <= mid) ? find_right(ls, l, mid, ql, qid) : ALLOK;
        return (leftPart == ALLOK) ? find_right(rs, mid + 1, r, ql, qid)
                                   : leftPart;
    }
}
#undef ALLOK

void modify_val(int o, int l, int r, int ql, int qr, int val, int id,
                bool ignoreLid = false) {
    // 若 val = INF 代表不需要对 val 进行处理
    // 若 ignoreLid = true 则不对 lid 进行更改
    if (l >= ql && r <= qr) {
        if (val != INF)
            tree[o].val = tree[o].lazy_val = val;
        tree[o].id1 = tree[o].lazy_id1 = id;
        tree[o].id2 = tree[o].lazy_id2 = id;
        if (!ignoreLid)
            tree[o].lid = tree[o].lazy_lid = id;
        return;
    }
    pushdown(o);
    int mid = (l + r) >> 1;
    if (ql <= mid) {
        modify_val(ls, l, mid, ql, qr, val, id, ignoreLid);
    }
    if (qr > mid) {
        modify_val(rs, mid + 1, r, ql, qr, val, id, ignoreLid);
    }
    pushup(o);
}

bool is_same_id(int o, int l, int r, int ql, int qr, int id,
                bool isRecover = false) {
    // 判断该区域 id 和 lid 是否满足条件
    if (l >= ql && r <= qr) {
        if (isRecover) {
            // 检查 id = 0 且 lid = id
            return (tree[o].id2 == 0 && tree[o].lid == id);
        } else {
            // 检查 id = id
            return (tree[o].id2 == id);
        }
    }
    pushdown(o);
    int mid = (l + r) >> 1;
    bool isSame = true;
    if (ql <= mid) {
        isSame = isSame && is_same_id(ls, l, mid, ql, qr, id, isRecover);
    }
    if (qr > mid && isSame) {
        isSame = isSame && is_same_id(rs, mid + 1, r, ql, qr, id, isRecover);
    }
    return isSame;
}

int query_val(int o, int l, int r, int p) {
    // 线段树单点求值：val
    if (p >= l && p <= r && tree[o].val != INF) {
        return tree[o].val;
    }
    pushdown(o);
    int mid = (l + r) >> 1;
    if (p <= mid)
        return query_val(ls, l, mid, p);
    else
        return query_val(rs, mid + 1, r, p);
}

int query_id(int o, int l, int r, int p) {
    // 线段树单点求值：id2
    if (p >= l && p <= r && tree[o].id2 != -1) {
        return tree[o].id2;
    }
    pushdown(o);
    int mid = (l + r) >> 1;
    if (p <= mid)
        return query_id(ls, l, mid, p);
    else
        return query_id(rs, mid + 1, r, p);
}

#undef ls
#undef rs

struct instruction {
    int opt, id, l, r, x;
} inst[maxn];
// numList 存储所有可能出现的数，totnum 表示个数
int numList[maxn << 2], totnum;
void discretization() {
    // 离散化操作
    sort(numList + 1, numList + 1 + totnum);
    totnum = unique(numList + 1, numList + 1 + totnum) - numList - 1;
    m = totnum;
    for (int i = 1; i <= k; ++i) {
        if (inst[i].opt == 0 || inst[i].opt == 1 || inst[i].opt == 2) {
            inst[i].l =
                lower_bound(numList + 1, numList + 1 + totnum, inst[i].l) -
                numList;
            inst[i].r =
                lower_bound(numList + 1, numList + 1 + totnum, inst[i].r) -
                numList;
        } else {
            inst[i].x =
                lower_bound(numList + 1, numList + 1 + totnum, inst[i].x) -
                numList;
        }
    }
}

int main() {
    scanf("%d%d%d", &n, &m, &k);
    numList[++totnum] = 1;
    numList[++totnum] = m;
    for (int i = 1; i <= k; ++i) {
        scanf("%d", &inst[i].opt);
        if (inst[i].opt == 0) {
            scanf("%d%d%d%d", &inst[i].id, &inst[i].l, &inst[i].r, &inst[i].x);
            numList[++totnum] = inst[i].l;
            numList[++totnum] = inst[i].r;
            // 注意边界问题，为了方便这里把交界处两点分开了，下同
            if (inst[i].l != 1)
                numList[++totnum] = inst[i].l - 1;
            if (inst[i].r != m)
                numList[++totnum] = inst[i].r + 1;
        } else if (inst[i].opt == 1) {
            scanf("%d%d%d", &inst[i].id, &inst[i].l, &inst[i].r);
            numList[++totnum] = inst[i].l;
            numList[++totnum] = inst[i].r;
            if (inst[i].l != 1)
                numList[++totnum] = inst[i].l - 1;
            if (inst[i].r != m)
                numList[++totnum] = inst[i].r + 1;
        } else if (inst[i].opt == 2) {
            scanf("%d%d%d", &inst[i].id, &inst[i].l, &inst[i].r);
            numList[++totnum] = inst[i].l;
            numList[++totnum] = inst[i].r;
            if (inst[i].l != 1)
                numList[++totnum] = inst[i].l - 1;
            if (inst[i].r != m)
                numList[++totnum] = inst[i].r + 1;
        } else {
            scanf("%d", &inst[i].x);
            // 对于查询的数，不需要进行离散化，查找第一个比它大的数即可
        }
    }

    // 离散化处理
    discretization();

    // 线段树初始化建树
    build(1, 1, m);

    // 进行操作
    for (int i = 1; i <= k; ++i) {
        if (inst[i].opt == 0) {
            // 写入操作：先求得范围，再进行填充
            int r = find_right(1, 1, m, inst[i].l, inst[i].id);
            if (r == -2)
                // r = -2 代表全部满足
                r = inst[i].r;
            else
                r = min(r, inst[i].r);
            if (inst[i].l <= r) {
                printf("%d\n", numList[r]); // 注意返回离散化前的值
                modify_val(1, 1, m, inst[i].l, r, inst[i].x, inst[i].id);
            } else {
                printf("-1\n");
            }
        } else if (inst[i].opt == 1) {
            // 删除操作：先判断是否可行，之后执行
            if (is_same_id(1, 1, m, inst[i].l, inst[i].r, inst[i].id)) {
                printf("OK\n");
                modify_val(1, 1, m, inst[i].l, inst[i].r, INF, 0, true);
            } else {
                printf("FAIL\n");
            }
        } else if (inst[i].opt == 2) {
            // 恢复操作：先判断是否可行，之后执行
            if (is_same_id(1, 1, m, inst[i].l, inst[i].r, inst[i].id, true)) {
                printf("OK\n");
                modify_val(1, 1, m, inst[i].l, inst[i].r, INF, inst[i].id,
                           true);
            } else {
                printf("FAIL\n");
            }
        } else if (inst[i].opt == 3) {
            // 读取操作：分别读取 id 和 val
            int id = query_id(1, 1, m, inst[i].x);
            int val = query_val(1, 1, m, inst[i].x);
            if (id == 0) {
                printf("0 0\n");
            } else {
                printf("%d %d\n", id, val);
            }
        }
    }
    return 0;
}
