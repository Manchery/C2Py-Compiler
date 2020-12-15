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
    root.codeGen(*this); /* emit bytecode for the toplevel block */

    code << "if __name__ == '__main__':\n\tmain()" << std::endl;
}

std::string CodeGenContext::outputCode()
{
    return code.str();
}

/* -- Code Generation -- */

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

void NIdentifier::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating identifier reference: " << name << std::endl;
    context.code << name;
}

void NMethodCall::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating method call: " << id.name << std::endl;
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
    }

    rhs.codeGen(context);
    context.code << ")";
}

void NAssignment::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating assignment for " << lhs.name << std::endl;
    context.code << lhs.name << " = ";
    rhs.codeGen(context);
}

void NBlock::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating block" << std::endl;
    context.indent++;

    StatementList::const_iterator it;
    for (it = statements.begin(); it != statements.end(); it++)
    {
        std::cerr << "Generating code for " << typeid(**it).name() << std::endl;
        context.code << std::string(context.indent, '\t');
        (**it).codeGen(context);
    }

    context.indent--;
}

void NExpressionStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Generating code for " << typeid(expression).name() << std::endl;
    expression.codeGen(context);
    context.code << std::endl;
}

void NVariableDeclaration::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating variable declaration " << type << " " << id.name << std::endl;
    if (assignmentExpr != NULL)
    {
        NAssignment assn(id, *assignmentExpr);
        assn.codeGen(context);
    }
    else
    {
        context.code << id.name << " = None";
    }
    context.code << std::endl;
}

void NFunctionDeclaration::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating function: " << id.name << std::endl;

    context.code << "def " << id.name << "(";

    VariableList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++)
    {
        // (**it).codeGen(context);
        context.code << (**it).id.name;
    }
    context.code << "):" << std::endl;

    block.codeGen(context);

    context.code << std::endl
                 << std::endl;
}