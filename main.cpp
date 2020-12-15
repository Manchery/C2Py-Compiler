#include <iostream>
#include "codegen.h"
#include "node.h"

#define YY_STDINIT

using namespace std;

extern int yyparse();
extern NBlock *programBlock;

int main(int argc, char **argv)
{
    freopen("in.txt", "r", stdin);
    freopen("out.py", "w", stdout);
    yyparse();
    std::cerr << programBlock << std::endl;

    CodeGenContext context;
    context.generateCode(*programBlock);

    std::string code = context.outputCode();
    std::cout << code << std::endl;

    fclose(stdin);
    fclose(stdout);
    return 0;
}