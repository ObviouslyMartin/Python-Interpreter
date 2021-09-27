
#include<iostream>
#include "Expr.hpp"

// TODO: Add string '+' concat

/***********************************************************************************************************************
 *  Super class Functions
 **********************************************************************************************************************/

ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

ExprNode *&InfixExprNode::left() { return _left; }
ExprNode *&InfixExprNode::right() { return _right; }

ExprNode *&RelExprNode::left() { return _left; }
ExprNode *&RelExprNode::right() { return _right; }
ExprNode *&BoolExprNode::left() { return _left; }
ExprNode *&BoolExprNode::right() { return _right; }

/***********************************************************************************************************************
 *  Infix constructor
 **********************************************************************************************************************/

InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

/***********************************************************************************************************************
 *  Infix evaluate()
 **********************************************************************************************************************/

TypeDescriptor *InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    auto lValueDesc = (IntegerTypeDescriptor*)left()->evaluate(symTab);
    auto rValueDesc = (IntegerTypeDescriptor *) right()->evaluate(symTab);

    int lValue = lValueDesc->value();
    int rValue = rValueDesc->value();

    std::cout << "\nInfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    if (token().isAdditionOperator()) {
        int i = lValue + rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    } else if (token().isSubtractionOperator()) {
        int i = lValue - rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    } else if (token().isMultiplicationOperator()) {
        int i = lValue * rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    } else if (token().isDivisionOperator()) {
        int i = lValue / rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    } else if (token().isModuloOperator()) {
        int i = lValue % rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    } else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}


/***********************************************************************************************************************
 *  Infix print()
 **********************************************************************************************************************/

void InfixExprNode::print() {
    if(_left)
        _left->print();
    token().print();
    _right->print();
}

/***********************************************************************************************************************
 *  Relational constructor
 **********************************************************************************************************************/

RelExprNode::RelExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

/***********************************************************************************************************************
 *  Relational evaluate()
 **********************************************************************************************************************/

// Evaluates relational expressions returning 0 for false 1 for true
TypeDescriptor *RelExprNode::evaluate(SymTab &symTab) {
    auto lValueDesc = (IntegerTypeDescriptor *) left()->evaluate(symTab);
    auto rValueDesc = (IntegerTypeDescriptor *) right()->evaluate(symTab);

    int lValue = lValueDesc->value();
    int rValue = rValueDesc->value();

    std::cout << "RelExprNode::evaluate: " << lValue << " " << token().getRelOp() << " " << rValue << std::endl;

    if (token().isLessThanOperator()) {
        int i = lValue < rValue;
        std::cout << i<< std::endl;
        return new IntegerTypeDescriptor(i);
    } else if (token().isGreaterThanOperator()){
        int i = lValue > rValue;
        std::cout << i<< std::endl;
        return new IntegerTypeDescriptor(i);
    }else if(token().isLessThanEqualOperator()){
        int i = lValue <= rValue;
        std::cout << i<< std::endl;
        return new IntegerTypeDescriptor(i);
    }else if(token().isGreaterEqualOperator()){
        int i = lValue >= rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    }else if(token().isEqualOperator()){
        int i = lValue == rValue;
        std::cout << i<< std::endl;
        return new IntegerTypeDescriptor(i);
    }else if(token().isNotEqualOperator()) {
        int i = lValue != rValue;
        std::cout << i << std::endl;
        return new IntegerTypeDescriptor(i);
    }else {
        std::cout << "RelExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

/***********************************************************************************************************************
 *  Relational print()
 **********************************************************************************************************************/

void RelExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}
BoolExprNode::BoolExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}
void BoolExprNode::print(){
    if(_left)
        _left->print();
    token().print();
    _right->print();
}
TypeDescriptor *BoolExprNode::evaluate(SymTab &symTab) {
    auto isleftnull = (IntegerTypeDescriptor*)left();
    if(isleftnull != nullptr) {
        auto lValueDesc = (IntegerTypeDescriptor *) left()->evaluate(symTab);
        auto rValueDesc = (IntegerTypeDescriptor *) right()->evaluate(symTab);

        int lValue = lValueDesc->value();
        int rValue = rValueDesc->value();

        std::cout << "BoolExprNode::evaluate: " << lValue << " " << token().getBoolOp() << " " << rValue << std::endl;

        if (token().isOrOperator()) {
            int i = lValue || rValue;
            std::cout << i << std::endl;
            return new IntegerTypeDescriptor(i);
        } else if (token().isAndOperator()) {
            int i = lValue && rValue;
            std::cout << i << std::endl;
            return new IntegerTypeDescriptor(i);
        } else if (token().isNotOperator()) {
            int i = !rValue;
            std::cout << i << std::endl;
            return new IntegerTypeDescriptor(i);
        } else {
            std::cout << "RelExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
    else{
        auto rValueDesc = (IntegerTypeDescriptor *) right()->evaluate(symTab);

        int rValue = rValueDesc->value();

        std::cout << "\nInfixExprNode::evaluate: " << token().getBoolOp() << " " << rValue << std::endl;

        if (token().isNotOperator()) {
            int i = !rValue;
            std::cout << i << std::endl;
            return new IntegerTypeDescriptor(i);

        } else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
}
/***********************************************************************************************************************
 *  WholeNumber constructor
 **********************************************************************************************************************/

WholeNumber::WholeNumber(Token token): ExprNode{token} {}

/***********************************************************************************************************************
 *  WholeNumber print()
 **********************************************************************************************************************/

void WholeNumber::print() {
    token().print();
}

/***********************************************************************************************************************
 *  WholeNumber evaluate()
 **********************************************************************************************************************/

TypeDescriptor *WholeNumber::evaluate(SymTab &symTab) {
    auto value = new IntegerTypeDescriptor(token().getWholeNumber());

    std::cout << "\nWholeNumber::evaluate: returning " << value->value() << std::endl;
    std::cout << value->value() << std::endl;
    return value;
}

/***********************************************************************************************************************
 *  Variable Constructor
 **********************************************************************************************************************/

Variable::Variable(Token token): ExprNode{token} {}

/***********************************************************************************************************************
 *  Variable print()
 **********************************************************************************************************************/

void Variable::print() {

    token().print();

}

/***********************************************************************************************************************
 *  Variable evaluate()
 **********************************************************************************************************************/

TypeDescriptor *Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }

    std::cout << "Variable::evaluate: returning ";

    auto value = symTab.getValueFor(token().getName());

    if(value->type() == TypeDescriptor::INTEGER){
        std::cout << dynamic_cast<IntegerTypeDescriptor*>(value)->value() << std::endl;
    }else
        std::cout << dynamic_cast<StringTypeDescriptor*>(value)->value() << std::endl;

    return value;
}


String::String(Token token) : ExprNode(token) {}

void String::print() {
    std::cout << "\"";
    token().print();
    std::cout << "\"";
}

TypeDescriptor *String::evaluate(SymTab &symTab) {
    std::string printstring = token().getString();
    std::cout << "String::evaluate: returning " << printstring << std::endl;
    std::cout << printstring << std::endl;
    return new StringTypeDescriptor(token().getString());
}

TestList::TestList():_testlist(new std::vector<ExprNode*>){};

void TestList::addExpression(ExprNode *expr) {_testlist->emplace_back(expr);}

void TestList::evaluate(SymTab &symTab) {
    for(auto i: *_testlist){
        if(i != nullptr)
            i->evaluate(symTab);
    }
}

void TestList::print() {
    std::vector<ExprNode*> list;
    for(auto i: *_testlist){
        if(i != nullptr){
            list.emplace_back(i);
        }
    }

    for(auto i: list){
        i->print();
        if(i != list.at(list.size()-1))
            std::cout <<", ";
    }
}
void TestList::appendList(ExprNode *node) {
    std::cout << "Before:";
    for (auto i: *_testlist){
        i->print();
    }
    std::cout << std::endl;
    _testlist->push_back(node);
    std::cout << "After:";
    for (auto i: *_testlist){
        i->print();
    }
}
void TestList::popList(int i){
    int size = _testlist->size();
    std::cout << "Before:";
    for (auto i: *_testlist){
        i->print();
    }
    if(size < i){
        std::cout << "pop out of range";
        return;
    }
    std::cout << std::endl;
    _testlist->erase(_testlist->begin() + i);
    std::cout << "After:";
    for (auto i: *_testlist){
        i->print();
    }
}
ExprNode *TestList::getValueAt(int i) {
    int size = _testlist->size();
    if(i > size){
        std::cout << "Subscript out of range";
        return nullptr;
    }
    return _testlist->at(i);
}
Array::Array(TestList *testList): ExprNode(Token()),  _testList{testList} {}
void Array::print(){
    std::cout << '[';
    _testList->print();
    std::cout << ']';

}
TypeDescriptor *Array::evaluate(SymTab &symTab) {
    std::cout << "Array::evaluate returning";
    _testList->print();

    return new ArrayTypeDescriptor(_testList);
}

lenExpr::lenExpr(Token token): _id{token},  ExprNode(Token()){}
void lenExpr::print() {
    std::cout << "len(";
    _id.print();
    std::cout << ")" << std:: endl;

}

TypeDescriptor *lenExpr::evaluate(SymTab &symTab) {
   auto arryTD = symTab.getValueFor(_id.getName());
   auto thearray = dynamic_cast<ArrayTypeDescriptor*>(arryTD)->getList();
   auto list = thearray->getList();
   return new IntegerTypeDescriptor(list->size());
}

ParameterList::ParameterList(): _parameters{new std::vector<Token>} {}
void ParameterList::addExpression(Token id) {
    _parameters->emplace_back(id);
}
void ParameterList::print(){
    for (auto i: *_parameters){
        i.print();

    }

}
CallExpr::CallExpr(Token token):_functionId{token}, _tstList(new TestList), ExprNode{token}{}
CallExpr::CallExpr(Token token, TestList*) : _functionId{token}, _tstList{new TestList}, ExprNode(token) {}
void CallExpr::print() {
    _functionId.print();
    std::cout << "(";
    _tstList->print();
    std::cout << ")" << std::endl;

}
TypeDescriptor *CallExpr::evaluate(SymTab &symTab) {



    return new IntegerTypeDescriptor(1);

}

SubExpr::SubExpr(Token token): _arrayId{token}, ExprNode{token}{}
SubExpr::SubExpr(Token token, ExprNode* test): _arrayId{token}, _test{test}, ExprNode{token} {}
void SubExpr::print() {
    _arrayId.print();
    std::cout << '[';
    _test->print();
    std::cout << ']' << std::endl;
}
TypeDescriptor *SubExpr::evaluate(SymTab &symTab) {
    auto arryTD = dynamic_cast<ArrayTypeDescriptor*>(symTab.getValueFor(_arrayId.getName()));
    auto arrayTD = arryTD->getList();
    auto testEval = dynamic_cast<IntegerTypeDescriptor*>(_test->evaluate(symTab));
    int i = testEval->value();
    auto exprNode = arrayTD->getValueAt(i);
    int j = exprNode->token().getWholeNumber();

    return new IntegerTypeDescriptor(j);
}