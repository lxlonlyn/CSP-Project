## problem 文件夹说明

### 内容

历年题目的题面以及分析。

### 题面文件格式

1. 文件命名格式为 `考试场次-题目编号-p.tex`；
2. 题目背景、题目描述、输入格式、输出格式、子任务、提示以及其相关项使用 `\subsection*{}`；
3. 关于样例的说明：
    1. 样例存放在 data 文件夹中，请不要在题面中直接写入样例，而是在 data 中导入，在题面文件中引用；
    2. 样例请直接使用 `\examplebox{\lstinputlisting[frame=none]{输入文件.in}}{\lstinputlisting[frame=none]{输出文件.out}}` 进行添加。
    3. 对于带有样例说明的样例，请使用 `\examplebox*{}{}`，之后直接输入解释；
    4. 样例不需要另外创建 `subsection*`；
    5. 请不要导入过长的样例；如果实在需要导入，请使用超链接指向 data 内的文件；
    6. 以下是一个参考：
        ```latex
        \examplebox*{\lstinputlisting[frame=none]{data/24/3-1.in}}{\lstinputlisting[frame=none]{data/24/3-1.out}}
        ```
4. 关于图片的说明：
    1. 所有图片存放在 image 文件夹中，请在 image 文件夹中导入，在题面文件中引用；
    2. 虽然可能存在美观性问题，请禁止编译器对浮动题的调整；
    3. 为了统一，请将图片宽调整为整体的 0.95 倍；
    4. 以下是一个参考：
        ```latex
        \begin{figure}[H]
        \centering
        \includegraphics[width=0.95\textwidth]{image/24/3-p-3.png}
        \end{figure}
        ```
5. 关于表格的说明：
    1. 虽然可能存在美观性问题，请禁止编译器对浮动题的调整；
    2. 对齐格式参考原题面格式；

### 分析文件格式

1. 文件命名格式为 `考试场次-题目编号.tex`；
2. 对于每种思路，使用一个 `subsection`，格式为 `\subsection{$xx%$ 数据——思路概括}`；
3. 在每种思路 `subsection` 内部，至少有一个 `subsubsection` 名为 `思路`，用来讲解解题思路；
4. 在每种思路 `subsection` 内部，如果有题解，则可以用一个 `subsubsection` 存放，格式为 `\subsection{xx实现}`，其中 `xx` 指的是使用语言。
5. 所有题解代码存放在 code 文件夹中，请不要直接粘贴在分析文件中，而是通过 code 文件导入，导入格式为： 
    ```latex
    \lstinputlisting[language=xx]{code/场次/题解名称.xx}
    ```
6. 除去非常简单的模拟题，一定要有时间复杂度的分析；对于卡空间的题目，要有空间复杂度的分析；可以放在末尾（先有思路，后有验证），也可以放在开头（根据题目要求猜测复杂度，套出解法），如果在中间请尽量突出。