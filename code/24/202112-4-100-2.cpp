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
struct treenode {
    // 左右子节点编号
    int lc, rc;
    // 当前节点的值，若不唯一则为 INF；lazy 为 INF 表示无延迟更新
    int val, lazy_val;
    // 当前占用 id，若存在除 0 以外两种 id 则为 -1；lazy 为 -1 表示无延迟更新
    int id1, lazy_id1;
    // 当前占用 id，若存在两种 id 则为 -1；lazy 为 -1 表示无延迟更新
    int id2, lazy_id2;
    // 上次占用 id，若存在两种 id 则为 -1；lazy 为 -1表示无延迟更新
    int lid, lazy_lid;
} tree[maxn << 5];
int cnt; // 线段树节点个数
#define ls tree[o].lc
#define rs tree[o].rc
int insert_node() {
    // 向线段树中插入一个节点
    ++cnt;
    tree[cnt].lc = tree[cnt].rc = 0;
    tree[cnt].val = 0;
    tree[cnt].id1 = tree[cnt].id2 = 0;
    tree[cnt].lid = 0;
    tree[cnt].lazy_val = INF;
    tree[cnt].lazy_id1 = tree[cnt].lazy_id2 = -1;
    tree[cnt].lid = -1;
    return cnt;
}

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
    // 如果对应点未被创建，则进行创建
    if (!ls)
        ls = insert_node();
    if (!rs)
        rs = insert_node();
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

int main() {
    scanf("%d%d%d", &n, &m, &k);
    // 创建根节点
    insert_node();
    // 进行操作
    int r_opt, r_id, r_l, r_r, r_x, r_p;
    while (k--) {
        scanf("%d", &r_opt);
        if (r_opt == 0) {
            // 写入
            scanf("%d%d%d%d", &r_id, &r_l, &r_r, &r_x);
            int r = find_right(1, 1, m, r_l, r_id);
            if (r == -2)
                r = r_r;
            else
                r = min(r, r_r);
            if (r_l <= r) {
                printf("%d\n", r);
                modify_val(1, 1, m, r_l, r, r_x, r_id);
            } else {
                printf("-1\n");
            }
        } else if (r_opt == 1) {
            // 删除
            scanf("%d%d%d", &r_id, &r_l, &r_r);
            if (is_same_id(1, 1, m, r_l, r_r, r_id)) {
                printf("OK\n");
                modify_val(1, 1, m, r_l, r_r, INF, 0, true);
            } else {
                printf("FAIL\n");
            }
        } else if (r_opt == 2) {
            // 恢复
            scanf("%d%d%d", &r_id, &r_l, &r_r);
            if (is_same_id(1, 1, m, r_l, r_r, r_id, true)) {
                printf("OK\n");
                modify_val(1, 1, m, r_l, r_r, INF, r_id, true);
            } else {
                printf("FAIL\n");
            }
        } else {
            // 查询
            scanf("%d", &r_p);
            int id = query_id(1, 1, m, r_p);
            int val = query_val(1, 1, m, r_p);
            if (id == 0) {
                printf("0 0\n");
            } else {
                printf("%d %d\n", id, val);
            }
        }
    }
    return 0;
}