
#include <iostream>
#include "SymTab.hpp"


void SymTab::setValueFor(std::string vName, int value) {
    // Define a variable by setting its initial value.
    std::cout << vName << " <- " << value << std::endl;
    symTab[vName] = new IntegerTypeDescriptor(vName,value);
}


void SymTab::setValueFor(std::string vName, std::string value) {
    // Define a variable by setting its initial value.
    std::cout << vName << " <- " << value << std::endl;
    symTab[vName] = new StringTypeDescriptor(vName,value);
}
void SymTab::setValueFor(std::string vName, TestList *testList) {
    std::cout << vName << " <- ";
    testList->print();
    std::cout << std::endl;
    symTab[vName] = new ArrayTypeDescriptor(testList);
}
void SymTab::setValueFor(std::string name, ParameterList *parameterList, Statements *statements){
    std::cout << name << " <- ";
    std::cout << "(";
    parameterList->print();
    std::cout << "):";
    symTab[name] = new FunctionTypeDescriptor(name, parameterList, statements);


}
bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}


TypeDescriptor *SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }

    auto value = symTab.find(vName)->second;

    if(value->type() == TypeDescriptor::INTEGER){
        auto intData = dynamic_cast<IntegerTypeDescriptor*>(value);
        std::cout << "SymTab::getValueFor: " << vName << " contains " << intData->value() << std::endl;
    }else if(value->type() == TypeDescriptor::STRING){
        auto stringData = dynamic_cast<StringTypeDescriptor*>(value);
        std::cout << "SymTab::getValueFor: " << vName << " contains " << stringData->value() << std::endl;
    }else if(value->type() == TypeDescriptor::ARRAY){
        auto arrayData = dynamic_cast<ArrayTypeDescriptor*>(value);
        std::cout << "SymTab::getValueFor: " << vName << " contains ";
        arrayData->getList()->print();
        std::cout << std::endl;
    }else{
        auto functionData = dynamic_cast<FunctionTypeDescriptor*>(value);
        std::cout << "SymTab::getValueFor: " << vName << " contains ";
        functionData->getParametList()->print();
        std::cout << std::endl;
    }

    return value;
}


IntegerTypeDescriptor::IntegerTypeDescriptor(std::string name, int value): TypeDescriptor(INTEGER),_name(name),_value(value){}
IntegerTypeDescriptor::IntegerTypeDescriptor(int value): TypeDescriptor(INTEGER),_name(""),_value(value){}
IntegerTypeDescriptor::IntegerTypeDescriptor(std::string name): TypeDescriptor(INTEGER),_name(name),_value(0){}

StringTypeDescriptor::StringTypeDescriptor(std::string name, std::string value):TypeDescriptor(STRING),_name(name),_value(value){}
StringTypeDescriptor::StringTypeDescriptor(std::string value):TypeDescriptor(STRING),_name(""),_value(value){}

ArrayTypeDescriptor::ArrayTypeDescriptor(std::string name, TestList *testList): _name(name), TypeDescriptor(ARRAY), _tstList(testList){}
ArrayTypeDescriptor::ArrayTypeDescriptor(): TypeDescriptor(ARRAY), _tstList(new TestList()){}
ArrayTypeDescriptor::ArrayTypeDescriptor(TestList *tstList):TypeDescriptor(ARRAY), _tstList(tstList), _name(""){}

FunctionTypeDescriptor::FunctionTypeDescriptor(Statements* stmt) : TypeDescriptor(FUNCTION), _parameterList{new ParameterList}, _statements{stmt}, _name{""}, _return{nullptr} {}
FunctionTypeDescriptor::FunctionTypeDescriptor(ParameterList* parameterList, Statements* stmt): TypeDescriptor(FUNCTION), _parameterList{parameterList}, _statements{stmt},_name{""},_return{nullptr} {}
FunctionTypeDescriptor::FunctionTypeDescriptor(std::string name, ParameterList *parameterList, Statements* stmt) : TypeDescriptor(FUNCTION), _parameterList{parameterList}, _statements{stmt}, _name{name}, _return{nullptr} {}
Statements *FunctionTypeDescriptor::getStatements() { return _statements;}
ParameterList *FunctionTypeDescriptor::getParametList() { return _parameterList;}
std::string FunctionTypeDescriptor::getName() { return _name;}