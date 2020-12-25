#include "node.h"
#include <stack>
#include <sstream>
#include <vector>

std::ostream &operator<<(std::ostream &os, ExprType type);

class NBlock;

// singleDeclaration是符号表（作用域栈帧）中的一个条目
struct singleDeclaration
{
    public:
        std::string newName, oldName;
        ExprType type;
        NIdentifier *expValue;
};

// CodeGenContext保存着整个编译过程中所需的全局变量
class CodeGenContext
{
public:
    // 缩进
    int indent;

    // 输出代码缓冲区
    std::stringstream code;

    // 作用域栈（符号表）
    std::vector<std::vector<singleDeclaration>> stack;
    
    // 错误信息
    std::vector<std::string> errorMessage;

    // 函数符号表
    std::vector<std::string> funcDeclaration;

public:
    CodeGenContext()
    {
        indent = -1;
    }
    
    // 在作用域栈中查找变量的定义
    singleDeclaration* findDeclaration(std::string name, int type);
    int findLayerDeclaration(std::string name);

    // 代码生成与输出
    void generateCode(NBlock &root);
    std::string outputCode();
};