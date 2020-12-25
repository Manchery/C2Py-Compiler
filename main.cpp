#include <iostream>
#include "codegen.h"
#include "node.h"

#define YY_STDINIT

using namespace std;

extern int yyparse();
extern NBlock *programBlock;

int main(int argc, char **argv)
{
    // 设定输入输出文件
    if (argc >= 2)
    {
        freopen(argv[1], "r", stdin);
    }
    else
    {
        freopen("in.c", "r", stdin);
    }
    freopen("out.py", "w", stdout);

    // 词法、语法解析
    yyparse();
    std::cerr << programBlock << std::endl;

    // 对AST遍历输出目标代码
    CodeGenContext context;
    context.generateCode(*programBlock);

    // 输出编译结果
    std::cerr << "------\n\n" << std::endl;

    if (context.errorMessage.size() != 0)
    {
        std::cerr << "Error occurred while compiling, stopping..." << std::endl;
        for (auto msg : context.errorMessage)
        {
            std::cerr << msg << std::endl;
        }
        fclose(stdin);
        fclose(stdout);
        return -1;
    }
    

    std::cerr << "Source code is compiled successfully." << std::endl;
    std::string code = context.outputCode();
    std::cout << code << std::endl;

    fclose(stdin);
    fclose(stdout);
    return 0;
}