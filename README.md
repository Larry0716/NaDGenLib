## 这是一个用于生成朴素数据的平凡的库

因为自己平时想颓废不想被抓，于是只好练练构造，顺带写一个库给大家玩。

## 这个库可以干什么？

如果你想快速生成一道题的基准数据，那么这个库可能是你的不二选择。

这个库没有什么别的优点，主要是其极高的自由度和简短的代码。

简短的代码？对，你没听错，如果你要是想生成一张抗 SPFA 的图，你可能只需要写寥寥几行代码，下面是一个示例：

```cpp
#include <bits/stdc++.h>
#include "genlib.h"
using namespace std;
using namespace Generator;
const int vmin = 114514;
const int vmax = 1919810;
void CustomFunction(){
    AntiSPFA atspfa(10,1e5/10);
    atspfa.AutoMode(vmin,vmax);
}
int main(){
    AutoGenerate(".\\data\\test%d.in",1,10,CustomFunction);
    return 0;
}
```

还有更多的功能，后面会慢慢道来。

## 你这库要是不保熟咋办？

不保熟？我直接让你成为维护之一，跟我一起乱搞！

是的，你没听错，如果你发现了当前库有什么 $\text{BUG}$，可以直接参与仓库的 $\text{Pull Request}$，不过 $\text{Pull Request}$ 的要求还是不少的，如果你无法满足以下要求中的几点的话，请不要提交 $\text{Pull Request}$（就算提交了我也给你驳回掉）：

1. 请尽量使代码风格基于 `Google` 编码规范，并在保证正确的前提下尽可能优秀。

2. 请写好 `Doxygen` 注释。

3. 如果感觉无法写好，请提交 $\text{Issue}$ 而不是 $\text{Pull Request}$。

4. 请在 $\text{Pull Request}$ 之前提交 $\text{Issue}$，然后再 $\text{Pull Request}$ 中引用你所提交的 $\text{Issue}$。

4. 请在保证正确的前提下尽可能的给予使用者高度的自由性，即可供其选择的项要更多。

如果不能保证上述内容，请遵循第三条，在提交 $\text{Issue}$ 的时候，请务必遵循以下原则（否则直接 $\text{close}$）：

1. 请将出现问题的源代码完整的展示出来。

2. 请保证自己所使用的库是最新版本，如果因为时间差导致而没有注意到这个问题的……（好像也可以 $\text{close}$ 了）。

3. 请将问题描述清楚，并给予构造时发生错误的数据。如果数据太大放不开，可以尝试将其扔到网盘上。

4. 请保证自己的代码没有任何问题。

5. 如果是请求添加功能的 $\text{Issue}$，请将要求描述清楚！

如果你想让我添加什么奇怪的功能的话，请提交 $\text{Issue}$。

如果你想拿我的库进行商用（会有这样的人吗？）……请遵循 $\text{MPLv2}$ 协议。

## 关于更多：

如果你想详细了解如何使用库的话，建议去我的博客或该仓库的 $\text{Wiki}$ 上查看。