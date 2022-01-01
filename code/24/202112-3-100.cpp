#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
const int maxn = 210;
const int mod = 929;
int w, lev;
char s[100010];
int n;
vector<int> numberList;
vector<int> codeWord;
int verifyCodeLen; // 校验码长度
int currentMode;
void checkmode(char c) {
    /*
        检查将要输出的下个字符与目前模式是否匹配，
        若不匹配，则输出对应更改模式步骤。
    */
    if (c >= '0' && c <= '9') {
        if (currentMode != 2) {
            numberList.push_back(28);
            currentMode = 2;
        }
    } else if (c >= 'a' && c <= 'z') {
        if (currentMode != 1) {
            numberList.push_back(27);
            currentMode = 1;
        }
    } else if (c >= 'A' && c <= 'Z') {
        if (currentMode == 1) {
            numberList.push_back(28);
            numberList.push_back(28);
            currentMode = 0;
        }
        if (currentMode == 2) {
            numberList.push_back(28);
            currentMode = 0;
        }
    }
}
vector<int> get_gx(int k) {
    // 根据 k 计算 g(x)
    vector<int> res;
    res.push_back(mod - 3);
    res.push_back(1);
    int a0 = 3;
    for (int i = 2; i <= k; ++i) {
        a0 = (a0 * 3) % mod;
        res.insert(res.begin(), 0); // 在最低位插入 1，即整体次数 +1
        for (int j = 0; j < i; ++j) {
            res[j] = (res[j] - (a0 * res[j + 1]) % mod + mod) % mod;
        }
    }
    return res;
}

void get_verify_code() {
    // 计算校验码并输出
    vector<int> tmp;
    vector<int> g = get_gx(verifyCodeLen);
    // 初始化 x^kd(x)
    for (int i = 1; i <= verifyCodeLen; ++i) {
        tmp.push_back(0);
    }
    for (int i = codeWord.size() - 1; i >= 0; --i) {
        tmp.push_back(codeWord[i]);
    }
    // 多项式长除法计算结果
    for (int i = tmp.size() - 1; i >= verifyCodeLen; --i) {
        int val = tmp[i];
        for (int j = 0; j < g.size(); ++j) {
            tmp[i - j] =
                (tmp[i - j] - (val * g[g.size() - 1 - j]) % mod + mod) % mod;
        }
    }
    // 将 -r(x) 转化为 r(x)
    for (int i = 0; i < verifyCodeLen; ++i) {
        // 注意：不能直接 mod - tmp[i]，因为 tmp[i] 可能为 0
        tmp[i] = (mod - tmp[i]) % mod;
    }
    // 输出结果
    for (int i = verifyCodeLen - 1; i >= 0; --i) {
        printf("%d\n", tmp[i]);
    }
}
int main() {
    scanf("%d%d", &w, &lev);
    scanf("%s", s);
    n = strlen(s);
    // 步骤一：得到数字序列
    currentMode = 0;
    for (int i = 0; i < n; ++i) {
        checkmode(s[i]);
        if (s[i] >= '0' && s[i] <= '9') {
            numberList.push_back(s[i] - '0');
        } else if (s[i] >= 'a' && s[i] <= 'z') {
            numberList.push_back(s[i] - 'a');
        } else if (s[i] >= 'A' && s[i] <= 'Z') {
            numberList.push_back(s[i] - 'A');
        }
    }
    if (numberList.size() % 2)
        numberList.push_back(29);
    // 步骤二：转换为码字
    for (int i = 0; i < numberList.size(); i += 2) {
        codeWord.push_back(30 * numberList[i] + numberList[i + 1]);
    }
    if (lev == -1)
        verifyCodeLen = 0;
    else {
        verifyCodeLen = 1;
        for (int i = 0; i <= lev; ++i) {
            verifyCodeLen *= 2;
        }
    }
    // 步骤三：补充码字
    while ((1 + verifyCodeLen + codeWord.size()) % w != 0) {
        codeWord.push_back(900);
    }
    codeWord.insert(codeWord.begin(), codeWord.size() + 1);
    // 步骤四：输出数据码结果
    for (int i = 0; i < codeWord.size(); ++i) {
        printf("%d\n", codeWord[i]);
    }
    // 步骤五：如果有校验码，则计算并输出
    if (verifyCodeLen != 0) {
        get_verify_code();
    }
    return 0;
}