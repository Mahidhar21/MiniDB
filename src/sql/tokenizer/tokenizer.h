#pragma once

#include <string>
#include <vector>

using namespace std;

enum class TokenType {

    SELECT,
    INSERT,
    INTO,
    VALUES,
    FROM,
    WHERE,

    IDENTIFIER,

    NUMBER,

    STRING,

    STAR,

    COMMA,

    SEMICOLON,

    EQUALS,

    LEFT_PAREN,

    RIGHT_PAREN,

    END_OF_FILE
};

struct Token {

    TokenType type;

    string text;
};

class Tokenizer {
private:
    string input_;

    size_t pos_;

    char currentChar();

    void advance();

    void skipWhitespace();

    Token identifier();

    Token number();

    Token stringLiteral();

public:
    explicit Tokenizer(
        const string& input
    );

    vector<Token> tokenize();
};