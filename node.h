#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <set>
#include <string>
#include <algorithm>
#include <typeinfo>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement *> StatementList;
typedef std::vector<NExpression *> ExpressionList;
typedef std::vector<NVariableDeclaration *> VariableList;

extern int lineNumber;

enum ExprType
{
    UNKNOWN,
    INT,
    DOUBLE,
    CHAR,
    STR,
    ARRAY,
    VOID
};

class Node
{
public:
    int line;
    virtual ~Node() {}
    virtual void codeGen(CodeGenContext &context) {}
};

class NExpression : public Node
{
public:
    ExprType type;
};

class NStatement : public Node
{
};

class NInteger : public NExpression
{
public:
    long long value;
    NInteger(long long value) : value(value)
    {
        type = INT;
        line = lineNumber;
    }
    virtual void codeGen(CodeGenContext &context);
};

class NDouble : public NExpression
{
public:
    double value;
    NDouble(double value) : value(value)
    {
        type = DOUBLE;
        line = lineNumber;
    }
    virtual void codeGen(CodeGenContext &context);
};

class NString : public NExpression
{
public:
    std::string value;
    NString(const std::string &value) : value(value)
    {
        type = STR;
        line = lineNumber;
    }
    virtual void codeGen(CodeGenContext &context);
};

class NChar : public NExpression
{
public:
    char value;
    NChar(char value) : value(value)
    {
        type = CHAR;
        line = lineNumber;
    }
    virtual void codeGen(CodeGenContext &context);
};

class NIdentifier : public NExpression
{
public:
    std::string name;
    NIdentifier(const std::string &name) : name(name) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NMethodCall : public NExpression
{
public:
    const NIdentifier &id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier &id, ExpressionList &arguments) : id(id), arguments(arguments) { line = lineNumber; }
    NMethodCall(const NIdentifier &id) : id(id) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NBinaryOperator : public NExpression
{
public:
    int op;
    NExpression &lhs;
    NExpression &rhs;
    NBinaryOperator(NExpression &lhs, int op, NExpression &rhs) : lhs(lhs), rhs(rhs), op(op) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NArrayVariable : public NExpression
{
public:
    // TODO: check type & assignmentExpr.type
    NIdentifier &id;
    NExpression &indexExpr;
    NArrayVariable(NIdentifier &id, NExpression &indexExpr) : id(id), indexExpr(indexExpr) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NAssignment : public NExpression
{
public:
    bool isArrayVariable;
    NIdentifier &lhs;
    NExpression &rhs;
    NArrayVariable variable;
    NAssignment(NIdentifier &lhs, NExpression &rhs) : lhs(lhs), rhs(rhs), isArrayVariable(false), variable(lhs, rhs) { line = lineNumber; }
    NAssignment(NIdentifier &lhs, NExpression &rhs, NExpression &index) : lhs(lhs), rhs(rhs), isArrayVariable(true), variable(lhs, index) { line = lineNumber; }

    virtual void codeGen(CodeGenContext &context);
};

class NBlock : public NExpression
{
public:
    StatementList statements;
    NBlock() {}
    virtual void codeGen(CodeGenContext &context);
};

class NExpressionStatement : public NStatement
{
public:
    NExpression &expression;
    NExpressionStatement(NExpression &expression) : expression(expression) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NVariableDeclaration : public NStatement
{
public:
    // TODO: check type & assignmentExpr.type
    ExprType type;
    NIdentifier &id;
    NExpression *assignmentExpr;
    NVariableDeclaration(ExprType type, NIdentifier &id) : type(type), id(id), assignmentExpr(nullptr) { line = lineNumber; }
    NVariableDeclaration(ExprType type, NIdentifier &id, NExpression *assignmentExpr) : type(type), id(id), assignmentExpr(assignmentExpr) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NFunctionDeclaration : public NStatement
{
public:
    ExprType type;
    const NIdentifier &id;
    VariableList arguments;
    NBlock &block;
    NFunctionDeclaration(ExprType type, const NIdentifier &id,
                         const VariableList &arguments, NBlock &block) : type(type), id(id), arguments(arguments), block(block) { line = id.line; }
    virtual void codeGen(CodeGenContext &context);
};

class NIfStatement : public NStatement
{
public:
    NExpression &condition;
    NBlock *trueBlock;
    NBlock *falseBlock;
    NIfStatement(NExpression &condition,
                 NBlock *trueBlock,
                 NBlock *falseBlock = nullptr) : condition(condition), trueBlock(trueBlock), falseBlock(falseBlock) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NForStatement : public NStatement
{
public:
    NExpression *initializerExpr;
    NStatement *initializerDecl;
    NExpression *condition;
    NExpression *iterator;
    NBlock *block;
    NForStatement(NExpression *initializerExpr,
                  NExpression *condition,
                  NExpression *iterator,
                  NBlock *block) : initializerExpr(initializerExpr), initializerDecl(nullptr), condition(condition), iterator(iterator), block(block) { line = lineNumber; }
    NForStatement(NStatement *initializerDecl,
                  NExpression *condition,
                  NExpression *iterator,
                  NBlock *block) : initializerExpr(nullptr), initializerDecl(initializerDecl), condition(condition), iterator(iterator), block(block) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NWhileStatement : public NStatement
{
public:
    NExpression *condition;
    NBlock *block;
    NWhileStatement(NExpression *condition,
                    NBlock *block) : condition(condition), block(block) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NReturnStatement : public NStatement
{
public:
    NExpression *value;
    NReturnStatement(NExpression *value = nullptr) : value(value) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

class NArrayDeclaration : public NStatement
{
public:
    // TODO: check type & assignmentExpr.type
    ExprType type;
    NIdentifier &id;
    NExpression *lengthExpr;
    NArrayDeclaration(ExprType type, NIdentifier &id, NExpression *lengthExpr) : type(type), id(id), lengthExpr(lengthExpr) { line = lineNumber; }
    virtual void codeGen(CodeGenContext &context);
};

#endif