#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class ExpressionNode
    : public ASTNode {
};

class IdentifierNode
    : public ExpressionNode {

public:
    string name;

    explicit IdentifierNode(
        const string& n
    );
};

class NumberNode
    : public ExpressionNode {

public:
    int value;

    explicit NumberNode(int v);
};

class StringNode
    : public ExpressionNode {

public:
    string value;

    explicit StringNode(
        const string& v
    );
};

class BinaryExpressionNode
    : public ExpressionNode {

public:
    string op;

    shared_ptr<ExpressionNode>
        left;

    shared_ptr<ExpressionNode>
        right;

    BinaryExpressionNode(
        const string& operation,
        shared_ptr<ExpressionNode> lhs,
        shared_ptr<ExpressionNode> rhs
    );
};

class SelectStatementNode
    : public ASTNode {

public:
    vector<string> columns;

    string tableName;

    shared_ptr<ExpressionNode>
        whereClause;
};

class InsertStatementNode
    : public ASTNode {

public:
    string tableName;

    vector<string> values;
};