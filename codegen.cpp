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

void NIdentifier::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating identifier reference: " << name << std::endl;
    context.code << name;
}

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
    context.code << lhs.name << " = ";
    rhs.codeGen(context);
}

void NBlock::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating block" << std::endl;
    context.indent++;
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

void NVariableDeclaration::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating variable declaration " << type << " " << id.name << std::endl;
    if (assignmentExpr != nullptr)
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

void NForStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating for statement" << std::endl;

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
}

void NWhileStatement::codeGen(CodeGenContext &context)
{
    std::cerr << "Creating while statement" << std::endl;

    context.code << "while ";
    condition->codeGen(context);
    context.code << ":" << std::endl;

    block->codeGen(context);
}