/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "limbaj.y"

 // verif bool
 // transf char urile in litere normale
    //am eliminat %type <intt> expression 

#include <iostream>
#include <string>
#include <vector>
#include "IdList.h"
#include "Node.h"
#include <cstring>
#include <cstdlib>
#include <sstream>

using namespace std;

void yyerror(const char * s);

int indexx;

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();

VariableList vars;
FunctionList fct;
ClassList cls;
string Typeof_expression;

vector<string> parseString(const string& inputString) {

   std::unordered_set<std::string> delimiters = {"+", "*", "-", "/", "lt", "le", "gt", "ge", "eq", "!", "or", "and", ")", "(", "or"};

    std::istringstream iss(inputString);
    std::vector<std::string> words;

    std::string token;

    while (iss >> token) {

        if (delimiters.find(token) == delimiters.end()) {
            words.push_back(token);
        } else {
            size_t pos = 0;
            while ((pos = token.find_first_of("+-*/()!", pos)) != std::string::npos) {
                std::string subToken = token.substr(pos, 1);
                if(token != "+" && token != "-" && token != "*" && token != "/" && token != "(" && token != ")" && token != "!")
                words.push_back(subToken);
                pos++;
            }
        }
    }

    return words;
}

string TypeOf(string expression){

    string input = expression;
    string typeof;
    bool typeof_selected = false;

    vector<string> tokens = parseString(input);


    for (auto& token : tokens) {
        int found = 0;

        if(token[0] == '!' || token[0] == '(')
            token.erase(0,1);
        
        if(token == "true" || token == "false")
        {
             // cout << "bool: " << token << endl;
             found = 1;
            if(typeof_selected == false)
            {
                typeof_selected = true;
                typeof = string("maybe");
            }
        }

        if(token.find('(') != string::npos)
        {
            size_t dotPosition = token.find('.');

            if (dotPosition != string::npos && dotPosition + 1 < token.length()) {

                token = token.substr(dotPosition + 1);

            }

            dotPosition = token.find('(');

            if (dotPosition != string::npos && dotPosition + 1 < token.length()) {

                token = token.substr(0,dotPosition);

            }

            // cout << "function: " << token << endl;
            
            for(auto& functions : fct.vars)
            {
                if(functions.name == token)
                {
                    found = 1;
                    if(typeof_selected == false)
                    {
                        typeof_selected = true;
                        typeof = functions.type;
                    }
                    else if(functions.type != typeof)
                    {
                        yyerror("TypeOf can only return a type if all the elements are of the same type");
                        return string("-1");
                    }
                }
            }
            if(found == 0)
            {
                yyerror("One of the elements doesnt exist in the app");
                return string("-1");
            }

        }
        else if(token[0] >= '0' && token[0] <= '9' && token.find('.') != string::npos)
        {
            // cout << "float: " << token << endl;
            found = 1;
            if(typeof_selected == false)
            {
                typeof_selected = true;
                typeof = string("smart");
            }
            else if(string("smart") != typeof)
            {
                yyerror("TypeOf can only return a type if all the elements are of the same type");
                return string("-1");
            }
        }
        else if(token[0] >= '1' && token[0] <= '9')
        {
            // cout << "int: " << token << endl;
            found = 1;
            if(typeof_selected == false)
            {
                typeof_selected = true;
                typeof = string("basic");
            }
            else if(string("basic") != typeof)
            {
                yyerror("TypeOf can only return a type if all the elements are of the same type");
                return string("-1");
            }
        }
        else if(token[0] == '\'')
        {
             // cout << "char: " << token << endl;
            found = 1;
            if(typeof_selected == false)
            {
                typeof_selected = true;
                typeof = string("singurel");
            }
            else if(string("singurel") != typeof)
            {
                yyerror("TypeOf can only return a type if all the elements are of the same type");
                return string("-1");
            }
        }
        else if(token[0] == '\"')
        {
             // cout << "char: " << token << endl;
            found = 1;
            if(typeof_selected == false)
            {
                typeof_selected = true;
                typeof = string("multicei");
            }
            else if(string("multicei") != typeof)
            {
                yyerror("TypeOf can only return a type if all the elements are of the same type");
                return string("-1");
            }
        }
        else
        {
            size_t dotPosition = token.find('.');

            if (dotPosition != string::npos && dotPosition + 1 < token.length()) {

                token = token.substr(dotPosition + 1);

            }

            string token_copy = token;
            token.clear();

            for(int j = 0 ; j < token_copy.size() ;j++)
                if(token_copy[j] != '<')
                    token += token_copy[j];
                else 
                    break;

            // cout << "variables: " << token << endl;
            for(auto& variable : vars.vars)
            {
                if(variable.name == token)
                {
                    found = 1;
                    if(typeof_selected == false)
                    {
                        typeof_selected = true;
                        typeof = variable.type;
                    }
                    else if(variable.type != typeof)
                    {
                        yyerror("TypeOf can only return a type if all the elements are of the same type");
                        return string("-1");
                    }
                }
            }
            if(found == 0)
            {
                yyerror("One of the elements doesnt exist in the app");
                return string("-1");
            }
        }
    }

    return typeof;
}

// %left '+' '-'
// %left '*' '/'
// %left AND OR
// %right NOT
// %nonassoc LT LE GT GE EQ


// %type <node> call_list_element
// %type <node_vector> call_list


#line 318 "limbaj.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "limbaj.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ASSIGN = 3,                     /* ASSIGN  */
  YYSYMBOL_VARDCL = 4,                     /* VARDCL  */
  YYSYMBOL_FCTDCL = 5,                     /* FCTDCL  */
  YYSYMBOL_CONST = 6,                      /* CONST  */
  YYSYMBOL_MAIN = 7,                       /* MAIN  */
  YYSYMBOL_IF = 8,                         /* IF  */
  YYSYMBOL_FOR = 9,                        /* FOR  */
  YYSYMBOL_WHILE = 10,                     /* WHILE  */
  YYSYMBOL_EVAL = 11,                      /* EVAL  */
  YYSYMBOL_TYPEOF = 12,                    /* TYPEOF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_LT = 14,                        /* LT  */
  YYSYMBOL_LE = 15,                        /* LE  */
  YYSYMBOL_GT = 16,                        /* GT  */
  YYSYMBOL_GE = 17,                        /* GE  */
  YYSYMBOL_EQ = 18,                        /* EQ  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_CLSDCL = 21,                    /* CLSDCL  */
  YYSYMBOL_CLASS = 22,                     /* CLASS  */
  YYSYMBOL_FUNCTION = 23,                  /* FUNCTION  */
  YYSYMBOL_ID = 24,                        /* ID  */
  YYSYMBOL_TYPE = 25,                      /* TYPE  */
  YYSYMBOL_NR = 26,                        /* NR  */
  YYSYMBOL_CHAR = 27,                      /* CHAR  */
  YYSYMBOL_STRING = 28,                    /* STRING  */
  YYSYMBOL_BOOLEAN = 29,                   /* BOOLEAN  */
  YYSYMBOL_30_ = 30,                       /* '+'  */
  YYSYMBOL_31_ = 31,                       /* '-'  */
  YYSYMBOL_32_ = 32,                       /* '*'  */
  YYSYMBOL_33_ = 33,                       /* '/'  */
  YYSYMBOL_NOT = 34,                       /* NOT  */
  YYSYMBOL_35_ = 35,                       /* '{'  */
  YYSYMBOL_36_ = 36,                       /* '}'  */
  YYSYMBOL_37_ = 37,                       /* ';'  */
  YYSYMBOL_38_ = 38,                       /* ':'  */
  YYSYMBOL_39_ = 39,                       /* '<'  */
  YYSYMBOL_40_ = 40,                       /* '>'  */
  YYSYMBOL_41_ = 41,                       /* ','  */
  YYSYMBOL_42_ = 42,                       /* '('  */
  YYSYMBOL_43_ = 43,                       /* ')'  */
  YYSYMBOL_44_ = 44,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_progr = 46,                     /* progr  */
  YYSYMBOL_declarations_block = 47,        /* declarations_block  */
  YYSYMBOL_custom_block = 48,              /* custom_block  */
  YYSYMBOL_class_declarations = 49,        /* class_declarations  */
  YYSYMBOL_class_declaration = 50,         /* class_declaration  */
  YYSYMBOL_class_body = 51,                /* class_body  */
  YYSYMBOL_class_var_declarations = 52,    /* class_var_declarations  */
  YYSYMBOL_class_var_declaration_list = 53, /* class_var_declaration_list  */
  YYSYMBOL_class_fct_declarations = 54,    /* class_fct_declarations  */
  YYSYMBOL_class_fct_decl = 55,            /* class_fct_decl  */
  YYSYMBOL_variable_block = 56,            /* variable_block  */
  YYSYMBOL_var_declarations = 57,          /* var_declarations  */
  YYSYMBOL_var_decl_type = 58,             /* var_decl_type  */
  YYSYMBOL_var_decl = 59,                  /* var_decl  */
  YYSYMBOL_var_decl_with_assignment = 60,  /* var_decl_with_assignment  */
  YYSYMBOL_value_list = 61,                /* value_list  */
  YYSYMBOL_value_type = 62,                /* value_type  */
  YYSYMBOL_function_block = 63,            /* function_block  */
  YYSYMBOL_fct_declarations = 64,          /* fct_declarations  */
  YYSYMBOL_fct_decl = 65,                  /* fct_decl  */
  YYSYMBOL_antet = 66,                     /* antet  */
  YYSYMBOL_list_param = 67,                /* list_param  */
  YYSYMBOL_param = 68,                     /* param  */
  YYSYMBOL_type = 69,                      /* type  */
  YYSYMBOL_before_list = 70,               /* before_list  */
  YYSYMBOL_block = 71,                     /* block  */
  YYSYMBOL_list = 72,                      /* list  */
  YYSYMBOL_statement = 73,                 /* statement  */
  YYSYMBOL_predefined = 74,                /* predefined  */
  YYSYMBOL_if = 75,                        /* if  */
  YYSYMBOL_while = 76,                     /* while  */
  YYSYMBOL_for = 77,                       /* for  */
  YYSYMBOL_for_incrementation = 78,        /* for_incrementation  */
  YYSYMBOL_function_call = 79,             /* function_call  */
  YYSYMBOL_call_list = 80,                 /* call_list  */
  YYSYMBOL_call_list_element = 81,         /* call_list_element  */
  YYSYMBOL_expression = 82                 /* expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   433

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  115
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  273

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      42,    43,    32,    30,    41,    31,    44,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,    37,
      39,     2,    40,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    34
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   277,   277,   280,   285,   286,   289,   290,   293,   316,
     319,   320,   323,   324,   328,   329,   332,   333,   338,   346,
     349,   350,   353,   354,   357,   358,   361,   365,   369,   375,
     376,   385,   386,   387,   388,   393,   401,   404,   405,   408,
     449,   465,   481,   497,   513,   529,   549,   550,   559,   565,
     571,   579,   589,   592,   599,   600,   603,   613,   614,   617,
     632,   651,   665,   679,   680,   681,   682,   683,   684,   687,
     723,   741,   742,   743,   744,   745,   746,   749,   750,   753,
     766,   781,   796,   824,   856,   857,   866,   870,   876,   883,
     887,   894,   898,   902,   906,   933,   935,   937,   939,   941,
     943,   945,   947,   949,   951,   953,   955,   957,   958,   962,
     976,   980,   984,   990,   994,   998
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ASSIGN", "VARDCL",
  "FCTDCL", "CONST", "MAIN", "IF", "FOR", "WHILE", "EVAL", "TYPEOF",
  "ELSE", "LT", "LE", "GT", "GE", "EQ", "OR", "AND", "CLSDCL", "CLASS",
  "FUNCTION", "ID", "TYPE", "NR", "CHAR", "STRING", "BOOLEAN", "'+'",
  "'-'", "'*'", "'/'", "NOT", "'{'", "'}'", "';'", "':'", "'<'", "'>'",
  "','", "'('", "')'", "'.'", "$accept", "progr", "declarations_block",
  "custom_block", "class_declarations", "class_declaration", "class_body",
  "class_var_declarations", "class_var_declaration_list",
  "class_fct_declarations", "class_fct_decl", "variable_block",
  "var_declarations", "var_decl_type", "var_decl",
  "var_decl_with_assignment", "value_list", "value_type", "function_block",
  "fct_declarations", "fct_decl", "antet", "list_param", "param", "type",
  "before_list", "block", "list", "statement", "predefined", "if", "while",
  "for", "for_incrementation", "function_call", "call_list",
  "call_list_element", "expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-196)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      11,    57,    97,   102,   107,   112,  -196,    86,  -196,   114,
     136,   123,    68,   127,   116,   129,   142,  -196,   150,  -196,
     134,  -196,   156,   160,   162,   163,   167,     4,  -196,  -196,
    -196,   170,   116,   155,  -196,  -196,  -196,  -196,  -196,    18,
      29,   175,   190,   190,  -196,   306,   192,   306,   306,   306,
     109,    14,   188,    35,   193,  -196,   187,  -196,   205,  -196,
     200,  -196,   214,    70,   202,  -196,   213,   129,   190,   204,
     106,  -196,  -196,  -196,   306,   306,  -196,    69,   260,    99,
     143,   164,  -196,   344,   239,   262,   226,   126,  -196,  -196,
    -196,  -196,   -20,  -196,     5,  -196,   231,  -196,   230,  -196,
     236,  -196,  -196,  -196,   129,   268,   269,  -196,   278,   283,
    -196,   237,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   275,   285,   277,  -196,  -196,   310,   261,
       7,   301,    35,   304,    35,  -196,   306,    35,   291,    36,
    -196,   297,  -196,  -196,   296,    81,  -196,   212,   212,   212,
     212,   212,   364,   371,    49,    49,  -196,  -196,    16,   302,
     179,   261,  -196,  -196,  -196,  -196,  -196,   306,   316,   298,
      62,  -196,  -196,   344,   113,   128,   311,   303,   324,  -196,
    -196,   318,   332,   211,   306,  -196,   250,  -196,   344,   354,
    -196,  -196,  -196,    13,   321,   330,   340,   348,   343,   355,
     378,   336,  -196,   357,   176,  -196,  -196,  -196,   353,     8,
     358,  -196,  -196,   267,   365,   375,   261,  -196,   367,   381,
     372,   366,     9,  -196,   273,   284,   380,   368,   376,   373,
      30,   382,   116,   383,   388,   384,   377,  -196,  -196,   290,
     385,   386,  -196,   261,   176,  -196,   116,   387,   116,   389,
     116,   390,  -196,  -196,   307,  -196,  -196,   391,  -196,   392,
     116,   393,   116,  -196,   313,  -196,  -196,   394,  -196,   395,
    -196,  -196,  -196
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     0,    19,     0,     1,     0,     2,     0,
      36,     0,     0,     0,    55,     0,     0,     3,     0,     4,
       0,     6,     0,     0,     0,     0,     0,     0,    68,    22,
      23,     0,    54,     0,    67,    64,    65,    66,    63,     0,
       0,     0,     0,    15,     7,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,    56,     0,    57,     0,    18,
       0,    20,     0,     0,     0,    39,     0,    11,    14,     0,
     111,   109,   110,   108,     0,     0,   113,     0,     0,     0,
       0,     0,    62,    59,     0,    24,     0,    86,    89,    91,
      92,    93,     0,    84,     0,    58,     0,    21,     0,    35,
       0,    37,     8,     9,    10,     0,     0,    16,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,     0,     0,
       0,     0,    94,     0,    94,    82,     0,    94,     0,     0,
      38,     0,    12,    17,     0,   112,   107,   100,   101,   102,
     103,    99,   104,   105,    95,    96,    97,    98,     0,     0,
       0,     0,    34,    32,    33,    31,    26,     0,     0,     0,
       0,    87,    85,    61,     0,     0,     0,     0,    46,    13,
     114,     0,    71,     0,     0,    77,     0,    27,    60,    25,
      88,    90,    83,     0,     0,     0,     0,     0,     0,     0,
      72,     0,    78,     0,     0,    53,    52,    48,     0,     0,
       0,    47,   115,     0,     0,     0,     0,    49,     0,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,    29,
       0,     0,    55,     0,     0,     0,     0,    74,    75,     0,
       0,     0,    28,     0,     0,    50,    55,     0,    55,     0,
      55,     0,    76,    81,     0,    30,    51,     0,    43,     0,
      55,     0,    55,    79,     0,    44,    45,     0,    40,     0,
      80,    41,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -196,  -196,  -196,  -196,  -196,   396,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,    -9,  -196,  -196,   173,  -117,  -196,  -196,
     359,   -27,   229,  -196,  -195,  -152,  -196,  -147,   -31,  -196,
    -196,  -196,  -196,  -196,   -14,  -115,   299,   -45
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,    12,    13,    66,   103,   104,    67,
      68,    10,    40,    28,    29,    30,   228,   229,    17,    63,
      64,    65,   177,   178,   207,    31,     8,    32,    33,    34,
      35,    36,    37,   227,    76,    92,    93,    77
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    56,    79,    80,    81,    83,    41,    50,   136,   217,
     167,   183,   166,   186,   219,   234,    69,   170,    38,   204,
      84,   134,   174,   135,    22,    23,    24,    25,    26,   110,
     111,    60,     1,   220,   235,   245,   244,   205,   206,    85,
      27,   106,    51,    52,   187,   168,    53,   137,    54,   256,
     221,   236,   182,    39,   205,   206,    51,    58,   105,    87,
     175,    88,    89,    90,    91,    59,   224,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   239,   176,
     247,   121,   122,   112,   113,   114,   115,   116,   117,   118,
      11,   173,     5,    62,   257,   141,   259,     6,   261,   119,
     120,   121,   122,   134,    19,   191,    99,   264,   267,     7,
     269,     9,   123,   112,   113,   114,   115,   116,   117,   118,
     181,    14,   188,   137,    22,    23,    24,    25,    26,   119,
     120,   121,   122,    70,    11,    71,    72,    82,    73,   201,
      27,    16,   125,    74,    38,   108,    38,    18,    53,    15,
     109,    75,    56,    39,   134,    56,   192,   112,   113,   114,
     115,   116,   117,   118,    21,   131,   193,   194,   132,    38,
     133,    44,    38,   119,   120,   121,   122,    42,   112,   113,
     114,   115,   116,   117,   118,    43,   126,    22,    23,    24,
      25,    26,    57,    56,   119,   120,   121,   122,    45,    38,
     205,   206,    46,    27,    47,    48,    55,   127,    56,    49,
      38,    38,    61,    62,    86,   185,    78,    94,    38,    22,
      23,    24,    25,    26,    95,    38,    -1,    -1,    -1,    -1,
      -1,    96,    38,    56,    38,    27,    38,    97,    98,   101,
      38,   107,   119,   120,   121,   122,    38,   200,    38,   102,
      38,   112,   113,   114,   115,   116,   117,   118,    22,    23,
      24,    25,    26,   124,   128,   129,   130,   119,   120,   121,
     122,   138,   139,   140,    27,    22,    23,    24,    25,    26,
     146,    22,    23,    24,    25,    26,   202,   162,   163,   164,
     165,    27,    22,    23,    24,    25,    26,    27,    22,    23,
      24,    25,    26,   223,   144,   142,   143,   145,    27,   237,
     158,   159,   160,   161,    27,    22,    23,    24,    25,    26,
     238,    22,    23,    24,    25,    26,   252,   169,   171,   168,
      70,    27,    71,    72,   179,    73,   180,    27,   190,   184,
      74,   189,   195,   263,   198,   199,   196,   208,    75,   270,
     112,   113,   114,   115,   116,   117,   118,   203,   112,   113,
     114,   115,   116,   117,   118,   197,   119,   120,   121,   122,
     209,   210,   175,   215,   119,   120,   121,   122,   112,   113,
     114,   115,   116,   212,   118,   112,   113,   114,   115,   116,
     213,   214,   216,   218,   119,   120,   121,   122,   222,   226,
     225,   119,   120,   121,   122,   230,   231,   232,    20,   233,
     240,   241,   242,   249,   243,   253,   255,   246,   248,   250,
     251,   254,   100,   258,   260,   262,   211,   265,   266,   268,
     271,   272,     0,   172
};

static const yytype_int16 yycheck[] =
{
      14,    32,    47,    48,    49,    50,    15,     3,     3,   204,
       3,   158,   129,   160,     6,     6,    43,   132,    32,     6,
       6,    41,   137,    43,     8,     9,    10,    11,    12,    74,
      75,    40,    21,    25,    25,   230,     6,    24,    25,    25,
      24,    68,    38,    39,   161,    38,    42,    42,    44,   244,
      42,    42,    36,    24,    24,    25,    38,    39,    67,    24,
      24,    26,    27,    28,    29,    36,   213,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   225,    43,
     232,    32,    33,    14,    15,    16,    17,    18,    19,    20,
      22,   136,    35,    23,   246,   104,   248,     0,   250,    30,
      31,    32,    33,    41,    36,    43,    36,   254,   260,     7,
     262,     4,    43,    14,    15,    16,    17,    18,    19,    20,
      39,    35,   167,    42,     8,     9,    10,    11,    12,    30,
      31,    32,    33,    24,    22,    26,    27,    28,    29,   184,
      24,     5,    43,    34,   158,    39,   160,    24,    42,    35,
      44,    42,   183,    24,    41,   186,    43,    14,    15,    16,
      17,    18,    19,    20,    37,    39,    38,    39,    42,   183,
      44,    37,   186,    30,    31,    32,    33,    35,    14,    15,
      16,    17,    18,    19,    20,    35,    43,     8,     9,    10,
      11,    12,    37,   224,    30,    31,    32,    33,    42,   213,
      24,    25,    42,    24,    42,    42,    36,    43,   239,    42,
     224,   225,    37,    23,    26,    36,    24,    24,   232,     8,
       9,    10,    11,    12,    37,   239,    14,    15,    16,    17,
      18,    26,   246,   264,   248,    24,   250,    37,    24,    37,
     254,    37,    30,    31,    32,    33,   260,    36,   262,    36,
     264,    14,    15,    16,    17,    18,    19,    20,     8,     9,
      10,    11,    12,     3,    25,     3,    40,    30,    31,    32,
      33,    40,    42,    37,    24,     8,     9,    10,    11,    12,
      43,     8,     9,    10,    11,    12,    36,    26,    27,    28,
      29,    24,     8,     9,    10,    11,    12,    24,     8,     9,
      10,    11,    12,    36,    26,    37,    37,    24,    24,    36,
      35,    26,    35,     3,    24,     8,     9,    10,    11,    12,
      36,     8,     9,    10,    11,    12,    36,    26,    24,    38,
      24,    24,    26,    27,    37,    29,    40,    24,    40,    37,
      34,    25,    31,    36,    26,    13,    43,    26,    42,    36,
      14,    15,    16,    17,    18,    19,    20,     3,    14,    15,
      16,    17,    18,    19,    20,    41,    30,    31,    32,    33,
      40,    31,    24,    37,    30,    31,    32,    33,    14,    15,
      16,    17,    18,    40,    20,    14,    15,    16,    17,    18,
      35,    13,    35,    40,    30,    31,    32,    33,    40,    24,
      35,    30,    31,    32,    33,    38,    25,    35,    12,    43,
      30,    43,    36,    25,    41,    30,   243,    35,    35,    35,
      43,    35,    63,    36,    35,    35,   197,    36,    36,    36,
      36,    36,    -1,   134
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    21,    46,    47,    48,    35,     0,     7,    71,     4,
      56,    22,    49,    50,    35,    35,     5,    63,    24,    36,
      50,    37,     8,     9,    10,    11,    12,    24,    58,    59,
      60,    70,    72,    73,    74,    75,    76,    77,    79,    24,
      57,    58,    35,    35,    37,    42,    42,    42,    42,    42,
       3,    38,    39,    42,    44,    36,    73,    37,    39,    36,
      58,    37,    23,    64,    65,    66,    51,    54,    55,    66,
      24,    26,    27,    29,    34,    42,    79,    82,    24,    82,
      82,    82,    28,    82,     6,    25,    26,    24,    26,    27,
      28,    29,    80,    81,    24,    37,    26,    37,    24,    36,
      65,    37,    36,    52,    53,    58,    66,    37,    39,    44,
      82,    82,    14,    15,    16,    17,    18,    19,    20,    30,
      31,    32,    33,    43,     3,    43,    43,    43,    25,     3,
      40,    39,    42,    44,    41,    43,     3,    42,    40,    42,
      37,    58,    37,    37,    26,    24,    43,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    35,    26,
      35,     3,    26,    27,    28,    29,    62,     3,    38,    26,
      80,    24,    81,    82,    80,    24,    43,    67,    68,    37,
      40,    39,    36,    72,    37,    36,    72,    62,    82,    25,
      40,    43,    43,    38,    39,    31,    43,    41,    26,    13,
      36,    82,    36,     3,     6,    24,    25,    69,    26,    40,
      31,    67,    40,    35,    13,    37,    35,    69,    40,     6,
      25,    42,    40,    36,    72,    35,    24,    78,    61,    62,
      38,    25,    35,    43,     6,    25,    42,    36,    36,    72,
      30,    43,    36,    41,     6,    69,    35,    70,    35,    25,
      35,    43,    36,    30,    35,    61,    69,    70,    36,    70,
      35,    70,    35,    36,    72,    36,    36,    70,    36,    70,
      36,    36,    36
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    47,    48,    48,    49,    49,    50,    51,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    59,    59,    60,    60,    60,    61,
      61,    62,    62,    62,    62,    63,    63,    64,    64,    65,
      66,    66,    66,    66,    66,    66,    67,    67,    68,    68,
      68,    68,    69,    69,    70,    70,    71,    72,    72,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    74,
      74,    75,    75,    75,    75,    75,    75,    76,    76,    77,
      77,    78,    79,    79,    80,    80,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     3,     4,     0,     2,     3,     5,     2,
       1,     0,     2,     3,     1,     0,     2,     3,     4,     0,
       2,     3,     1,     1,     3,     6,     5,     6,    10,     1,
       3,     1,     1,     1,     1,     4,     0,     2,     3,     1,
      11,    12,    12,    10,    11,    11,     1,     3,     3,     4,
       6,     7,     1,     1,     1,     0,     4,     2,     3,     3,
       6,     5,     3,     1,     1,     1,     1,     1,     1,     4,
       4,     6,     7,     9,    10,    10,    11,     6,     7,    12,
      13,     3,     4,     6,     1,     3,     1,     3,     4,     1,
       4,     1,     1,     1,     0,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     1,     1,
       1,     1,     3,     1,     4,     6
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* progr: declarations_block block  */
#line 277 "limbaj.y"
                                {printf("The programme is correct!\n");}
#line 1595 "limbaj.tab.c"
    break;

  case 8: /* class_declaration: CLASS ID '{' class_body '}'  */
#line 293 "limbaj.y"
                                                {
                        if(!vars.existsVar())
                            vars.add_family_block((yyvsp[-3].string));
                        else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                        if(!fct.existsFct())
                            fct.add_family_block((yyvsp[-3].string));
                        else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                        // if(!cls.existsCls($2)) {
                        //     cout << "CLASS ID" << indexx++ << endl;
                        //     cls.class_index++;
                        //     cls.addCls($2, strdup(""));
                        // } else {
                        //     yyerror("Class already declared");
                        // }
                    }
#line 1621 "limbaj.tab.c"
    break;

  case 18: /* variable_block: VARDCL '{' var_declarations '}'  */
#line 338 "limbaj.y"
                                                 {
                    if(!vars.existsVar())
                        vars.add_family_block("global");
                    else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                }
#line 1634 "limbaj.tab.c"
    break;

  case 24: /* var_decl: ID ':' TYPE  */
#line 357 "limbaj.y"
                         { vars.addVar((yyvsp[0].string),(yyvsp[-2].string),true,1);}
#line 1640 "limbaj.tab.c"
    break;

  case 25: /* var_decl: ID '<' NR '>' ':' TYPE  */
#line 358 "limbaj.y"
                                   { vars.addVar((yyvsp[0].string),(yyvsp[-5].string),true,std::atoi((yyvsp[-3].string)));}
#line 1646 "limbaj.tab.c"
    break;

  case 26: /* var_decl_with_assignment: ID ':' TYPE ASSIGN value_type  */
#line 361 "limbaj.y"
                                                           { 
                vars.addVar((yyvsp[-2].string),(yyvsp[-4].string),true,1);
                vars.add_value((yyvsp[-4].string),(yyvsp[0].string));
            }
#line 1655 "limbaj.tab.c"
    break;

  case 27: /* var_decl_with_assignment: ID ':' CONST TYPE ASSIGN value_type  */
#line 365 "limbaj.y"
                                                 { 
                vars.addVar((yyvsp[-2].string),(yyvsp[-5].string),false,1);
                vars.add_value((yyvsp[-5].string),(yyvsp[0].string));
            }
#line 1664 "limbaj.tab.c"
    break;

  case 28: /* var_decl_with_assignment: ID '<' NR '>' ':' TYPE ASSIGN '{' value_list '}'  */
#line 369 "limbaj.y"
                                                              { 
                vars.addVar((yyvsp[-4].string),(yyvsp[-9].string),true,std::atoi((yyvsp[-7].string)));
                vars.add_value((yyvsp[-9].string),(yyvsp[-1].string));
            }
#line 1673 "limbaj.tab.c"
    break;

  case 29: /* value_list: value_type  */
#line 375 "limbaj.y"
                        { (yyval.string) = (yyvsp[0].string);}
#line 1679 "limbaj.tab.c"
    break;

  case 30: /* value_list: value_type ',' value_list  */
#line 376 "limbaj.y"
                                       {
                (yyval.string) = (char*)malloc(strlen((yyvsp[-2].string)) + strlen((yyvsp[0].string)) + 1); 
                strcpy((yyval.string), (yyvsp[-2].string));
                strcat((yyval.string), " ");
                strcat((yyval.string),(yyvsp[0].string));
                free((yyvsp[-2].string));
            }
#line 1691 "limbaj.tab.c"
    break;

  case 35: /* function_block: FCTDCL '{' fct_declarations '}'  */
#line 393 "limbaj.y"
                                                 {
                    if(!fct.existsFct())
                        fct.add_family_block("global");
                    else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                }
#line 1704 "limbaj.tab.c"
    break;

  case 40: /* antet: FUNCTION ID '(' list_param ')' '-' '>' TYPE '{' before_list '}'  */
#line 449 "limbaj.y"
                                                                       { 
               if(fct.addFct((yyvsp[-3].string),(yyvsp[-9].string),strdup(""),(yyvsp[-7].string),true,vars, [](const std::vector<double>& args) -> ValueType {
                    //Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block((yyvsp[-9].string));
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
#line 1725 "limbaj.tab.c"
    break;

  case 41: /* antet: FUNCTION ID '(' list_param ')' '-' '>' CONST TYPE '{' before_list '}'  */
#line 465 "limbaj.y"
                                                                                  { 
               if(fct.addFct((yyvsp[-3].string),(yyvsp[-10].string),strdup(""),(yyvsp[-8].string),false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block((yyvsp[-10].string));
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
#line 1746 "limbaj.tab.c"
    break;

  case 42: /* antet: FUNCTION ID '(' list_param ')' '-' '>' '(' ')' '{' before_list '}'  */
#line 481 "limbaj.y"
                                                                               { 
            if(fct.addFct("void",(yyvsp[-10].string),strdup(""),(yyvsp[-8].string),false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block((yyvsp[-10].string));
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
#line 1767 "limbaj.tab.c"
    break;

  case 43: /* antet: FUNCTION ID '(' ')' '-' '>' TYPE '{' before_list '}'  */
#line 497 "limbaj.y"
                                                               { 
               if(fct.addFct((yyvsp[-3].string),(yyvsp[-8].string),strdup(""),"",true,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block((yyvsp[-8].string));
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
#line 1788 "limbaj.tab.c"
    break;

  case 44: /* antet: FUNCTION ID '(' ')' '-' '>' CONST TYPE '{' before_list '}'  */
#line 513 "limbaj.y"
                                                                     {
          if(fct.addFct((yyvsp[-3].string),(yyvsp[-9].string),strdup(""),"",false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block((yyvsp[-9].string));
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
#line 1809 "limbaj.tab.c"
    break;

  case 45: /* antet: FUNCTION ID '(' ')' '-' '>' '(' ')' '{' before_list '}'  */
#line 529 "limbaj.y"
                                                                  { 
               if(fct.addFct("void",(yyvsp[-9].string),strdup(""),"",false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block((yyvsp[-9].string));
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
#line 1830 "limbaj.tab.c"
    break;

  case 46: /* list_param: param  */
#line 549 "limbaj.y"
                   { (yyval.string) = (yyvsp[0].string); }
#line 1836 "limbaj.tab.c"
    break;

  case 47: /* list_param: param ',' list_param  */
#line 550 "limbaj.y"
                                   { 
               (yyval.string) = (char*)malloc(strlen((yyvsp[-2].string)) + 1 + strlen((yyvsp[0].string)) + 1);
               strcpy((yyval.string), (yyvsp[-2].string));
               strcat((yyval.string), ",");
               strcat((yyval.string), (yyvsp[0].string));
               free((yyvsp[0].string));
            }
#line 1848 "limbaj.tab.c"
    break;

  case 48: /* param: ID ':' type  */
#line 559 "limbaj.y"
                     { 
          (yyval.string) = (char*)malloc(strlen((yyvsp[-2].string)) + 10 + strlen((yyvsp[0].string)) + 1); 
          strcpy((yyval.string), (yyvsp[-2].string));
          strcat((yyval.string), ":mutable ");
          strcat((yyval.string), (yyvsp[0].string));
      }
#line 1859 "limbaj.tab.c"
    break;

  case 49: /* param: ID ':' CONST type  */
#line 565 "limbaj.y"
                           {
          (yyval.string) = (char*)malloc(strlen((yyvsp[-3].string)) + 2 + strlen((yyvsp[0].string)) + 1);
          strcpy((yyval.string), (yyvsp[-3].string));
          strcat((yyval.string), ":");
          strcat((yyval.string), (yyvsp[0].string));
      }
#line 1870 "limbaj.tab.c"
    break;

  case 50: /* param: ID '<' NR '>' ':' type  */
#line 571 "limbaj.y"
                              { 
          (yyval.string) = (char*)malloc(strlen((yyvsp[-5].string)) + 12 + strlen((yyvsp[-3].string)) + strlen((yyvsp[0].string)) + 1); 
          strcpy((yyval.string), (yyvsp[-5].string));
          strcat((yyval.string), "<");
          strcat((yyval.string),(yyvsp[-3].string));
          strcat((yyval.string),">:mutable ");
          strcat((yyval.string), (yyvsp[0].string));
           }
#line 1883 "limbaj.tab.c"
    break;

  case 51: /* param: ID '<' NR '>' ':' CONST type  */
#line 579 "limbaj.y"
                                    { 
          (yyval.string) = (char*)malloc(strlen((yyvsp[-6].string)) + 5 + strlen((yyvsp[-4].string)) + strlen((yyvsp[0].string)) + 1);
          strcpy((yyval.string), (yyvsp[-6].string));
          strcat((yyval.string), "<");
          strcat((yyval.string),(yyvsp[-4].string));
          strcat((yyval.string),">: ");
          strcat((yyval.string), (yyvsp[0].string));
          }
#line 1896 "limbaj.tab.c"
    break;

  case 52: /* type: TYPE  */
#line 589 "limbaj.y"
            { (yyval.string) = (char*)malloc(strlen((yyvsp[0].string)) + 1); 
            strcpy((yyval.string), (yyvsp[0].string));
            }
#line 1904 "limbaj.tab.c"
    break;

  case 53: /* type: ID  */
#line 592 "limbaj.y"
          {(yyval.string) = (char*)malloc(strlen((yyvsp[0].string)) + 1); 
            strcpy((yyval.string), (yyvsp[0].string));
            }
#line 1912 "limbaj.tab.c"
    break;

  case 56: /* block: MAIN '{' before_list '}'  */
#line 603 "limbaj.y"
                                 { if(!vars.existsVar())
                        vars.add_family_block("main");
                        else{
                                yyerror("Variable already declared");
                                exit(1);
                            }
                        }
#line 1924 "limbaj.tab.c"
    break;

  case 59: /* statement: ID ASSIGN expression  */
#line 617 "limbaj.y"
                                { 

                                string decoy = string((yyvsp[-2].string));

                                if(TypeOf(decoy) != TypeOf(Typeof_expression))
                                {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_value((yyvsp[-2].string),(yyvsp[0].node)->Eval(),TypeOf(decoy),strdup(""),"0");
                                }
                                    Typeof_expression.clear();
                                }
#line 1944 "limbaj.tab.c"
    break;

  case 60: /* statement: ID '<' NR '>' ASSIGN expression  */
#line 632 "limbaj.y"
                                           { 

                                string decoy = string((yyvsp[-5].string));
                                decoy += "<";
                                decoy += string((yyvsp[-3].string));
                                decoy += ">";


                                if(TypeOf(decoy) != TypeOf(Typeof_expression))
                                {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_value((yyvsp[-5].string),(yyvsp[0].node)->Eval(),TypeOf(decoy),strdup(""),(yyvsp[-3].string));
                                }
                                    Typeof_expression.clear();
                                }
#line 1968 "limbaj.tab.c"
    break;

  case 61: /* statement: ID '.' ID ASSIGN expression  */
#line 651 "limbaj.y"
                                       { 
                                string decoy = string((yyvsp[-2].string));

                                if(TypeOf(decoy) != TypeOf(Typeof_expression))
                                 {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_value((yyvsp[-2].string),(yyvsp[0].node)->Eval(),TypeOf(decoy),(yyvsp[-4].string),"0");
                                }
                                    Typeof_expression.clear();
                                }
#line 1987 "limbaj.tab.c"
    break;

  case 62: /* statement: ID ASSIGN STRING  */
#line 665 "limbaj.y"
                            { 
                                string decoy = string((yyvsp[-2].string));

                                if(TypeOf(decoy) != "multicei")
                                {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_string((yyvsp[-2].string),(yyvsp[0].string),strdup(""));
                                }
                                    
                                }
#line 2006 "limbaj.tab.c"
    break;

  case 64: /* statement: if  */
#line 680 "limbaj.y"
                    {Typeof_expression.clear();}
#line 2012 "limbaj.tab.c"
    break;

  case 65: /* statement: while  */
#line 681 "limbaj.y"
                    {Typeof_expression.clear();}
#line 2018 "limbaj.tab.c"
    break;

  case 66: /* statement: for  */
#line 682 "limbaj.y"
                    {Typeof_expression.clear();}
#line 2024 "limbaj.tab.c"
    break;

  case 69: /* predefined: EVAL '(' expression ')'  */
#line 687 "limbaj.y"
                                     { 

                char dec[100];
                string string_value;

                if(TypeOf(Typeof_expression) == "basic")
                {
                    sprintf(dec,"%.0f",(yyvsp[-1].node)->Eval());
                    string_value = string(dec);
                }
                else if(TypeOf(Typeof_expression) == "smart")
                {
                    sprintf(dec,"%.5f",(yyvsp[-1].node)->Eval());
                    string_value = string(dec);
                }
                else if(TypeOf(Typeof_expression) == "chatgpt")
                {
                    sprintf(dec,"%.5f",(yyvsp[-1].node)->Eval());
                    string_value = string(dec);
                }
                else if(TypeOf(Typeof_expression) == "singurel")
                {
                    char asciiChar = static_cast<char>((yyvsp[-1].node)->Eval());
                    string_value = asciiChar;
                }
                else if(TypeOf(Typeof_expression) == "maybe")
                {
                    if((yyvsp[-1].node)->Eval() == 1)
                        string_value = "true";
                    else
                        string_value = "false";
                }

                cout << string_value << endl;

                Typeof_expression.clear();}
#line 2065 "limbaj.tab.c"
    break;

  case 70: /* predefined: TYPEOF '(' expression ')'  */
#line 723 "limbaj.y"
                                       {  cout << TypeOf(Typeof_expression) << endl;Typeof_expression.clear();}
#line 2071 "limbaj.tab.c"
    break;

  case 79: /* for: FOR '(' ID ASSIGN NR ';' expression ';' for_incrementation ')' '{' '}'  */
#line 753 "limbaj.y"
                                                                            {
                    bool gasit = false;
                    for(auto x:vars.vars)
                    {
                        if(x.name == string((yyvsp[-9].string)))
                        gasit = true; 
                    }
                    if(!gasit)
                    {
                        yyerror("The variable wasn't declared");
                        exit(1);
                    }
    }
#line 2089 "limbaj.tab.c"
    break;

  case 80: /* for: FOR '(' ID ASSIGN NR ';' expression ';' for_incrementation ')' '{' list '}'  */
#line 766 "limbaj.y"
                                                                                 {
                    bool gasit = false;
                    for(auto x:vars.vars)
                    {
                        if(x.name == string((yyvsp[-10].string)))
                        gasit = true; 
                    }
                    if(!gasit)
                    {
                        yyerror("The variable wasn't declared");
                        exit(1);
                    }
    }
#line 2107 "limbaj.tab.c"
    break;

  case 81: /* for_incrementation: ID '+' '+'  */
#line 781 "limbaj.y"
                                {
                    bool gasit = false;
                    for(auto x:vars.vars)
                    {
                        if(x.name == string((yyvsp[-2].string)))
                        gasit = true; 
                    }
                    if(!gasit)
                    {
                        yyerror("The variable wasn't declared");
                        exit(1);
                    }
    }
#line 2125 "limbaj.tab.c"
    break;

  case 82: /* function_call: ID '(' call_list ')'  */
#line 796 "limbaj.y"
                                     {
                            std::vector<Node*> decoy;

                            if(fct.existsWasDeclared((yyvsp[-3].string)) == false)
                            {
                                yyerror("Function wasnt declared");
                                exit(1);
                            }
                            if(fct.call_has_same_params((yyvsp[-3].string),(yyvsp[-1].string),"",vars) == -1)
                            {
                                yyerror("Function call doesn't have the same number of parameters as the declared function");
                                exit(1);
                            }
                            else if(fct.call_has_same_params((yyvsp[-3].string),(yyvsp[-1].string),"",vars) == -2)
                            {
                                yyerror("Function call contains a variable that hasnt been declared");
                                exit(1);
                            }
                            else if(fct.call_has_same_params((yyvsp[-3].string),(yyvsp[-1].string),"",vars) == -3)
                            {
                                yyerror("Function call has different types of variables in the call from the declared ones");
                                exit(1);
                            }
                            (yyval.node) = new Node(FUNCTION_CALL, UNDEFINED, (yyvsp[-3].string), decoy);
                            Typeof_expression += (yyvsp[-3].string);
                            Typeof_expression += "("; 
                            Typeof_expression += ") ";  
                        }
#line 2158 "limbaj.tab.c"
    break;

  case 83: /* function_call: ID '.' ID '(' call_list ')'  */
#line 824 "limbaj.y"
                                            {
                            std::vector<Node*> decoy;
                            if(fct.existsWasDeclared((yyvsp[-3].string)) == false)
                            {
                                yyerror("Function wasnt declared");
                                exit(1);
                            }
                            if(fct.call_has_same_params((yyvsp[-3].string),(yyvsp[-1].string),(yyvsp[-5].string),vars) == -1)
                            {
                                yyerror("Function call doesn't have the same number of parameters as the declared function");
                                exit(1);
                            }
                            else if(fct.call_has_same_params((yyvsp[-3].string),(yyvsp[-1].string),(yyvsp[-5].string),vars) == -2)
                            {
                                yyerror("Function call contains a variable that hasnt been declared");
                                exit(1);
                            }
                            else if(fct.call_has_same_params((yyvsp[-3].string),(yyvsp[-1].string),(yyvsp[-5].string),vars) == -3)
                            {
                                yyerror("Function call has different types of variables in the call from the declared ones");
                                exit(1);
                            }
                            (yyval.node) = new Node(FUNCTION_CALL, UNDEFINED, (yyvsp[-3].string), decoy);
                            Typeof_expression += (yyvsp[-5].string);
                            Typeof_expression += ".";
                            Typeof_expression += (yyvsp[-3].string);
                            Typeof_expression += "("; 
                            Typeof_expression += ") "; 
                        }
#line 2192 "limbaj.tab.c"
    break;

  case 84: /* call_list: call_list_element  */
#line 856 "limbaj.y"
                              { (yyval.string) = (yyvsp[0].string); }
#line 2198 "limbaj.tab.c"
    break;

  case 85: /* call_list: call_list ',' call_list_element  */
#line 857 "limbaj.y"
                                             { 
               (yyval.string) = (char*)malloc(strlen((yyvsp[-2].string)) + 1 + strlen((yyvsp[0].string)) + 1);
               strcpy((yyval.string), (yyvsp[-2].string));
               strcat((yyval.string), ",");
               strcat((yyval.string), (yyvsp[0].string));
               free((yyvsp[0].string));
            }
#line 2210 "limbaj.tab.c"
    break;

  case 86: /* call_list_element: ID  */
#line 866 "limbaj.y"
                       {
                    (yyval.string) = (char*)malloc(strlen((yyvsp[0].string)) + 1); 
                    strcpy((yyval.string), (yyvsp[0].string));
                  }
#line 2219 "limbaj.tab.c"
    break;

  case 87: /* call_list_element: ID '.' ID  */
#line 870 "limbaj.y"
                              {   
                    (yyval.string) = (char*)malloc(strlen((yyvsp[-2].string)) + strlen((yyvsp[0].string)) + 2); 
                    strcpy((yyval.string), (yyvsp[-2].string));
                    strcat((yyval.string), ",");
                    strcat((yyval.string), (yyvsp[0].string));
                  }
#line 2230 "limbaj.tab.c"
    break;

  case 88: /* call_list_element: ID '<' NR '>'  */
#line 876 "limbaj.y"
                                  {   
                    (yyval.string) = (char*)malloc(strlen((yyvsp[-3].string)) + strlen((yyvsp[-1].string)) + 3); 
                    strcpy((yyval.string), (yyvsp[-3].string));
                    strcat((yyval.string), "<");
                    strcat((yyval.string), (yyvsp[-1].string));
                    strcat((yyval.string), ">");
                    }
#line 2242 "limbaj.tab.c"
    break;

  case 89: /* call_list_element: NR  */
#line 883 "limbaj.y"
                       { 
                    (yyval.string) = (char*)malloc(strlen((yyvsp[0].string)) + 1); 
                    strcpy((yyval.string), (yyvsp[0].string));
                    }
#line 2251 "limbaj.tab.c"
    break;

  case 90: /* call_list_element: ID '(' call_list ')'  */
#line 887 "limbaj.y"
                                         {
                    (yyval.string) = (char*)malloc(strlen((yyvsp[-3].string)) + 3); 
                    strcpy((yyval.string), (yyvsp[-3].string));
                    strcat((yyval.string), "(");
                    strcat((yyval.string), (yyvsp[-1].string));
                    strcat((yyval.string), ")");
                  }
#line 2263 "limbaj.tab.c"
    break;

  case 91: /* call_list_element: CHAR  */
#line 894 "limbaj.y"
                         {
                    (yyval.string) = (char*)malloc(strlen((yyvsp[0].string)) + 1); 
                    strcpy((yyval.string), (yyvsp[0].string));
                  }
#line 2272 "limbaj.tab.c"
    break;

  case 92: /* call_list_element: STRING  */
#line 898 "limbaj.y"
                           {
                    (yyval.string) = (char*)malloc(strlen((yyvsp[0].string)) + 1); 
                    strcpy((yyval.string), (yyvsp[0].string));
                  }
#line 2281 "limbaj.tab.c"
    break;

  case 93: /* call_list_element: BOOLEAN  */
#line 902 "limbaj.y"
                            {
                    (yyval.string) = (char*)malloc(5); 
                    strcpy((yyval.string), "true");
                  }
#line 2290 "limbaj.tab.c"
    break;

  case 94: /* call_list_element: %empty  */
#line 906 "limbaj.y"
                                {
                    (yyval.string) = (char*)malloc(2); 
                    strcpy((yyval.string), "");
                   }
#line 2299 "limbaj.tab.c"
    break;

  case 95: /* expression: expression '+' expression  */
#line 933 "limbaj.y"
                                        { /*$$ = $1 + $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "+");
                                        }
#line 2306 "limbaj.tab.c"
    break;

  case 96: /* expression: expression '-' expression  */
#line 935 "limbaj.y"
                                        { /*$$ = $1 - $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "-");
                                        }
#line 2313 "limbaj.tab.c"
    break;

  case 97: /* expression: expression '*' expression  */
#line 937 "limbaj.y"
                                        { /*$$ = $1 * $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "*");
                                        }
#line 2320 "limbaj.tab.c"
    break;

  case 98: /* expression: expression '/' expression  */
#line 939 "limbaj.y"
                                        { /*$$ = $1 / $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "/");
                                        }
#line 2327 "limbaj.tab.c"
    break;

  case 99: /* expression: expression EQ expression  */
#line 941 "limbaj.y"
                                        { /*$$ = $1 == $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "==");
                                        }
#line 2334 "limbaj.tab.c"
    break;

  case 100: /* expression: expression LT expression  */
#line 943 "limbaj.y"
                                        { /*$$ = $1 < $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "<");
                                        }
#line 2341 "limbaj.tab.c"
    break;

  case 101: /* expression: expression LE expression  */
#line 945 "limbaj.y"
                                        { /*$$ = $1 <= $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "<=");
                                        }
#line 2348 "limbaj.tab.c"
    break;

  case 102: /* expression: expression GT expression  */
#line 947 "limbaj.y"
                                        { /*$$ = $1 > $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), ">");
                                        }
#line 2355 "limbaj.tab.c"
    break;

  case 103: /* expression: expression GE expression  */
#line 949 "limbaj.y"
                                        { /*$$ = $1 >= $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), ">=");
                                        }
#line 2362 "limbaj.tab.c"
    break;

  case 104: /* expression: expression OR expression  */
#line 951 "limbaj.y"
                                        { /*$$ = $1 || $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "||");
                                        }
#line 2369 "limbaj.tab.c"
    break;

  case 105: /* expression: expression AND expression  */
#line 953 "limbaj.y"
                                        { /*$$ = $1 && $3;*/ (yyval.node) = new Node(OPERATOR, (yyvsp[-2].node), (yyvsp[0].node), "&&");
                                        }
#line 2376 "limbaj.tab.c"
    break;

  case 106: /* expression: NOT expression  */
#line 955 "limbaj.y"
                                        { /*$$ = !$2;*/ (yyval.node) = new Node(OPERATOR, nullptr, (yyvsp[0].node), "!");
                                        }
#line 2383 "limbaj.tab.c"
    break;

  case 107: /* expression: '(' expression ')'  */
#line 957 "limbaj.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2389 "limbaj.tab.c"
    break;

  case 108: /* expression: BOOLEAN  */
#line 958 "limbaj.y"
                                        { /*$$ = atoi($1);*/ (yyval.node) = new Node(VALUE, BOOL, (yyvsp[0].string), 0);
                                            Typeof_expression += (yyvsp[0].string);
                                            Typeof_expression += " ";
                                         }
#line 2398 "limbaj.tab.c"
    break;

  case 109: /* expression: NR  */
#line 962 "limbaj.y"
                                        { /*$$ = atoi($1);*/ 
          
                                            string mystring = string((yyvsp[0].string));
                                            size_t foundPos = mystring.find(".");

                                            if (foundPos != std::string::npos) {
                                                (yyval.node) = new Node(VALUE, FLOAT, (yyvsp[0].string), 0);
                                            } else {
                                                (yyval.node) = new Node(VALUE, INT, (yyvsp[0].string), 0);
                                            }

                                            Typeof_expression += (yyvsp[0].string);
                                            Typeof_expression += " ";
                                         }
#line 2417 "limbaj.tab.c"
    break;

  case 110: /* expression: CHAR  */
#line 976 "limbaj.y"
                                        { /*$$ = atoi($1);*/ (yyval.node) = new Node(VALUE, CHAR_TYPE, (yyvsp[0].string), 0);
                                            Typeof_expression += (yyvsp[0].string);
                                            Typeof_expression += " ";
                                        }
#line 2426 "limbaj.tab.c"
    break;

  case 111: /* expression: ID  */
#line 980 "limbaj.y"
                                        { /*$$ = 1;*/ (yyval.node) = new Node(IDENTIFIER, UNDEFINED, (yyvsp[0].string), 0);
                                            Typeof_expression += (yyvsp[0].string);
                                            Typeof_expression += " ";         
                                        }
#line 2435 "limbaj.tab.c"
    break;

  case 112: /* expression: ID '.' ID  */
#line 984 "limbaj.y"
                                        { /*$$ = 1;*/ (yyval.node) = new Node(IDENTIFIER, UNDEFINED, (yyvsp[0].string), (yyvsp[-2].string), 0); 
                                            Typeof_expression += (yyvsp[-2].string);
                                            Typeof_expression += "."; 
                                            Typeof_expression += (yyvsp[0].string);
                                            Typeof_expression += " ";             
                                        }
#line 2446 "limbaj.tab.c"
    break;

  case 113: /* expression: function_call  */
#line 990 "limbaj.y"
                                        {   (yyval.node) = (yyvsp[0].node);
                                            // Typeof_expression += $1;
                                            // Typeof_expression += " ";
                                        }
#line 2455 "limbaj.tab.c"
    break;

  case 114: /* expression: ID '<' NR '>'  */
#line 994 "limbaj.y"
                                        { /*$$ = 1;*/ (yyval.node) = new Node(IDENTIFIER, UNDEFINED, (yyvsp[-3].string), std::stoi((yyvsp[-1].string)));
                                            Typeof_expression += (yyvsp[-3].string);
                                            Typeof_expression += " ";         
                                        }
#line 2464 "limbaj.tab.c"
    break;

  case 115: /* expression: ID '.' ID '<' NR '>'  */
#line 998 "limbaj.y"
                                          {/*$$ = 1;*/ (yyval.node) = new Node(IDENTIFIER, UNDEFINED, (yyvsp[-3].string), (yyvsp[-5].string), std::stoi((yyvsp[-1].string))); 
                                            Typeof_expression += (yyvsp[-3].string);
                                            Typeof_expression += " ";  
                                        }
#line 2473 "limbaj.tab.c"
    break;


#line 2477 "limbaj.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1004 "limbaj.y"


#include "Node.h"
void yyerror(const char * s){
printf("error: %s at line:%d\n",s,yylineno);
}
// Forward declaration of print functions for each table

int main(int argc, char** argv){
    yyin = fopen(argv[1], "r");
    yyparse();

    vars.printVars();
    fct.printFct();


    return 0; 
}


