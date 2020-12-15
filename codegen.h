#include <stack>
#include <sstream>

class NBlock;

// class CodeGenBlock
// {
// public:
//     BasicBlock *block;
//     std::map<std::string, Value *> locals;
// };

class CodeGenContext
{
    // std::stack<CodeGenBlock *> blocks;
    // Function *mainFunction;
    int indent;
    std::stringstream code;

public:
    // Module *module;
    CodeGenContext()
    {
        indent = -1;
        // module = new Module("main", getGlobalContext());
    }

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

    friend class Node;
    friend class NExpression;
    friend class NStatement;
    friend class NInteger;
    friend class NDouble;
    friend class NIdentifier;
    friend class NMethodCall;
    friend class NBinaryOperator;
    friend class NAssignment;
    friend class NBlock;
    friend class NExpressionStatement;
    friend class NVariableDeclaration;
    friend class NFunctionDeclaration;
};