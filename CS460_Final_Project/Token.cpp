//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <algorithm>
#include "Token.hpp"

Token::Token(): _relOp(""), _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false} {
    _keyWords.emplace_back("for");
    _keyWords.emplace_back("print");
    _keyWords.emplace_back("if");
    _keyWords.emplace_back("in");
    _keyWords.emplace_back("and");
    _keyWords.emplace_back("not");
    _keyWords.emplace_back("or");
    _keyWords.emplace_back("def");
    _keyWords.emplace_back("return");

}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isGreaterThanOperator()) std::cout << " > ";
    else if( isLessThanOperator()) std::cout << " < ";
    else if( isGreaterEqualOperator()) std::cout << " >= ";
    else if( isLessThanEqualOperator()) std::cout << " <= ";
    else if( isSemiColonOperator()) std::cout <<" ; ";
    else if( isEqualOperator()) std::cout << " == ";
    else if( isNotEqualOperator()) std::cout << " != ";
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if (isDotOperator()) std::cout << ".";
    else if( isName() )  std::cout << getName();
    else if( isString() ) std::cout << getString();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isCloseBrace()) std::cout << "}";
    else if( isOpenBrace()) std::cout << "{";
    else if( isOpenBracket()) std::cout << '[';
    else if( isCloseBracket()) std::cout << ']';
    else if( isColon()) std::cout << ":";
    else if( isComma()) std::cout << ",";
    else if( isOrOperator()) std::cout << "or";
    else if( isNotOperator()) std::cout << "not";
    else if( isAndOperator()) std::cout << "and";
    else if( isAppend()) std::cout << "append";
    else if( isPop()) std::cout << "pop";
    else if( isLen()) std::cout << "len";
    else if( isDef()) std::cout << "def";
    else if( isReturn()) std::cout << "return";
    else if( isIndent()) std:: cout << "INDENT "<< _indent.length();
    else std::cout << "Uninitialized token.\n";
}


bool Token::isKeyword() {
    return std::find(_keyWords.begin(),_keyWords.end(),_name) != _keyWords.end();
}