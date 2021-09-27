//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "Expr.hpp"
#include "SymTab.hpp"
// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

// TODO: Implement If statements and functions

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
    virtual ~Statement() = default;
};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);
    void print();
    ~Statements(){
        for (auto i: _statements){
            delete(i);
        }
    }
private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);
//    AssignmentStatement(std::string lhsVar, TestList*testList);
    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();
    ~AssignmentStatement() {
        delete(_rhsExpression);
    }

private:
    std::string _lhsVariable;

    ExprNode *_rhsExpression;
};

class PrintStatement: public Statement{
public:
    PrintStatement();
    PrintStatement(TestList*);
    virtual void evaluate(SymTab &symTab);
    virtual void print();
    ~PrintStatement(){
        delete(_testList);
    }
private:
    TestList *_testList;
};

// TODO: add testlist functionality and limit to 3 params
class RangeStatement: public Statement{
public:
    RangeStatement(): _testList(new TestList()){};
    RangeStatement(TestList* testList): _testList(testList){};
    virtual void evaluate(SymTab &symTab);
    virtual void print();
    TestList *getTestList();
    ~RangeStatement(){
        delete(_testList);
    }
private:
    TestList * _testList;
};



class ForStatement : public Statement{
public:
    ForStatement();
    ForStatement(Token,RangeStatement*,Statements*);

    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ~ForStatement(){
        delete(_range);
        delete(_body);
    }
private:
    Token _id;
    RangeStatement *_range;
    Statements *_body;
};

class IfStatement: public Statement{
public:
    IfStatement();
    IfStatement(ExprNode *,Statements*);

    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ~IfStatement(){
        delete(_test);
        delete(_statements);
    }
private:
    ExprNode *_test;
    Statements *_statements;
};

class FunctionStatement: public Statement{
public:
    FunctionStatement();
    FunctionStatement(ParameterList*, Statements*, Token);
    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ~FunctionStatement(){
        delete(_functionSymTab);
        delete(_parameterList);
        delete(_statements);
    }
private:
    SymTab *_functionSymTab;
    ParameterList *_parameterList;
    Statements *_statements;
    Token _id;

};

class ArrayOps: public Statement{
public:
    ArrayOps(Token, Token, ExprNode *);
    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ExprNode *getTest(){ return _test; }
    ~ArrayOps(){
        delete(_test);
    }
private:
    Token _id;
    Token _type;
    ExprNode *_test;
};
class CallStatment: public Statement{
public:
    CallStatment();
    CallStatment(TestList*, Token);
    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ~CallStatment(){
        delete(_testList);
    }
private:
    Token _functionId;
    TestList *_testList;
};
class ReturnStatement: public Statement{
public:
    ReturnStatement();
    ReturnStatement(ExprNode*);
    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ~ReturnStatement(){
        delete(_test);
    }
private:
    ExprNode *_test;

};
class SubscriptionStatement: public Statement{
public:
    SubscriptionStatement();
    SubscriptionStatement(Token, ExprNode*);
    virtual void print();
    virtual void evaluate(SymTab &symTab);
    ~SubscriptionStatement(){
        delete(_subscript);
    }
private:
    Token _arrayId;
    ExprNode *_subscript;

};
#endif //EXPRINTER_STATEMENTS_HPP
