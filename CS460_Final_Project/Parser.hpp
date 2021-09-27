
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"
#include<stack>
#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();
    AssignmentStatement *assignStatement(Token token);
    PrintStatement *printStatement();
    ForStatement *forStatement();
    RangeStatement *rangeStatement();
    IfStatement *ifStatement();
    FunctionStatement *functionStatement();
    ReturnStatement *returnStatement();
    CallStatment *callStatment(Token token);
    TestList *testList();
    ExprNode *test();
    ExprNode *orTest();
    ExprNode *andTest();
    ExprNode *notTest();
    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();
    ExprNode *arrayInit();
    ArrayOps *arrayOps(Token token);
    ExprNode *lengthExpr();
    ParameterList *parameterList();
    ExprNode *calledExpr(Token token);
    ExprNode *subExpr(Token token);



private:
    Tokenizer &tokenizer;
    std::stack<int> indent;
    void die(std::string where, std::string message, Token &token);
};

#endif

