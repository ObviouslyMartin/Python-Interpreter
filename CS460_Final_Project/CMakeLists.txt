cmake_minimum_required(VERSION 3.14)
project(CS460_Final_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CS460_Final_Project
        CMakeLists.txt
        Expr.cpp
        Expr.hpp
        input2.txt
        inputExpressions.txt
        main.cpp
        Makefile
        Parser.cpp
        Parser.hpp
        README.txt
        Statements.cpp
        Statements.hpp
        SymTab.cpp
        SymTab.hpp
        Token.cpp
        Token.hpp
        Tokenizer.cpp
        Tokenizer.hpp)
