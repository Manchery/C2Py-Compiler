#include "node.h"
#include "codegen.h"
#include "parser.hpp"

using namespace std;

std::ostream &operator<<(std::ostream &os, ExprType type)
{
    if (type == INT)
        os << "INT";
    if (type == DOUBLE)
        os << "DOUBLE";
    if (type == CHAR)
        os << "CHAR";
    if (type == STR)
        os << "STR";
    if (type == VOID)
        os << "VOID";
    return os;
}

/* Compile the AST into a module */
void CodeGenContext::generateCode(NBlock &root)
{
    std::cerr << "Generating code...n" << std::endl;
    code << "from utils import *" << std::endl
         << std::endl;

    root.codeGen(*this); /* emit bytecode for the toplevel block */

    code << "if __name__ == '__main__':\n\tmain()" << std::endl;
}

std::string CodeGenContext::outputCode()
{
    return code.str();
}

/* -- Code Generation -- */
// 以下代码中，每个函数用于生成不同类型结点的代码
// 通常，其规则为根据AST中获得的信息，输出符合Python语法要求的代码
// 对于需要特殊处理的函数，将在其定义部分给出详细注释
void NInteger::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating integer: " << value << std::endl;
    context.code << value;
}

void NDouble::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating double: " << value << std::endl;
    context.code << value;
}

void NString::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating string: " << value << std::endl;

    context.code << "[";
    for (int i = 0; i < value.length(); i++)
    {
        if (i > 0)
            context.code << ", ";
        context.code << "'" << value[i] << "'";
    }
    context.code << "]";
}

void NChar::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating char: " << value << std::endl;

    context.code << "'" << value << "'";
}

void NIdentifier::codeGen(CodeGenContext &context)
{
    auto oldDeclaration = context.findDeclaration(name, 1);
    if (oldDeclaration == nullptr)
    {
        context.errorMessage.push_back("Line " + std::to_string(line) + ": Identifier \"" + name + "\" is undefined");
    }
    else
    {
        context.code << oldDeclaration->newName;
    }
}

// 为代码调用生成函数调用
// 由于Python不支持printf, scanf, atoi和strlen等函数，因此需要特判
// 其余由用户自行定义的函数则按照一般语法规则生成即可
void NMethodCall::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating method call: " << id.name << ", " << arguments.size() << " arguments" << std::endl;

    if (id.name == "printf")
    {
        context.code << "printf("
                     << "\"" << (dynamic_cast<NString *>(arguments[0]))->value << "\""
                     << ", (";
        ExpressionList::const_iterator it = arguments.begin();
        bool first = true;
        for (it++; it != arguments.end(); it++)
        {
            if (!first)
                context.code << ", ";
            first = false;
            (**it).codeGen(context);
        }
        context.code << "))";
        return;
    }

    if (id.name == "scanf")
    {
        context.code << "[";
        ExpressionList::const_iterator it = arguments.begin();
        bool first = true;
        for (it++; it != arguments.end(); it++)
        {
            if (!first)
                context.code << ", ";
            first = false;
            (**it).codeGen(context);
        }
        context.code << "] = ";
        context.code << "scanf("
                     << "\"" << (dynamic_cast<NString *>(arguments[0]))->value << "\""
                     << ")";
        return;
    }
    if ((id.name != "atoi") && (id.name != "strlen"))
    {
        int bz = 1;
        for (int i = 0; i < context.funcDeclaration.size(); i++)
        {
            if (context.funcDeclaration[i] == id.name)
            {
                bz = 0;
                break;
            }
        }
        if (bz)
        {
            context.errorMessage.push_back("Line " + std::to_string(line) + ": Function \"" + id.name + "\" is undefined");
            return;
        }
    }
    context.code << id.name << "(";
    ExpressionList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++)
    {
        if (it != arguments.begin())
            context.code << ", ";
        (**it).codeGen(context);
    }
    context.code << ")";
}

void NBinaryOperator::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating binary operation " << op << std::endl;
    context.code << "(";
    lhs.codeGen(context);

    switch (op)
    {
    case TPLUS:
        context.code << " + ";
        break;
    case TMINUS:
        context.code << " - ";
        break;
    case TMUL:
        context.code << " * ";
        break;
    case TDIV:
        context.code << " / ";
        break;
    case TCEQ:
        context.code << " == ";
        break;
    case TCNE:
        context.code << " != ";
        break;
    case TCLT:
        context.code << " < ";
        break;
    case TCLE:
        context.code << " <= ";
        break;
    case TCGT:
        context.code << " > ";
        break;
    case TCGE:
        context.code << " >= ";
        break;
    case TAND:
        context.code << " and ";
        break;
    case TOR:
        context.code << " or ";
        break;
    }

    rhs.codeGen(context);
    context.code << ")";
}

void NAssignment::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating assignment for " << lhs.name << std::endl;
    if (this->isArrayVariable)
    {
        variable.codeGen(context);
        context.code << " = ";
    }
    else
    {
        lhs.codeGen(context);
        context.code << " = ";
    }
    rhs.codeGen(context);
}

// 代码块的生成
// 每生成一个代码块，就需要维护缩进、作用域栈帧、符号表等信息
void NBlock::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating block" << std::endl;
    context.indent++;
    context.stack.push_back(std::vector<singleDeclaration>());
    std::cerr << "Current indents: " << context.indent << std::endl;

    StatementList::const_iterator it;
    for (it = statements.begin(); it != statements.end(); it++)
    {
        std::cerr << "Generating code for " << typeid(**it).name() << std::endl;
        context.code << std::string(context.indent, '\t');
        (**it).codeGen(context);
    }

    if (statements.size() == 0)
    {
        context.code << std::string(context.indent, '\t') << "pass" << std::endl;
    }

    context.stack.pop_back();
    std::cerr << "Leaving block" << std::endl;
    context.indent--;
    std::cerr << "Current indents: " << context.indent << std::endl;
}

void NExpressionStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Generating code for " << typeid(expression).name() << std::endl;
    expression.codeGen(context);
    context.code << std::endl;
}

// 查找同作用域中的变量声明
int CodeGenContext::findLayerDeclaration(std::string name)
{
    auto &layer = stack[stack.size() - 1];
    for (int j = layer.size() - 1; j >= 0; j--)
    {
        auto *decl = &layer[j];
        if (decl->oldName == name)
        {
            return 1;
        }
    }
    return 0;
}

// 在整个作用域栈中查找变量声明
singleDeclaration *CodeGenContext::findDeclaration(std::string name, int type)
{
    for (int i = stack.size() - type; i >= 0; i--)
    {
        auto &layer = stack[i];
        for (int j = layer.size() - 1; j >= 0; j--)
        {
            auto *decl = &layer[j];
            if ((type == 1 ? decl->oldName : decl->newName) == name)
            {
                return decl;
            }
        }
    }
    return nullptr;
}

// 变量声明时的重命名检测与处理
void NVariableDeclaration::codeGen(CodeGenContext &context)
{
    if (context.findLayerDeclaration(id.name))
    {
        context.errorMessage.push_back("Line " + std::to_string(line) + ": Redefinition of identifier \"" + id.name + "\"");
        return;
    }
    auto &layer = context.stack[context.stack.size() - 1];
    auto oldDeclaration = context.findDeclaration(id.name, 2);
    std::string newName = id.name;
    if (oldDeclaration != nullptr)
    {
        newName = id.name + '_' + std::to_string(context.stack.size() - 1);
    }
    layer.push_back(singleDeclaration{newName, id.name, id.type, &id});
    std::cerr << "Creating variable declaration " << type << " " << id.name << std::endl;
    if (assignmentExpr != nullptr)
    {
        NAssignment assn(id, *assignmentExpr);
        assn.codeGen(context);
    }
    else
    {
        context.code << newName << " = None";
    }
    context.code << std::endl;
}

// 函数声明时的重命名检测与处理
void NFunctionDeclaration::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating function: " << id.name << std::endl;

    for (int i = 0; i < context.funcDeclaration.size(); i++)
    {
        if (context.funcDeclaration[i] == id.name)
        {
            context.errorMessage.push_back("Line " + std::to_string(line) + ": Redefinition of function \"" + id.name + "\"");
            return;
        }
    }
    context.funcDeclaration.push_back(id.name);
    context.code << "def " << id.name << "(";

    context.stack.push_back(std::vector<singleDeclaration>());
    VariableList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++)
    {
        // (**it).codeGen(context);
        auto &layer = context.stack[context.stack.size() - 1];
        auto oldDeclaration = context.findDeclaration((**it).id.name, 2);
        std::string newName = (**it).id.name;
        if (oldDeclaration != nullptr)
        {
            newName = (**it).id.name + '_' + std::to_string(context.stack.size() - 1);
        }
        layer.push_back(singleDeclaration{newName, (**it).id.name, (**it).id.type, &(**it).id});

        if (it != arguments.begin())
            context.code << ", ";
        context.code << newName;
    }
    context.code << "):" << std::endl;

    auto &firstLayer = context.stack[0];
    if (firstLayer.size())
    {
        context.code << "\tglobal ";
        for (int i = 0; i < firstLayer.size(); i++)
        {
            if (i)
                context.code << ", ";
            context.code << firstLayer[i].oldName;
        }
        context.code << std::endl;
    }

    block.codeGen(context);

    context.stack.pop_back();

    context.code << std::endl;
}

void NIfStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating if statement" << std::endl;

    context.code << "if ";
    condition.codeGen(context);
    context.code << ":" << std::endl;

    trueBlock->codeGen(context);

    if (falseBlock != nullptr)
    {
        context.code << std::string(context.indent, '\t') << "else:" << std::endl;
        falseBlock->codeGen(context);
    }
}

// For循环的参数处理与函数参数类似，需要解决重命名问题
void NForStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating for statement" << std::endl;

    context.stack.push_back(std::vector<singleDeclaration>());
    if (initializerExpr != nullptr)
        initializerExpr->codeGen(context);
    if (initializerDecl != nullptr)
        initializerDecl->codeGen(context);

    context.code << std::string(context.indent, '\t');
    context.code << "while ";
    condition->codeGen(context);
    context.code << ":" << std::endl;

    block->codeGen(context);

    context.indent++;
    context.code << std::string(context.indent, '\t');
    iterator->codeGen(context);
    context.code << std::endl;
    context.indent--;
    context.stack.pop_back();
}

void NWhileStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating while statement" << std::endl;

    context.code << "while ";
    condition->codeGen(context);
    context.code << ":" << std::endl;

    block->codeGen(context);
}

void NReturnStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating return statement" << std::endl;

    context.code << "return ";

    if (value)
    {
        value->codeGen(context);
    }

    context.code << std::endl;
}

// 数组声明，需要单独维护其符号
void NArrayDeclaration::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating array statement: " << id.name << std::endl;

    auto &layer = context.stack[context.stack.size() - 1];
    if (context.findLayerDeclaration(id.name))
    {
        context.errorMessage.push_back("Line " + std::to_string(line) + ": Redefinition of identifier \"" + id.name + "\"");
        return;
    }
    auto oldDeclaration = context.findDeclaration(id.name, 2);
    std::string newName = id.name;
    if (oldDeclaration != nullptr)
    {
        newName = id.name + '_' + std::to_string(context.indent);
    }
    layer.push_back(singleDeclaration{newName, id.name, id.type, &id});

    context.code << newName << " = [None] * ";

    lengthExpr->codeGen(context);

    context.code << std::endl;
}

// 引用数组变量也需要单独处理
void NArrayVariable::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating array variable" << std::endl;

    // id.type = ARRAY;
    id.codeGen(context);

    context.code << "[";

    indexExpr.codeGen(context);

    context.code << "]";
}