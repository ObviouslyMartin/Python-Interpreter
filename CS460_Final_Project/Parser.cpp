
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

/***********************************************************************************************************************
 *  Statements()
 **********************************************************************************************************************/

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of statements.

    auto *stmts = new Statements();
    Token tok = tokenizer.getToken();

    while(tok.isName() || tok.eol() || tok.isIndent()) {

        // Handle indents
        if(tok.isIndent()){
            // Check the indentation stack
            if(tok.getIndent() > indent.top() || indent.empty()){
                die("Parser::statements", "Unexpected Indent", tok);
            }else if(tok.getIndent() < indent.top()){
                // Dedent the stack
                indent.pop();
                tokenizer.ungetToken();
                return stmts;
            }
            tok = tokenizer.getToken();
        }else{
            if(!indent.empty()){
                indent.pop();
                tokenizer.ungetToken();
                return stmts;
            }
        }

        // Check to see if name is a keyword
        if(tok.isKeyword()){

            if(tok.isPrint()){
                tokenizer.ungetToken();
                PrintStatement *printStmt = printStatement();
                stmts->addStatement(printStmt);
            }else if(tok.isFor()){
                tokenizer.ungetToken();
                ForStatement *forStmt = forStatement();
                stmts->addStatement(forStmt);
            }else if(tok.isIf()){
                tokenizer.ungetToken();
                IfStatement *ifStmt = ifStatement();
                stmts->addStatement(ifStmt);
            }
            else if(tok.isDef()){
                tokenizer.ungetToken();
                FunctionStatement *functionStatement1 = functionStatement();
                stmts->addStatement(functionStatement1);
            }
            else if(tok.isReturn()){
                tokenizer.ungetToken();
                ReturnStatement *returnStatement1 = returnStatement();
                stmts->addStatement(returnStatement1);
            }
        }else if(tok.eol()){}else{
            // Case for normal words
            //tokenizer.ungetToken();
            Token nextoken = tokenizer.getToken();
            if(nextoken.isDotOperator()){
                tokenizer.ungetToken();
                ArrayOps *arrayOP = arrayOps(tok);
                stmts->addStatement(arrayOP);
            }
            else if(nextoken.isOpenParen()){
                tokenizer.ungetToken();
                CallStatment *cllStatement = callStatment(tok);
                stmts->addStatement(cllStatement);
            }else{
                tokenizer.ungetToken();
                AssignmentStatement *assignStmt = assignStatement(tok);
                stmts->addStatement(assignStmt);
            }

        }
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();

    return stmts;
}


IfStatement *Parser::ifStatement() {
    Token tok = tokenizer.getToken();
    if(!tok.isIf())
        die("Parser::ifStatement", "Expected an if token, instead got", tok);

    ExprNode *testExpr = test();

    tok = tokenizer.getToken();
    if(!tok.isColon())
        die("Parser::ifStatement", "Expected a colon token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.eol())
        die("Parser::ifStatement", "Expected an end of line token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.isIndent())
        die("Parser::ifStatement", "Expected an indent token, instead got", tok);

    indent.push(tok.getIndent());
    tokenizer.ungetToken();

    Statements *body = statements();

    return new IfStatement(testExpr,body);
}




/***********************************************************************************************************************
 *  forStatement()
 **********************************************************************************************************************/
// TODO: Finish For Loop Parsing
// Check for proper for loop syntax
ForStatement *Parser::forStatement() {
    Token tok = tokenizer.getToken();
    if (!tok.isFor())
        die("Parser::forStatement", "Expected a for token, instead got", tok);

    Token varName = tokenizer.getToken();
    if(!varName.isName())
        die("Parser::forStatement", "Expected a name token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.isIn())
        die("Parser::forStatement", "Expected an in token, instead got", tok);

    tok = tokenizer.getToken();
    if(tok.isRange())
        tokenizer.ungetToken();
    else
        die("Parser::forStatement", "Expected a range token, instead got", tok);

    RangeStatement *rangeStmt = rangeStatement();

    tok = tokenizer.getToken();
    if(!tok.isColon())
        die("Parser::forStatement", "Expected a colon token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.eol())
        die("Parser::forStatement", "Expected an end of line token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.isIndent())
        die("Parser::forStatement", "Expected an indent token, instead got", tok);

    indent.push(tok.getIndent());
    tokenizer.ungetToken();

    Statements *body = statements();

    return new ForStatement(varName,rangeStmt,body);

}
FunctionStatement *Parser::functionStatement() {
    Token tok = tokenizer.getToken();
    if (!tok.isDef())
        die("Parser::functionStatement", "Expected a def token, instead got", tok);

    Token id = tokenizer.getToken();
    if(!id.isName())
        die("Parser::functionStatement", "Expected a name token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.isOpenParen())
        die("Parser::functionStatement", "Expected an ( token, instead got", tok);

    auto paramList = parameterList();
    tok = tokenizer.getToken();
    if(!tok.isCloseParen()){
        die("Parser::functionStatement", "Expected a ) token, instead got", tok);

    }
    tok = tokenizer.getToken();
    if(!tok.isColon()) {
        die("Parser::functionStatement", "Expected a : token, instead got", tok);
    }
    tok = tokenizer.getToken();
    if(!tok.eol())
        die("Parser::functionStatement", "Expected an end of line token, instead got", tok);

    tok = tokenizer.getToken();
    if(!tok.isIndent())
        die("Parser::functionStatement", "Expected an indent token, instead got", tok);

    indent.push(tok.getIndent());
    tokenizer.ungetToken();

    Statements *body = statements();

    return new FunctionStatement(paramList,body,id);
}
/***********************************************************************************************************************
 *  rangeStatement()
 **********************************************************************************************************************/
// TODO: Implement Range Functions
RangeStatement *Parser::rangeStatement(){

    Token token = tokenizer.getToken();
    if (!token.isRange())
        die("Parser::rangeStatement", "Expected a range token, instead got", token);

    token = tokenizer.getToken();
    if (!token.isOpenParen())
        die("Parser::rangeStatement", "Expected an open parenthesis token, instead got", token);

    auto tstList = testList();

    token = tokenizer.getToken();
    if (!token.isCloseParen())
        die("Parser::rangeStatement", "Expected a close parenthesis, instead got", token);


    return new RangeStatement(tstList);
}

/***********************************************************************************************************************
 *  printStatement()
 **********************************************************************************************************************/
// Check for proper print statement syntax
PrintStatement *Parser::printStatement() {
    Token print = tokenizer.getToken();
    if (!print.isPrint())
        die("Parser::printStatement", "Expected a print token, instead got", print);

    Token word = tokenizer.getToken();

    if(!word.isName() && !word.isString())
        die("Parser::printStatement", "Expected a word token, instead got", word);

    tokenizer.ungetToken();

    auto tstLst = testList();

    Token endLine = tokenizer.getToken();

    if(!endLine.eol())
        die("Parser::printStatement", "Expected a new line token, instead got", endLine);

    return new PrintStatement(tstLst);
}

TestList *Parser::testList() {

    Token tok = tokenizer.getToken();
    auto *tstLst = new TestList();
    if (tok.isCloseBracket() || tok.isCloseParen()) {
        tokenizer.ungetToken();
        return tstLst;
    }

    if(!tok.isName() && !tok.isString() && !tok.isWholeNumber())
        die("Parser::testList", "Expected a word token, instead got", tok);

    tokenizer.ungetToken();

    do{
        tstLst->addExpression(test());
        tok = tokenizer.getToken();
    }while(tok.isComma());

    tokenizer.ungetToken();

    return tstLst;
}


ExprNode *Parser::test() {
    ExprNode *left = orTest();
    return left;
}

ExprNode *Parser::orTest(){
    ExprNode *left = andTest();
    Token tok = tokenizer.getToken();

    while (tok.isOrOperator()) {
        auto *p = new BoolExprNode(tok);
        p->left() = left;
        p->right() = andTest();
        left = p;
        tok = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::andTest() {
    ExprNode *left = notTest();
    Token tok = tokenizer.getToken();

    while (tok.isAndOperator()) {
        auto *p = new BoolExprNode(tok);
        p->left() = left;
        p->right() = notTest();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}
ExprNode *Parser::notTest(){

    Token tok = tokenizer.getToken();

    if(tok.isNotOperator()) {
        auto *p = new BoolExprNode(tok);
        ExprNode *right = notTest();
        p->left() = nullptr;
        p->right() = right;
        right = p;

        tokenizer.ungetToken();
        return right;
    }else{
        tokenizer.ungetToken();
        ExprNode *left = expr();
        return left;
    }

}
/***********************************************************************************************************************
 *  assignStatement()
 **********************************************************************************************************************/

AssignmentStatement *Parser::assignStatement(Token token) {
    //Token varName = tokenizer.getToken();
    if (!token.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", token);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);
    Token bracket = tokenizer.getToken();
    ExprNode *rhs;
    if(bracket.isOpenBracket()){
        tokenizer.ungetToken();
        rhs = arrayInit();
        return new AssignmentStatement(token.getName(), rhs);
    }else{
        tokenizer.ungetToken();
        rhs = test();
    }

    Token endLine = tokenizer.getToken();
    if (!endLine.eol())
        die("Parser::assignStatement", "Expected a eol, instead got", endLine);

    return new AssignmentStatement(token.getName(), rhs);
}

/***********************************************************************************************************************
 *  expr()
 **********************************************************************************************************************/

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();

    if(tok.isArithmeticOperator()){
        while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
            auto *p = new InfixExprNode(tok);
            p->left() = left;
            p->right() = term();
            left = p;
            tok = tokenizer.getToken();
        }
    }else if(tok.isRelationalOperator()){
        while(tok.isGreaterThanOperator() || tok.isLessThanOperator() ||
                tok.isGreaterEqualOperator() || tok.isLessThanEqualOperator()){
            auto *p = new RelExprNode(tok);
            p->left() = left;
            p->right() = term();
            left = p;
            tok = tokenizer.getToken();
        }
    }

    tokenizer.ungetToken();
    return left;
}


/***********************************************************************************************************************
 *  term()
 **********************************************************************************************************************/

ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    // Check to see if the token is arithmetic operator or relational and act accordingly
    if(tok.isArithmeticOperator()){
        while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
            auto *p = new InfixExprNode(tok);
            p->left() = left;
            p->right() = primary();
            left = p;
            tok = tokenizer.getToken();
        }
    }else if(tok.isRelationalOperator()){
        while(tok.isNotEqualOperator() || tok.isEqualOperator()){
            auto *p = new RelExprNode(tok);
            p -> left() = left;
            p -> right() = primary();
            left = p;
            tok = tokenizer.getToken();
        }
    }


    tokenizer.ungetToken();
    return left;
}

/***********************************************************************************************************************
 *  primary()
 **********************************************************************************************************************/

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber())
        return new WholeNumber(tok);
    else if (tok.isName()) {
        if (tok.isLen()) {
            return lengthExpr();
        }
        Token token = tokenizer.getToken();
        if(token.isOpenParen()){
            tokenizer.ungetToken();
            return calledExpr(tok);
        }
        else if(token.isOpenBracket()){
            tokenizer.ungetToken();
            return subExpr(tok);
        }
        tokenizer.ungetToken();
        return new Variable(tok);
    }
    else if( tok.isString())
        return new String(tok);
    else if( tok.isComma()){
        tokenizer.ungetToken();
        return nullptr;
    }else if (tok.isOpenParen()) {
        ExprNode *p = expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
ExprNode *Parser::arrayInit() {
    Token openBrack = tokenizer.getToken();
    if(!openBrack.isOpenBracket()){
        die("Parser::arrayInit", "Expected open bracket", openBrack);
    }
    auto tstList = testList();
    Token closeBrack = tokenizer.getToken();
    if(!closeBrack.isCloseBracket()){
        die("Parser::arrayInit", "Expected Close", closeBrack);
    }

    Token eol = tokenizer.getToken();
    if(!eol.eol()){
        die("Parser::arrayInit", "expected EOL instead got", eol);
    }

    return new Array(tstList);
}

ArrayOps *Parser::arrayOps(Token token) {
    Token tok = tokenizer.getToken();
    if(!tok.isDotOperator()){
        die("Parser::arrayOps", "expected . instead got", tok);
    }

    Token tokType = tokenizer.getToken();
    if(!tokType.isPop() && !tokType.isAppend()){
        die("Parser::arrayOps", "expected pop or append instead got", tok);

    }

    tok = tokenizer.getToken();
    if(!tok.isOpenParen()){
        die("Parser::arrayOps", "expected ( instead got", tok);
    }

    ExprNode *tst = test();
    Token token1 = tokenizer.getToken();
    if(!token1.isCloseParen()){
        die("Parser::arrayOps", "expected ) instead got", token1);
    }
    token1 = tokenizer.getToken();
    if(!token1.eol()){
        die("Parser::arrayOps", "expected eol instead got", token1);

    }
    return new ArrayOps(token, tokType, tst);
}

ExprNode *Parser::lengthExpr() {
    Token tok = tokenizer.getToken();
    if(!tok.isOpenParen()){
        die("Parser::lengthExpr", "expected ( instead got", tok);

    }
    tok = tokenizer.getToken();
    if(!tok.isName()){
        die("Parser::lengthExpr", "expected a name instead got", tok);

    }
    Token nextToken = tokenizer.getToken();
    if(!nextToken.isCloseParen()){
        die("Parser::lengthExpr", "expected ) instead got", nextToken);

    }
//
//    nextToken = tokenizer.getToken();
//    if(!nextToken.eol())
//        die("Parser::lengthExpr", "expected eol instead got", nextToken);
    return new lenExpr(tok);
}

ParameterList *Parser::parameterList() {
    Token tok = tokenizer.getToken();
    auto *params = new ParameterList();
    if (tok.isCloseParen()) {
        tokenizer.ungetToken();
        return params;
    }

    if(!tok.isName())
        die("Parser::testList", "Expected a name token, instead got", tok);

    tokenizer.ungetToken();

    do{
        tok = tokenizer.getToken();
        params->addExpression(tok);
        tok = tokenizer.getToken();
    }while(tok.isComma());

    tokenizer.ungetToken();

    return params;
}
ReturnStatement *Parser::returnStatement() {
    Token token = tokenizer.getToken();
    if(!token.isReturn()){
        die("Parser::returnStatement", "Expected a return token, instead got", token);
    }
    auto retTest = test();
    Token eol = tokenizer.getToken();
    if(!eol.eol()){
        die("Parser::returnStatement", "Expected a eol token, instead got", eol);
    }
    return new ReturnStatement(retTest);
}
CallStatment *Parser::callStatment(Token token) {
    //Token tok = tokenizer.getToken();
    if(!token.isName()){
        die("Parser::callStatement", "Expected a name token, instead got", token);
    }
    Token token1 = tokenizer.getToken();
    if(!token1.isOpenParen()){
        die("Parser::callStatement", "Expected a ( token, instead got", token1);
    }
    auto tstList = testList();
    Token token2 = tokenizer.getToken();
    if(!token2.isCloseParen()){
        die("Parser::callStatement", "Expected a ) token, instead got", token2);
    }

    Token eol = tokenizer.getToken();
    if(!eol.eol()){
        die("Parser::callStatement", "Expected a eol token, instead got", eol);
    }
    return new CallStatment(tstList, token);
}

ExprNode *Parser::calledExpr(Token token) {

    if(!token.isName()){
        die("Parser::callStatement", "Expected a name token, instead got", token);
    }
    Token token1 = tokenizer.getToken();
    if(!token1.isOpenParen()){
        die("Parser::callStatement", "Expected a ( token, instead got", token1);
    }
    auto tst = testList();
    Token token2 = tokenizer.getToken();
    if(!token2.isCloseParen()){
        die("Parser::callStatement", "Expected a ) token, instead got", token2);
    }

    Token eol = tokenizer.getToken();
    if(!eol.eol()){
        die("Parser::callStatement", "Expected a eol token, instead got", eol);
    }
    tokenizer.ungetToken();
    return new CallExpr(token, tst);
}
ExprNode *Parser::subExpr(Token token) {
    if(!token.isName()){
        die("Parser::subExpr", "Expected a name token, instead got", token);
    }
    Token token1 = tokenizer.getToken();
    if(!token1.isOpenBracket()){
        die("Parser::subExpr", "Expected a [ token, instead got", token1);
    }
    auto tst = test();
    Token token2 = tokenizer.getToken();
    if(!token2.isCloseBracket()){
        die("Parser::subExpr", "Expected a ] token, instead got", token2);
    }

    Token eol = tokenizer.getToken();
    if(!eol.eol()){
        die("Parser::subExpr", "Expected a eol token, instead got", eol);
    }
    return new SubExpr(token, tst);
}