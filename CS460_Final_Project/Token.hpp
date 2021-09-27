//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>
#include <vector>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isKeyword();

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
    bool isOpenBrace() const  { return _symbol == '{';}
    bool isCloseBrace() const { return _symbol == '}';}
    bool isColon() const      { return _symbol == ':';}
    bool isComma() const      { return _symbol == ',';}
    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }


    bool isAssignmentOperator() const     { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isSemiColonOperator() const      { return _symbol == ';'; }
    bool isGreaterThanOperator() const    { return _symbol == '>'; }
    bool isLessThanOperator() const       { return _symbol == '<'; }
    bool isOpenBracket() const            { return _symbol == '[';}
    bool isCloseBracket() const           { return _symbol == ']';}
    bool isDotOperator() const            { return _symbol == '.';}

    bool isEqualOperator() const          { return _relOp.length() > 0 && _relOp == "=="; }
    bool isGreaterEqualOperator() const   { return _relOp.length() > 0 && _relOp == ">="; }
    bool isLessThanEqualOperator() const  { return _relOp.length() > 0 && _relOp == "<="; }
    bool isNotEqualOperator() const       { return _relOp.length() > 0 && _relOp == "!="; }

    bool isAndOperator() const            { return _name.length() > 0 && _boolOp == "and"; }
    bool isNotOperator() const            { return _name.length() > 0 && _boolOp == "not"; }
    bool isOrOperator() const             { return _name.length() > 0 && _boolOp == "or"; }
    bool isAppend() const                 { return _name.length() > 0 && _name == "append";}
    bool isPop() const                    { return _name.length() > 0 && _name == "pop";}
    bool isLen() const                    { return _name.length() > 0 && _name == "len";}
    bool isReturn() const                 { return _name.length() > 0 && _name == "return";}

    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const           { return _name; }

    bool isFor()const                     { return isName() && _name == "for";}
    bool isPrint()const                   { return isName() && _name == "print";}
    bool isRange()const                   { return isName() && _name == "range";}
    bool isIn() const                     { return isName() && _name == "in";}
    bool isIf() const                     { return isName() && _name == "if";}
    bool isDef() const                    { return isName() && _name == "def";}


    bool isRelationalOperator() const {
        return  isGreaterThanOperator() ||
                isLessThanOperator() ||
                isEqualOperator() ||
                isGreaterEqualOperator() ||
                isLessThanEqualOperator() ||
                isNotEqualOperator();
    }

    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }

    bool isBooleanOperator() const {
        return isAndOperator() ||
                isOrOperator() ||
                isNotOperator();
    }



    bool isString() const                 { return _string.length() > 0; }
    std::string getString() const         { return _string; }

    bool isIndent() const                 {return _indent.length() > 0;}
    int getIndent() const                 {return _indent.length();}


    void setName(std::string n)           { _name = n; }
    void setString(std::string n)         { _string = n; }
    void setIndent(std::string n)         { _indent = n; }

    void setRelOp(char c, char b){
        _relOp += c;
        _relOp += b;
    }

    std::string getRelOp(){

        if(_symbol){
            std::string chr;
            chr+=_symbol;
            return chr;
        }

        return _relOp;
    }
    void setBoolOp(std::string name){
        _boolOp = name;
    }
    std::string getBoolOp(){
        return _boolOp;
    }
    bool &isWholeNumber()       { return _isWholeNumber; }
    bool isWholeNumber() const  { return _isWholeNumber; }
    int getWholeNumber() const  { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    void print() const;

private:
    std::string _indent;
    std::string _string;
    std::string _name;
    std::string _relOp;
    std::string _boolOp;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    int _wholeNumber;
    std::vector<std::string> _keyWords;
};

#endif //EXPRINTER_TOKEN_HPP
