#include "ast.h"

using namespace std;

IdentifierNode::IdentifierNode(
    const string& n
)
    : name(n) {
}

NumberNode::NumberNode(int v)
    : value(v) {
}

StringNode::StringNode(
    const string& v
)
    : value(v) {
}

BinaryExpressionNode::
BinaryExpressionNode(
    const string& operation,
    shared_ptr<ExpressionNode> lhs,
    shared_ptr<ExpressionNode> rhs
)
    : op(operation),
      left(lhs),
      right(rhs) {
}