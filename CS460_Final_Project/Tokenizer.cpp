//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);

    parsingANewLine = false;

    return name;
}

std::string Tokenizer::readString(){
    std::string str;
    char c;
    while(inStream.get(c) && c != '"'){
        str +=c;
    }

    return str;
}

std::string Tokenizer::readIndent() {
    std::string indent;
    char c;
    while(inStream.get(c) && isspace(c))
        indent += c;

    parsingANewLine = false;

    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);

    return indent;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, parsingANewLine{true} {}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    Token token;
    char c;
    char b;

    while( inStream.get(c) && isspace(c) && c != '\n' && !parsingANewLine);


    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }


    if( inStream.eof()) {
        token.eof() = true;
    }else if( isspace(c) && c != '\n'){
        inStream.putback(c);
        token.setIndent(readIndent());
    }else if( c == '\n' ) {
        token.eol() = true;
        parsingANewLine = true;
    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );
    } else if( c == '=' ){
        inStream.get(b);
        if(b == '=')
            token.setRelOp(c,b);
        else{
            inStream.putback(b);
            token.symbol(c);
        }
    }else if(c == ';' || c == ',' || c == ':' || c == '.')
        token.symbol(c);
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if(c == '>'){
        inStream.get(b);
        if(b == '=')
            token.setRelOp(c,b);
        else{
            inStream.putback(b);
            token.symbol(c);
        }
    }
    else if(c == '<'){
        inStream.get(b);
        if(b == '=')
            token.setRelOp(c,b);
        else{
            inStream.putback(b);
            token.symbol(c);
        }
    }else if( c == '!'){
        inStream.get(b);
        if(b == '=')
            token.setRelOp(c,b);
        else{
            inStream.putback(b);
            token.symbol(c);
        }
    }
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if( c == '{' || c == '}')
        token.symbol(c);
    else if( c == '[' || c == ']')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        std::string name = readName();
        token.setName( name );
        if(name == "and" || name == "or" || name == "not")
            token.setBoolOp(name);

    }else if(c == '"'){
        token.setString(readString());
    }else {
        std::cout << "Unknown character in input. -> " << c << " <-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
