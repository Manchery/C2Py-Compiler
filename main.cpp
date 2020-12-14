#include <iostream>
#include "codegen.h"
#include "node.h"

#define YY_STDINIT

using namespace std;

extern int yyparse();
extern NBlock* programBlock;

int main(int argc, char **argv)
{   
    freopen("in.txt", "r", stdin);
    std::cout << "yyparse start" << std::endl;
    yyparse();
    std::cout << "yyparse done" << std::endl;
    std::cout << programBlock << std::endl;

    CodeGenContext context;
    context.generateCode(*programBlock);
    // context.runCode();
    
    return 0;
}