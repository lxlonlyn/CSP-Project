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
vector<int> numberList; // 数字序列
vector<int> codeWord;   // 码字序列
int currentMode; // 目前编码器模式。0:大写模式 1:小写模式 2:数字模式
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
int main() {
    scanf("%d%d", &w, &lev); // lev 表示校验级别
    scanf("%s", s);
    n = strlen(s);
    // 步骤一：得到数字序列
    currentMode = 0; // 初始为大写模式
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
    // 步骤三：补充码字
    while ((1 + codeWord.size()) % w != 0) {
        codeWord.push_back(900);
    }
    // 步骤四：输出结果
    codeWord.insert(codeWord.begin(), codeWord.size() + 1);
    for (int i = 0; i < codeWord.size(); ++i) {
        printf("%d\n", codeWord[i]);
    }
    return 0;
}