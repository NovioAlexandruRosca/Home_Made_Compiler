%{
#include <stdio.h>
#include "Node.h"
#include "limbaj.tab.h"
%}
%option noyywrap
%%

"basic"|"smart"|"chatgpt"|"singurel"|"multicei"|"maybe" {
    yylval.string = strdup(yytext);
    return TYPE;
}

"fn" {
    return FUNCTION;
}

"gt" {
    return GT;
}

"ge" {
    return GE;
}

"lt" {
    return LT;
}

"le" {
    return LE;
}

"eq" {
    return EQ;
}

"!" {
    return NOT;
}

"or" {
    return OR;
}

"and" {
    return AND;
}

"fidel" {
    return CONST;
}

"hibrid" {
    return CLASS;
}

"VARIABLE_DECL" {
    return VARDCL;
}

"CLASS_DECL" {
    return CLSDCL;
}

"main" {
    return MAIN;
}

"FUNCTION_DECL" {
    return FCTDCL;
}

"for" {
    return FOR;
}

"while" {
    return WHILE;
}

"if" {
    return IF;
}

"else" {
    return ELSE;
}

"=" {
    return ASSIGN;
}

"Eval" {
    return EVAL;
}

"TypeOf" {
    return TYPEOF;
}

"true"|"false" {
    yylval.string = strdup(yytext);
    return BOOLEAN;
}

'[a-zA-Z0-9]' {
    yylval.string = strdup(yytext); 
    return CHAR;
}

\"[a-zA-Z0-9]*\" {
    yylval.string = strdup(yytext);
    return STRING;
}

[a-zA-Z][a-zA-Z0-9]* {
    yylval.string = strdup(yytext);
     return ID;
}

[1-9]+[0-9]*|[1-9]+[0-9]*"."[0-9]*[1-9]+|[1-9]+[0-9]*"."[0-9]+|[0]|[0]"."[0-9]*[1-9]+ {
    yylval.string = strdup(yytext);
    return NR;
}

[ \t] ;

\n {
    yylineno++;
}

. {
    return yytext[0];
}

%{
/*  
Data types:

basic - int
smart - float
chatgpt - double
singurel - char
multicei - string
maybe- bool

Array types:

a:basic<200>;

User defined types:

hibrid nume has{ 
    int x;
    fct;

    extrovert nume:tip;   (public)
    introver nume:tip<200>;    (private)

    extrovert nume_fct(x:tip) -> (){

    }

}

Declarari:

a:basic;

Constante:

fidel a:basic;

Functii:

nume_fct(x:tip) -> ()

Control statement

cand (if)
dela x please y panala z pas x++; (for) 

 */
%}

