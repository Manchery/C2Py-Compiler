#include "node.h"
#include <stack>
#include <sstream>
#include <vector>

std::ostream &operator<<(std::ostream &os, ExprType type);

class NBlock;

// class CodeGenBlock
// {
// public:
//     BasicBlock *block;
//     std::map<std::string, Value *> locals;
// };

struct singleDeclaration
{
    public:
        std::string newName, oldName;
        ExprType type;
        NIdentifier *expValue;
};

class CodeGenContext
{
public:
    // std::stack<CodeGenBlock *> blocks;
    // Function *mainFunction;
    int indent;
    std::stringstream code;
    std::vector<std::vector<singleDeclaration>> stack;
    std::vector<std::string> errorMessage;
    std::vector<std::string> funcDeclaration;

public:
    // Module *module;
    CodeGenContext()
    {
        indent = -1;
        // module = new Module("main", getGlobalContext());
    }

    singleDeclaration* findDeclaration(std::string name, int type);
    int findLayerDeclaration(std::string name);

    void generateCode(NBlock &root);
    std::string outputCode();
    // GenericValue runCode();
    // std::map<std::string, Value *> &locals() { return blocks.top()->locals; }
    // BasicBlock *currentBlock() { return blocks.top()->block; }
    // void pushBlock(BasicBlock *block)
    // {
    //     blocks.push(new CodeGenBlock());
    //     blocks.top()->block = block;
    // }
    // void popBlock()
    // {
    //     CodeGenBlock *top = blocks.top();
    //     blocks.pop();
    //     delete top;
    // }
};