#!/bin/bash

echo "compiling $1"
rm -f lex.yy.c
rm -f $1.tab.c
rm -f $1
bison -d $1.y -Wcounterexamples
lex $1.l
g++ IdList.cpp FunctionList.cpp ClassList.cpp Node.cpp lex.yy.c  $1.tab.c -std=c++11 -o $1
