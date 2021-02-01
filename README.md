# C to Python Translator

Jialong Wu, Yishujie Zhao and Shuwei Huang

## Introduction

A C to Python translator 🤖 based on Lex/Yacc.

Course project for Principles of Assembly and Compilation, instructed by [Prof. Wang](http://ise.thss.tsinghua.edu.cn/~wangchaokun/).

For details, see our report and slides (in Chinese).

## Features 🔥

### Basic Syntax of C

Support:

- Varible and literal
- Arithmetic and logical expression
- Conditional and loop statements
- One-dimensional array
- Function

Not support:

- Pointer and reference
- `continue` and `break`
- `do-while` statement
- Conditions like `if (a)` (must be `if (a!=0)`) 

### Library Functions

Standard library functions, including:

- `scanf`
- `printf`
- `atoi`
- `strlen`

Their Python equivalents are implemented in `utils.py`.

### Semantic Error Handling

Detect and report semantic errors, including:

- Redefinition of variables, arrays and functions
- Undefined identifiers

All semantic errors will be accumulated and reported in the end of compilation.

### Scope check and renaming for variables

We maintain a symbol table in order to do scope check and renaming of variables. For details, have a try to compile `examples/test.c`.

## Environment

Ubuntu 16.04+ with Lex, Yacc, and Python 3.7 
## Usage

```
make clean
make
./parser [Input C File]
python out.py
```

The default input file is `in.c` and results will be written to `out.py`. 

## Examples

There are more examples in `src/examples`:

- `calc/calc.c`: Expression evaluation
- `aplusb.c`: A+B Problem & HelloWorld
- `factorial.c`: Solving factorial
- `palindrome.c`: Palindrome Validation
- `special.c`: Example for semantic errors
- `test.c`: Example for variables renaming

For details, please refer to our report.

## References

- [Write text parsers with yacc and lex – IBM Developer](https://developer.ibm.com/technologies/systems/tutorials/au-lexyacc/)
- [Writing Your Own Toy Compiler Using Flex, Bison and LLVM (gnuu.org)](https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/)
- [re — Regular expression operations — Python 3.9.1 documentation](https://docs.python.org/3/library/re.html#simulating-scanf)