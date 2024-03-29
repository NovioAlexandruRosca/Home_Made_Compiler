/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_LIMBAJ_TAB_H_INCLUDED
# define YY_YY_LIMBAJ_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 247 "limbaj.y"

    #include <vector>
    #include "Node.h" // Assuming Node is defined in Node.h

#line 54 "limbaj.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ASSIGN = 258,                  /* ASSIGN  */
    VARDCL = 259,                  /* VARDCL  */
    FCTDCL = 260,                  /* FCTDCL  */
    CONST = 261,                   /* CONST  */
    MAIN = 262,                    /* MAIN  */
    IF = 263,                      /* IF  */
    FOR = 264,                     /* FOR  */
    WHILE = 265,                   /* WHILE  */
    EVAL = 266,                    /* EVAL  */
    TYPEOF = 267,                  /* TYPEOF  */
    ELSE = 268,                    /* ELSE  */
    LT = 269,                      /* LT  */
    LE = 270,                      /* LE  */
    GT = 271,                      /* GT  */
    GE = 272,                      /* GE  */
    EQ = 273,                      /* EQ  */
    OR = 274,                      /* OR  */
    AND = 275,                     /* AND  */
    CLSDCL = 276,                  /* CLSDCL  */
    CLASS = 277,                   /* CLASS  */
    FUNCTION = 278,                /* FUNCTION  */
    ID = 279,                      /* ID  */
    TYPE = 280,                    /* TYPE  */
    NR = 281,                      /* NR  */
    CHAR = 282,                    /* CHAR  */
    STRING = 283,                  /* STRING  */
    BOOLEAN = 284,                 /* BOOLEAN  */
    NOT = 285                      /* NOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 251 "limbaj.y"

    char* string;
    int intt;
    Node* node;
    std::vector<Node*>* node_vector; 

#line 108 "limbaj.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_LIMBAJ_TAB_H_INCLUDED  */
