#include "Statements.hpp"

/***********************************************************************************************************************
 *  Statement constructor
 **********************************************************************************************************************/
Statement::Statement() {}

/***********************************************************************************************************************
 *  Statements constructor
 **********************************************************************************************************************/

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

/***********************************************************************************************************************
 *  Statements print()
 **********************************************************************************************************************/

void Statements::print() {
    std::cout << std::endl;
    for (auto s: _statements) {
        s->print();
        std::cout << std::endl;
    }
}

/***********************************************************************************************************************
 *  Statements evaluate()
 **********************************************************************************************************************/
void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

/***********************************************************************************************************************
 *  AssignStatement constructors
 **********************************************************************************************************************/
AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

/***********************************************************************************************************************
 *  AssignStatement evaluate()
 **********************************************************************************************************************/

void AssignmentStatement::evaluate(SymTab &symTab) {

    auto rhs = rhsExpression()->evaluate(symTab);

    if(rhs->type() == TypeDescriptor::INTEGER){
        auto right = dynamic_cast<IntegerTypeDescriptor*>(rhs);
        symTab.setValueFor(lhsVariable(),right->value());
    }else if(rhs->type() == TypeDescriptor::STRING){
        auto right = dynamic_cast<StringTypeDescriptor*>(rhs);
        symTab.setValueFor(lhsVariable(),right->value());
    }else{
        auto right = dynamic_cast<ArrayTypeDescriptor*>(rhs);
        symTab.setValueFor(lhsVariable(), right->getList());
    }
}

/***********************************************************************************************************************
 *  AssignStatement accessor functions
 **********************************************************************************************************************/

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

/***********************************************************************************************************************
 *  AssignStatement print()
 **********************************************************************************************************************/

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
}


/***********************************************************************************************************************
 *  PrintStatement constructors
 **********************************************************************************************************************/
PrintStatement::PrintStatement(): _testList(nullptr){}
PrintStatement::PrintStatement(TestList *testlist): _testList(testlist) {}

/***********************************************************************************************************************
 *  PrintStatement print()
 **********************************************************************************************************************/

void PrintStatement::print() {
    std::cout << "print ";
    _testList->print();
}


/***********************************************************************************************************************
 *  PrintStatement evaluate()
 **********************************************************************************************************************/

void PrintStatement::evaluate(SymTab &symTab) {
    std::cout << "\nPrintStatement::evaluate: \n{\n";
    _testList->evaluate(symTab);
    std::cout << "}\n";
}


/***********************************************************************************************************************
 *  ForStatement print()
 **********************************************************************************************************************/
// TODO: Reimplement for print and eval
void ForStatement::print() {
    std::cout << "for ";
    _id.print();
    std::cout << " in ";
    _range->print();
    std::cout << ":";
    _body->print();
    std::cout<<std::endl;
}

/***********************************************************************************************************************
 *  ForStatement evaluate()
 **********************************************************************************************************************/

void ForStatement::evaluate(SymTab &symTab) {
    // Get ID token name
    std::string id = _id.getName();
    // Get the testlist for range function
    auto tstList = _range->getTestList();
    // Get the ExprNodes from the testlist
    std::vector<ExprNode*> *tests = tstList->getList();

    std::cout << "ForStatement::evaluate: ";
    std::cout << "Range::listSize: "<<tests->size();

    // Range has one element set end bound
    if(tests->size() == 1){
        auto bound = dynamic_cast<IntegerTypeDescriptor*>(tests->at(0)->evaluate(symTab))->value();
        for(int i = 0; i < bound; i++){
            symTab.setValueFor(id,i);
            _body->evaluate(symTab);
        }
    }else if(tests->size() == 2){
        auto begin = dynamic_cast<IntegerTypeDescriptor*>(tests->at(0)->evaluate(symTab))->value();
        auto bound = dynamic_cast<IntegerTypeDescriptor*>(tests->at(1)->evaluate(symTab))->value();
        for(int i = begin; i < bound; i++){
            symTab.setValueFor(id,i);
            _body->evaluate(symTab);
        }
    }else if(tests->size() == 3){
        auto begin = dynamic_cast<IntegerTypeDescriptor*>(tests->at(0)->evaluate(symTab))->value();
        auto bound = dynamic_cast<IntegerTypeDescriptor*>(tests->at(1)->evaluate(symTab))->value();
        auto step = dynamic_cast<IntegerTypeDescriptor*>(tests->at(2)->evaluate(symTab))->value();
        for(int i = begin; i < bound; i+= step){
            symTab.setValueFor(id,i);
            _body->evaluate(symTab);
        }
    }else{
        std::cout << "TOO MANY VALUES IN RANGE";
    }

}

/***********************************************************************************************************************
 *  ForStatement constructors
 **********************************************************************************************************************/

ForStatement::ForStatement(): _body(nullptr), _range(nullptr){}

ForStatement::ForStatement(Token id, RangeStatement *rangeStatement, Statements *body):
    _body(body), _id(id), _range(rangeStatement){}


void RangeStatement::evaluate(SymTab &symTab) {
    // implemented range as statement instead of key word ... ignore
}

void RangeStatement::print() {
}

TestList *RangeStatement::getTestList() {
    return _testList;
}


IfStatement::IfStatement(): _statements{nullptr},_test{nullptr} {}

IfStatement::IfStatement(ExprNode * test, Statements *statements): _test{test},_statements{statements} {}

// TODO: Implement if print
void IfStatement::print() {
    std::cout << "if ";
    _test->print();
    std::cout <<":";
    _statements->print();
}

void IfStatement::evaluate(SymTab &symTab) {
    std::cout << "\nIfStatement::evaluate: \n{\n";
    auto test = dynamic_cast<IntegerTypeDescriptor*>(_test->evaluate(symTab));
    int testValue = test->value();

    if(testValue > 0)
        _statements->evaluate(symTab);
    std::cout << "\n}";
}
ArrayOps::ArrayOps(Token token, Token tok,  ExprNode *test): _id{token}, _type{tok}, _test{test} {}
void ArrayOps::print(){
    std::cout << _id.getName();
    std::cout << ".";
    _type.print(); //append or pop or len
    std::cout << "(";
    _test->print();
    std::cout << ")";
}

void ArrayOps::evaluate(SymTab &symTab){
    std::string type = _type.getName();
    auto array = dynamic_cast<ArrayTypeDescriptor*>(symTab.getValueFor(_id.getName()));
    auto testList = array->getList();
    if (type == "append"){
        testList->appendList(_test);
    }else{
        auto i = dynamic_cast<IntegerTypeDescriptor*>(_test->evaluate(symTab));
        int j = i->value();
        testList->popList(j);
    }
}
FunctionStatement::FunctionStatement(): _functionSymTab{new SymTab}, _parameterList{new ParameterList}, _statements{new Statements} {}
FunctionStatement::FunctionStatement(ParameterList *list, Statements* statements, Token id): _functionSymTab{new SymTab}, _parameterList{list}, _statements{statements}, _id{id}{}
void FunctionStatement::print(){
    std::cout << "def ";
    _id.print();
    std::cout << "(";
    _parameterList->print();
    std::cout << "):";
    _statements->print();
}
void FunctionStatement::evaluate(SymTab &symTab) {
    //TODO: evaluate this
    //put params in symtab from call stmt
    //eval all statements
    //auto funcTD = new FunctionTypeDescriptor(_id.getName(), _parameterList, _statements);
    symTab.setValueFor(_id.getName(), _parameterList, _statements);

}
ReturnStatement::ReturnStatement() : _test{nullptr}{}
ReturnStatement::ReturnStatement(ExprNode* test): _test{test} {}
void ReturnStatement::print() {
    std::cout << "return";
    _test->print();

}
void ReturnStatement::evaluate(SymTab &symTab) {
    //TODO: return statement evaluate
}

CallStatment::CallStatment() : _functionId{Token()}, _testList{new TestList}{}
CallStatment::CallStatment(TestList* tstList, Token id): _testList{tstList}, _functionId{id} {}
void CallStatment::print(){
    _functionId.print();
    std::cout << '(';
    _testList->print();
    std::cout << ')';
}
void CallStatment::evaluate(SymTab &symTab) {
    std::cout << _functionId.getName() << "(";
    _testList->print();
    std::cout << ")";
    auto function = symTab.getValueFor(_functionId.getName());
    auto funcParams = dynamic_cast<FunctionTypeDescriptor*>(function);
    auto functionStatements = funcParams->getStatements();
    int paramsSize = funcParams->getParametList()->getParameters()->size();
    int testListSize = _testList->getList()->size();
    if(testListSize != paramsSize){
        std::cout << "missing function arguments" << std::endl;
    }
    SymTab FuncSymTab;
    for (int i = 0; i < testListSize; i++){
        std::string functionName = funcParams->getParametList()->getParameters()->at(i).getName();
        auto param1 = _testList->getValueAt(i)->evaluate(symTab);
        if(param1->type() == TypeDescriptor::INTEGER){
            auto dfs = dynamic_cast<IntegerTypeDescriptor*>(param1);
            FuncSymTab.setValueFor(functionName, dfs->value());
        }
        else if (param1->type() == TypeDescriptor::STRING){
            auto dfs = dynamic_cast<StringTypeDescriptor*>(param1);
            FuncSymTab.setValueFor(functionName, dfs->value());
        }
        else if(param1->type() == TypeDescriptor::ARRAY){
            auto dfs = dynamic_cast<ArrayTypeDescriptor*>(param1);
            FuncSymTab.setValueFor(functionName, dfs->getList());
        }

    }
    functionStatements->evaluate(FuncSymTab);

}
