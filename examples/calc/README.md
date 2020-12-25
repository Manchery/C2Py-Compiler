# 四则运算计算

```
.
├── autoValidation.sh   // 自动验证脚本
├── calc.c              // c 语言示例程序：四则运算计算
├── expGen.py           // 产生随机的表达式作为输入
├── groundTruth.py      // 用 python 中 eval 函数产生正确的输出
├── Makefile
└── README.md
```

该目录下用 C 实现了四则运算表达式求值，并验证了其正确性。该 C 程序作为我们的 C to Python 编译器的示例程序。

脚本 autoValidation.sh 验证方法如下：

- expGen.py 生成输入
- calc.c 和 groundTruth.py 分别输出
- 比对两者输出
- 一直循环直到出现输出不一致