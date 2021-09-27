
#ifndef EXPRINTER_ARITHEXPR_HPP
#define EXPRINTER_ARITHEXPR_HPP

#include "Token.hpp"
#include "SymTab.hpp"
class SymTab;
class TypeDescriptor;
class IntegerTypeDescriptor;
class StringTypeDescriptor;
class ArrayTypeDescriptor;
class FunctionTypeDescriptor;

class ExprNode {
public:
    ExprNode(Token token);
    Token token();
    virtual void print() = 0;
    virtual TypeDescriptor *evaluate(SymTab &symTab) = 0;
    virtual ~ExprNode() = default;
private:
    Token _token;
};

// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode: public ExprNode {  // An expression tree node.

public:
    InfixExprNode(Token tk);

    ExprNode *&left();
    ExprNode *&right();
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
    ~InfixExprNode(){
        delete(_left);
        delete(_right);
    }
private:
    ExprNode *_left, *_right;
};


class RelExprNode: public ExprNode{
public:
    RelExprNode(Token tk);

    ExprNode *&left();
    ExprNode *&right();

    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &);
    ~RelExprNode(){
        delete(_left);
        delete(_right);
    }
private:
    ExprNode *_left, *_right;

};

class BoolExprNode: public ExprNode{
public:
    BoolExprNode(Token tk);

    ExprNode *&left();
    ExprNode *&right();

    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &);
    ~BoolExprNode(){
        delete(_left);
        delete(_right);
    }
private:
    ExprNode *_left, *_right;

};
// WholeNumber is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class WholeNumber: public ExprNode {
public:
    WholeNumber(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};

// Variable is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class Variable: public ExprNode {
public:
    Variable(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};

class String: public ExprNode{
public:
    String(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};


class TestList{
public:
    TestList();
    void addExpression(ExprNode*);
    void evaluate(SymTab &symTab);\
    void appendList(ExprNode*);
    ExprNode *getValueAt(int);
    void popList(int);

    std::vector<ExprNode*> *getList(){ return _testlist;}
    void print();
    ~TestList(){
        delete(_testlist);
    }
private:
    std::vector<ExprNode*> *_testlist;
};

class ParameterList{
public:
    ParameterList();
    void addExpression(Token);
    //void evaluate(SymTab &symTab);
    std::vector<Token> *getParameters(){ return _parameters;}
    void print();
    ~ParameterList(){
        delete(_parameters);
    }
private:
    std::vector<Token> *_parameters;
};
class Array: public  ExprNode {
public:
    Array(TestList*);

    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
    ~Array(){
        delete(_testList);
    }

private:
    TestList *_testList;
};

class lenExpr :public ExprNode{
public:
    lenExpr(Token);
    virtual void print();
    TypeDescriptor *evaluate(SymTab &symTab);
private:
    Token _id;
};
class CallExpr: public ExprNode{
public:
    CallExpr(Token token);
    CallExpr(Token, TestList*);
    virtual void print();
    TypeDescriptor *evaluate(SymTab &symTab);
    ~CallExpr(){
        delete(_tstList);
    }
private:
    Token _functionId;
    TestList *_tstList;
};
class SubExpr: public ExprNode{
public:
    SubExpr(Token token);
    SubExpr(Token, ExprNode*);
    virtual void print();
    TypeDescriptor *evaluate(SymTab &symTab);
    ~SubExpr(){
        delete(_test);
    }
private:
    Token _arrayId;
    ExprNode *_test;
};
#endif //EXPRINTER_ARITHEXPR_HPP
