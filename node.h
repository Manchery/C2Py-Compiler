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

enum ExprType
{
    UNKNOWN,
    INT,
    DOUBLE,
    CHAR,
    STR,
    VOID
};

class Node
{
public:
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
    NInteger(long long value) : value(value) { type = INT; }
    virtual void codeGen(CodeGenContext &context);
};

class NDouble : public NExpression
{
public:
    double value;
    NDouble(double value) : value(value) { type = DOUBLE; }
    virtual void codeGen(CodeGenContext &context);
};

class NIdentifier : public NExpression
{
public:
    std::string name;
    NIdentifier(const std::string &name) : name(name) {}
    virtual void codeGen(CodeGenContext &context);
};

class NMethodCall : public NExpression
{
public:
    const NIdentifier &id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier &id, ExpressionList &arguments) : id(id), arguments(arguments) {}
    NMethodCall(const NIdentifier &id) : id(id) {}
    virtual void codeGen(CodeGenContext &context);
};

class NBinaryOperator : public NExpression
{
public:
    int op;
    NExpression &lhs;
    NExpression &rhs;
    NBinaryOperator(NExpression &lhs, int op, NExpression &rhs) : lhs(lhs), rhs(rhs), op(op) {}
    virtual void codeGen(CodeGenContext &context);
};

class NAssignment : public NExpression
{
public:
    NIdentifier &lhs;
    NExpression &rhs;
    NAssignment(NIdentifier &lhs, NExpression &rhs) : lhs(lhs), rhs(rhs) {}
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
    NExpressionStatement(NExpression &expression) : expression(expression) {}
    virtual void codeGen(CodeGenContext &context);
};

class NVariableDeclaration : public NStatement
{
public:
    // TODO: check type & assignmentExpr.type
    ExprType type;
    NIdentifier &id;
    NExpression *assignmentExpr;
    NVariableDeclaration(ExprType type, NIdentifier &id) : type(type), id(id) {}
    NVariableDeclaration(ExprType type, NIdentifier &id, NExpression *assignmentExpr) : type(type), id(id), assignmentExpr(assignmentExpr) {}
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
                         const VariableList &arguments, NBlock &block) : type(type), id(id), arguments(arguments), block(block) {}
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
                 NBlock *falseBlock = nullptr) : condition(condition), trueBlock(trueBlock), falseBlock(falseBlock) {}
    virtual void codeGen(CodeGenContext &context);
};

#endif