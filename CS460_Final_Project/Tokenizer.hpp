//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"

class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
  void printProcessedTokens();

private:
    Token lastToken;
    bool ungottenToken;
    bool parsingANewLine;
    std::ifstream &inStream;
    std::vector<Token> _tokens; 

private:
    std::string readString();
    std::string readName();
    std::string readIndent();
    int readInteger();

};

#endif //EXPRINTER_TOKENIZER_HPP
