#include "tokenizer.h"

#include <cctype>
#include <iostream>

using namespace std;

Tokenizer::Tokenizer(
    const string& input
)
    : input_(input),
      pos_(0) {
}

char Tokenizer::currentChar() {

    if(pos_ >= input_.size()) {
        return '\0';
    }

    return input_[pos_];
}

void Tokenizer::advance() {

    pos_++;
}

void Tokenizer::skipWhitespace() {

    while(isspace(currentChar())) {

        advance();
    }
}

Token Tokenizer::identifier() {

    string text;

    while(isalnum(currentChar())
          || currentChar() == '_') {

        text += currentChar();

        advance();
    }

    if(text == "SELECT") {
        return {TokenType::SELECT, text};
    }

    if(text == "INSERT") {
        return {TokenType::INSERT, text};
    }

    if(text == "INTO") {
        return {TokenType::INTO, text};
    }

    if(text == "VALUES") {
        return {TokenType::VALUES, text};
    }

    if(text == "FROM") {
        return {TokenType::FROM, text};
    }

    if(text == "WHERE") {
        return {TokenType::WHERE, text};
    }

    return {
        TokenType::IDENTIFIER,
        text
    };
}

Token Tokenizer::number() {

    string text;

    while(isdigit(currentChar())) {

        text += currentChar();

        advance();
    }

    return {
        TokenType::NUMBER,
        text
    };
}

Token Tokenizer::stringLiteral() {

    advance();

    string text;

    while(currentChar() != '\''
          && currentChar() != '\0') {

        text += currentChar();

        advance();
    }

    advance();

    return {
        TokenType::STRING,
        text
    };
}

vector<Token>
Tokenizer::tokenize() {

    vector<Token> tokens;

    while(currentChar() != '\0') {

        if(isspace(currentChar())) {

            skipWhitespace();

            continue;
        }

        if(isalpha(currentChar())
           || currentChar() == '_') {

            tokens.push_back(
                identifier()
            );

            continue;
        }

        if(isdigit(currentChar())) {

            tokens.push_back(
                number()
            );

            continue;
        }

        if(currentChar() == '\'') {

            tokens.push_back(
                stringLiteral()
            );

            continue;
        }

        switch(currentChar()) {

            case '*':

                tokens.push_back({
                    TokenType::STAR,
                    "*"
                });

                advance();

                break;

            case ',':

                tokens.push_back({
                    TokenType::COMMA,
                    ","
                });

                advance();

                break;

            case ';':

                tokens.push_back({
                    TokenType::SEMICOLON,
                    ";"
                });

                advance();

                break;

            case '=':

                tokens.push_back({
                    TokenType::EQUALS,
                    "="
                });

                advance();

                break;

            case '(':

                tokens.push_back({
                    TokenType::LEFT_PAREN,
                    "("
                });

                advance();

                break;

            case ')':

                tokens.push_back({
                    TokenType::RIGHT_PAREN,
                    ")"
                });

                advance();

                break;

            default:

                cout << "Unexpected char: "
                     << currentChar()
                     << endl;

                advance();

                break;
        }
    }

    tokens.push_back({
        TokenType::END_OF_FILE,
        ""
    });

    return tokens;
}