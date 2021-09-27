//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include <vector>
#include "Expr.hpp"
//#include "Statements.hpp"
class TestList;
class Statements;
class ParameterList;

class TypeDescriptor{
public:
    enum types {INTEGER, DOUBLE, BOOL, STRING, ARRAY, FUNCTION};

    explicit TypeDescriptor(types type): _type(type){};
    types &type(){return _type;}
    virtual ~TypeDescriptor() = default;

private:
    types _type;
};

class IntegerTypeDescriptor: public TypeDescriptor{
public:
    IntegerTypeDescriptor(std::string,int);
    explicit IntegerTypeDescriptor(std::string);
    explicit IntegerTypeDescriptor(int);
    std::string name(){return _name;}
    int value(){return _value;}
private:
    std::string _name;
    int _value;
};

class StringTypeDescriptor: public TypeDescriptor{
public:
    StringTypeDescriptor(std::string,std::string);
    explicit StringTypeDescriptor(std::string);
    std::string name(){return _name;}
    std::string value(){ return _value;}

private:
    std::string _name;
    std::string _value;

};
class ArrayTypeDescriptor: public TypeDescriptor{
public:
    ArrayTypeDescriptor(TestList*);
    ArrayTypeDescriptor(std::string, TestList*);
    explicit ArrayTypeDescriptor();
    TestList *getList(){return _tstList;}
    ~ArrayTypeDescriptor(){
        delete(_tstList);
    }
private:
    std::string _name;
    TestList *_tstList;
};
class FunctionTypeDescriptor: public TypeDescriptor{
public:
    FunctionTypeDescriptor(ParameterList*, Statements* stmt);
    FunctionTypeDescriptor(std::string, ParameterList*, Statements* stmt);
    explicit FunctionTypeDescriptor(Statements* stmt);
    Statements *getStatements();
    ParameterList *getParametList();
    std::string getName();
    TypeDescriptor *&getReturn() { return _return;}
    ~FunctionTypeDescriptor(){
        delete(_statements);
        delete(_parameterList);
    }
private:
    Statements *_statements;
    TypeDescriptor *_return;
    ParameterList *_parameterList;
    std::string _name;

};

class SymTab {
public:
    explicit SymTab() : symTab{std::map<std::string,TypeDescriptor*>()} {}
    void setValueFor(std::string vName, int value);
    void setValueFor(std::string vName, std::string value);
    void setValueFor(std::string vName, TestList *testList);
    void setValueFor(std::string vName, ParameterList* parameterList, Statements *statements);
    bool isDefined(std::string vName);
    TypeDescriptor *getValueFor(std::string vName);

private:
    std::map<std::string,TypeDescriptor*> symTab;
};

#endif //EXPRINTER_SYMTAB_HPP
