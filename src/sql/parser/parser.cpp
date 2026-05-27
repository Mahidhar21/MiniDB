#include "parser.h"

using namespace std;

Parser::Parser(
    const vector<Token>& tokens
)
    : tokens_(tokens),
      pos_(0) {
}

Token Parser::currentToken() {

    if(pos_ >= tokens_.size()) {

        return {
            TokenType::END_OF_FILE,
            ""
        };
    }

    return tokens_[pos_];
}

void Parser::advance() {

    if(pos_ < tokens_.size()) {
        pos_++;
    }
}

bool Parser::match(
    TokenType type
) {

    if(currentToken().type
       == type) {

        advance();

        return true;
    }

    return false;
}

shared_ptr<ExpressionNode>
Parser::parseExpression() {

    Token left =
        currentToken();

    advance();

    Token op =
        currentToken();

    advance();

    Token right =
        currentToken();

    advance();

    auto leftNode =
        make_shared<IdentifierNode>(
            left.text
        );

    auto rightNode =
        make_shared<NumberNode>(
            stoi(right.text)
        );

    return make_shared
    <
        BinaryExpressionNode
    >(
        op.text,
        leftNode,
        rightNode
    );
}

shared_ptr<SelectStatementNode>
Parser::parseSelect() {

    auto select =
        make_shared
        <
            SelectStatementNode
        >();

    match(TokenType::SELECT);

    if(match(TokenType::STAR)) {

        select->columns.push_back(
            "*"
        );
    }

    match(TokenType::FROM);

    select->tableName =
        currentToken().text;

    match(TokenType::IDENTIFIER);

    if(match(TokenType::WHERE)) {

        select->whereClause =
            parseExpression();
    }

    return select;
}

shared_ptr<InsertStatementNode>
Parser::parseInsert() {

    auto insert =
        make_shared
        <
            InsertStatementNode
        >();

    match(TokenType::INSERT);

    match(TokenType::INTO);

    insert->tableName =
        currentToken().text;

    match(TokenType::IDENTIFIER);

    match(TokenType::VALUES);

    match(TokenType::LEFT_PAREN);

    while(currentToken().type
          != TokenType::RIGHT_PAREN) {

        insert->values.push_back(
            currentToken().text
        );

        advance();

        if(currentToken().type
           == TokenType::COMMA) {

            advance();
        }
    }

    match(TokenType::RIGHT_PAREN);

    return insert;
}

shared_ptr<ASTNode>
Parser::parse() {

    if(currentToken().type
       == TokenType::SELECT) {

        return parseSelect();
    }

    if(currentToken().type
       == TokenType::INSERT) {

        return parseInsert();
    }

    return nullptr;
}