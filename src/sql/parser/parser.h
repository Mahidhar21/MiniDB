#pragma once

#include "../ast/ast.h"
#include "../tokenizer/tokenizer.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens_;

    size_t pos_;

    Token currentToken();

    void advance();

    bool match(TokenType type);

    shared_ptr<ExpressionNode>
    parseExpression();

    shared_ptr<SelectStatementNode>
    parseSelect();

    shared_ptr<InsertStatementNode>
    parseInsert();

public:
    explicit Parser(
        const vector<Token>& tokens
    );

    shared_ptr<ASTNode> parse();
};