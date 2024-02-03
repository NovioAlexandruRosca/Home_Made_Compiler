%{
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

%}
%code requires {
    #include <vector>
    #include "Node.h" // Assuming Node is defined in Node.h
}
%union {
    char* string;
    int intt;
    Node* node;
    std::vector<Node*>* node_vector; 
}



%token ASSIGN VARDCL FCTDCL CONST MAIN IF FOR WHILE EVAL TYPEOF ELSE
%token LT LE GT GE EQ OR AND CLSDCL CLASS FUNCTION 
%token<string> ID TYPE NR CHAR STRING BOOLEAN
%type <string> param list_param call_list call_list_element
%type <string> type value_type value_list

%type <node> expression function_call 

%left OR
%left AND
%nonassoc LT LE GT GE EQ
%left '+' '-'
%left '*' '/'
%right NOT

%start progr
%%
progr: declarations_block block {printf("The programme is correct!\n");}
     ;

declarations_block : custom_block variable_block function_block
                   ;

////////////////// CUSTOM DECLARATION BLOCK //////////////////////

custom_block : CLSDCL '{' class_declarations '}'
             | /* epsilon */ 
             ;

class_declarations :  class_declaration ';'     
                   |  class_declarations class_declaration ';'    
                   ;

class_declaration : CLASS ID '{' class_body '}' {
                        if(!vars.existsVar())
                            vars.add_family_block($2);
                        else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                        if(!fct.existsFct())
                            fct.add_family_block($2);
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
                  ;

class_body : class_fct_declarations class_var_declarations
           ;

class_var_declarations : class_var_declaration_list
                       | /* epsilon */
                       ;

class_var_declaration_list : var_decl_type ';'      
                           | class_var_declaration_list var_decl_type ';'  
                           ;


class_fct_declarations : class_fct_decl 
                       | /* epsilon */
                       ;

class_fct_decl : antet ';'      
               | class_fct_decl antet ';'   
               ;

//////////////////

variable_block : VARDCL '{' var_declarations '}' {
                    if(!vars.existsVar())
                        vars.add_family_block("global");
                    else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                }
                | /* epsilon */
                ;

var_declarations :  var_decl_type ';'      
	      |  var_declarations var_decl_type ';'    
	      ;

var_decl_type: var_decl
             | var_decl_with_assignment
             ;

var_decl  :  ID ':' TYPE { vars.addVar($3,$1,true,1);}
           | ID '<' NR '>' ':' TYPE{ vars.addVar($6,$1,true,std::atoi($3));}
           ;

var_decl_with_assignment :  ID ':' TYPE ASSIGN value_type  { 
                vars.addVar($3,$1,true,1);
                vars.add_value($1,$5);
            }
           | ID ':' CONST TYPE ASSIGN value_type { 
                vars.addVar($4,$1,false,1);
                vars.add_value($1,$6);
            }
           | ID '<' NR '>' ':' TYPE ASSIGN '{' value_list '}' { 
                vars.addVar($6,$1,true,std::atoi($3));
                vars.add_value($1,$9);
            }
           ;

value_list : value_type { $$ = $1;}
           | value_type ',' value_list {
                $$ = (char*)malloc(strlen($1) + strlen($3) + 1); 
                strcpy($$, $1);
                strcat($$, " ");
                strcat($$,$3);
                free($1);
            }
           ;

value_type : BOOLEAN
           | CHAR 
           | STRING
           | NR
           ;

//////////////////

function_block : FCTDCL '{' fct_declarations '}' {
                    if(!fct.existsFct())
                        fct.add_family_block("global");
                    else{
                            yyerror("Variable already declared");
                            exit(1);
                        }
                }
               | /* epsilon */
               ;

fct_declarations :  fct_decl ';'      
	      |  fct_declarations fct_decl ';'    
	      ;

fct_decl: antet
        ;

// antet: FUNCTION ID '(' list_param ')' '-' '>' TYPE '{''}' { 
//                             if(fct.addFct($8,$2,strdup(""),$4,true,vars)){
//                                 yyerror("Variable already declared in function or globally");
//                                 exit(1);
//                             }
//                          }    
//         | FUNCTION ID '(' list_param ')' '-' '>' CONST TYPE '{''}' { 
//                 if(fct.addFct($9,$2,strdup(""),$4,false,vars)){
//                                 yyerror("Variable already declared in function or globally");
//                                 exit(1);
//                             }
//                         }
//         | FUNCTION ID '(' list_param ')' '-' '>' '(' ')' '{''}' { 
//             if(fct.addFct("void",$2,strdup(""),$4,false,vars)){
//                                 yyerror("Variable already declared in function or globally");
//                                 exit(1);
//                             }
//             }
//         | FUNCTION ID '(' ')' '-' '>' TYPE '{''}' { 
//             if(fct.addFct($7,$2,strdup(""),"",true,vars)){
//                                 yyerror("Variable already declared in function or globally");
//                                 exit(1);
//                             }
//                             }
//         | FUNCTION ID '(' ')' '-' '>' CONST TYPE '{''}' {
//             if(fct.addFct($8,$2,strdup(""),"",false,vars)){
//                                 yyerror("Variable already declared in function or globally");
//                                 exit(1);
//                             }
//                             }
//         | FUNCTION ID '(' ')' '-' '>' '(' ')' '{' '}' { 
//             if(fct.addFct("void",$2,strdup(""),"",false,vars)){
//                                 yyerror("Variable already declared in function or globally");
//                                 exit(1);
//                             }
//                             }
//         ;     

antet: FUNCTION ID '(' list_param ')' '-' '>' TYPE '{' before_list '}' { 
               if(fct.addFct($8,$2,strdup(""),$4,true,vars, [](const std::vector<double>& args) -> ValueType {
                    //Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block($2);
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
          | FUNCTION ID '(' list_param ')' '-' '>' CONST TYPE '{' before_list '}' { 
               if(fct.addFct($9,$2,strdup(""),$4,false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block($2);
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
          | FUNCTION ID '(' list_param ')' '-' '>' '(' ')' '{' before_list '}' { 
            if(fct.addFct("void",$2,strdup(""),$4,false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block($2);
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
        | FUNCTION ID '(' ')' '-' '>' TYPE '{' before_list '}' { 
               if(fct.addFct($7,$2,strdup(""),"",true,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block($2);
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
        | FUNCTION ID '(' ')' '-' '>' CONST TYPE '{' before_list '}' {
          if(fct.addFct($8,$2,strdup(""),"",false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block($2);
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
        | FUNCTION ID '(' ')' '-' '>' '(' ')' '{' before_list '}' { 
               if(fct.addFct("void",$2,strdup(""),"",false,vars, [](const std::vector<double>& args) -> ValueType {
                    // Implem
                    return (ValueType)0;
               })){
                    yyerror("Variable already declared in function or globally");
                    exit(1);
               }
               if(!vars.existsVar())
                        vars.add_family_block($2);
                else{
                        yyerror("Variable already declared");
                        exit(1);
                    }
               
          }
        ;     



list_param : param { $$ = $1; }
           | param  ',' list_param { 
               $$ = (char*)malloc(strlen($1) + 1 + strlen($3) + 1);
               strcpy($$, $1);
               strcat($$, ",");
               strcat($$, $3);
               free($3);
            }
            ;
            
param : ID ':' type  { 
          $$ = (char*)malloc(strlen($1) + 10 + strlen($3) + 1); 
          strcpy($$, $1);
          strcat($$, ":mutable ");
          strcat($$, $3);
      }
      | ID ':' CONST type  {
          $$ = (char*)malloc(strlen($1) + 2 + strlen($4) + 1);
          strcpy($$, $1);
          strcat($$, ":");
          strcat($$, $4);
      }
      | ID '<' NR '>' ':' type{ 
          $$ = (char*)malloc(strlen($1) + 12 + strlen($3) + strlen($6) + 1); 
          strcpy($$, $1);
          strcat($$, "<");
          strcat($$,$3);
          strcat($$,">:mutable ");
          strcat($$, $6);
           }
      | ID '<' NR '>' ':' CONST type{ 
          $$ = (char*)malloc(strlen($1) + 5 + strlen($3) + strlen($7) + 1);
          strcpy($$, $1);
          strcat($$, "<");
          strcat($$,$3);
          strcat($$,">: ");
          strcat($$, $7);
          }
      ; 

type : TYPE { $$ = (char*)malloc(strlen($1) + 1); 
            strcpy($$, $1);
            }
     | ID {$$ = (char*)malloc(strlen($1) + 1); 
            strcpy($$, $1);
            }
     ;

//////////////////

before_list : list
            | /* epsilon */
            ;

block : MAIN '{' before_list '}' { if(!vars.existsVar())
                        vars.add_family_block("main");
                        else{
                                yyerror("Variable already declared");
                                exit(1);
                            }
                        } 
     ;
     

list :  statement ';' 
     | list statement ';'
     ;

statement: ID ASSIGN expression { 

                                string decoy = string($1);

                                if(TypeOf(decoy) != TypeOf(Typeof_expression))
                                {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_value($1,$3->Eval(),TypeOf(decoy),strdup(""),"0");
                                }
                                    Typeof_expression.clear();
                                }
         | ID '<' NR '>' ASSIGN expression { 

                                string decoy = string($1);
                                decoy += "<";
                                decoy += string($3);
                                decoy += ">";


                                if(TypeOf(decoy) != TypeOf(Typeof_expression))
                                {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_value($1,$6->Eval(),TypeOf(decoy),strdup(""),$3);
                                }
                                    Typeof_expression.clear();
                                }
         | ID '.' ID ASSIGN expression { 
                                string decoy = string($3);

                                if(TypeOf(decoy) != TypeOf(Typeof_expression))
                                 {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_value($3,$5->Eval(),TypeOf(decoy),$1,"0");
                                }
                                    Typeof_expression.clear();
                                }
         | ID ASSIGN STRING { 
                                string decoy = string($1);

                                if(TypeOf(decoy) != "multicei")
                                {
                                    yyerror("The language doesnt support type casting between expressions and the variable that is supposed to be populated");
                                    exit(1);
                                }
                                else
                                {
                                    vars.add_assign_string($1,$3,strdup(""));
                                }
                                    
                                } 
         | function_call
         | if       {Typeof_expression.clear();}
         | while    {Typeof_expression.clear();}
         | for      {Typeof_expression.clear();}
         | predefined
         | var_decl_type
         ;

predefined : EVAL '(' expression ')' { 

                char dec[100];
                string string_value;

                if(TypeOf(Typeof_expression) == "basic")
                {
                    sprintf(dec,"%.0f",$3->Eval());
                    string_value = string(dec);
                }
                else if(TypeOf(Typeof_expression) == "smart")
                {
                    sprintf(dec,"%.5f",$3->Eval());
                    string_value = string(dec);
                }
                else if(TypeOf(Typeof_expression) == "chatgpt")
                {
                    sprintf(dec,"%.5f",$3->Eval());
                    string_value = string(dec);
                }
                else if(TypeOf(Typeof_expression) == "singurel")
                {
                    char asciiChar = static_cast<char>($3->Eval());
                    string_value = asciiChar;
                }
                else if(TypeOf(Typeof_expression) == "maybe")
                {
                    if($3->Eval() == 1)
                        string_value = "true";
                    else
                        string_value = "false";
                }

                cout << string_value << endl;

                Typeof_expression.clear();}
           | TYPEOF '(' expression ')' {  cout << TypeOf(Typeof_expression) << endl;Typeof_expression.clear();}
        
/* left_value : ID {
                strcpy($$, $1);
                }
           | ID '<' NR '>' {
                strcpy($$, $1);
                strcat($$, "<");
                strcat($$, $3);
                strcat($$, ">");
                }
           | ID '.' ID  {
                strcpy($$, $1);
                strcat($$, ".");
                strcat($$, $3);
                }
           ; */

if  : IF '(' expression ')' '{' '}'
    | IF '(' expression ')' '{' list '}'
    | IF '(' expression ')' '{' '}' ELSE '{' '}'
    | IF '(' expression ')' '{' '}' ELSE '{' list '}'
    | IF '(' expression ')' '{' list '}' ELSE '{' '}'
    | IF '(' expression ')' '{' list '}' ELSE '{' list '}'
    ;

while : WHILE '(' expression ')' '{' '}'
      | WHILE '(' expression ')' '{' list '}'
      ;

for : FOR '(' ID ASSIGN NR ';' expression ';' for_incrementation ')' '{' '}'{
                    bool gasit = false;
                    for(auto x:vars.vars)
                    {
                        if(x.name == string($3))
                        gasit = true; 
                    }
                    if(!gasit)
                    {
                        yyerror("The variable wasn't declared");
                        exit(1);
                    }
    }
    | FOR '(' ID ASSIGN NR ';' expression ';' for_incrementation ')' '{' list '}'{
                    bool gasit = false;
                    for(auto x:vars.vars)
                    {
                        if(x.name == string($3))
                        gasit = true; 
                    }
                    if(!gasit)
                    {
                        yyerror("The variable wasn't declared");
                        exit(1);
                    }
    }
    ;

for_incrementation : ID '+' '+' {
                    bool gasit = false;
                    for(auto x:vars.vars)
                    {
                        if(x.name == string($1))
                        gasit = true; 
                    }
                    if(!gasit)
                    {
                        yyerror("The variable wasn't declared");
                        exit(1);
                    }
    }
                   ;

function_call : ID '(' call_list ')' {
                            std::vector<Node*> decoy;

                            if(fct.existsWasDeclared($1) == false)
                            {
                                yyerror("Function wasnt declared");
                                exit(1);
                            }
                            if(fct.call_has_same_params($1,$3,"",vars) == -1)
                            {
                                yyerror("Function call doesn't have the same number of parameters as the declared function");
                                exit(1);
                            }
                            else if(fct.call_has_same_params($1,$3,"",vars) == -2)
                            {
                                yyerror("Function call contains a variable that hasnt been declared");
                                exit(1);
                            }
                            else if(fct.call_has_same_params($1,$3,"",vars) == -3)
                            {
                                yyerror("Function call has different types of variables in the call from the declared ones");
                                exit(1);
                            }
                            $$ = new Node(FUNCTION_CALL, UNDEFINED, $1, decoy);
                            Typeof_expression += $1;
                            Typeof_expression += "("; 
                            Typeof_expression += ") ";  
                        }
              | ID '.' ID '(' call_list ')' {
                            std::vector<Node*> decoy;
                            if(fct.existsWasDeclared($3) == false)
                            {
                                yyerror("Function wasnt declared");
                                exit(1);
                            }
                            if(fct.call_has_same_params($3,$5,$1,vars) == -1)
                            {
                                yyerror("Function call doesn't have the same number of parameters as the declared function");
                                exit(1);
                            }
                            else if(fct.call_has_same_params($3,$5,$1,vars) == -2)
                            {
                                yyerror("Function call contains a variable that hasnt been declared");
                                exit(1);
                            }
                            else if(fct.call_has_same_params($3,$5,$1,vars) == -3)
                            {
                                yyerror("Function call has different types of variables in the call from the declared ones");
                                exit(1);
                            }
                            $$ = new Node(FUNCTION_CALL, UNDEFINED, $3, decoy);
                            Typeof_expression += $1;
                            Typeof_expression += ".";
                            Typeof_expression += $3;
                            Typeof_expression += "("; 
                            Typeof_expression += ") "; 
                        }
              ;


call_list : call_list_element { $$ = $1; }
           | call_list ',' call_list_element { 
               $$ = (char*)malloc(strlen($1) + 1 + strlen($3) + 1);
               strcpy($$, $1);
               strcat($$, ",");
               strcat($$, $3);
               free($3);
            }
           ;

call_list_element : ID {
                    $$ = (char*)malloc(strlen($1) + 1); 
                    strcpy($$, $1);
                  }
                  | ID '.' ID {   
                    $$ = (char*)malloc(strlen($1) + strlen($3) + 2); 
                    strcpy($$, $1);
                    strcat($$, ",");
                    strcat($$, $3);
                  }
                  | ID '<' NR '>' {   
                    $$ = (char*)malloc(strlen($1) + strlen($3) + 3); 
                    strcpy($$, $1);
                    strcat($$, "<");
                    strcat($$, $3);
                    strcat($$, ">");
                    }
                  | NR { 
                    $$ = (char*)malloc(strlen($1) + 1); 
                    strcpy($$, $1);
                    }
                  | ID '(' call_list ')' {
                    $$ = (char*)malloc(strlen($1) + 3); 
                    strcpy($$, $1);
                    strcat($$, "(");
                    strcat($$, $3);
                    strcat($$, ")");
                  }
                  | CHAR {
                    $$ = (char*)malloc(strlen($1) + 1); 
                    strcpy($$, $1);
                  }
                  | STRING {
                    $$ = (char*)malloc(strlen($1) + 1); 
                    strcpy($$, $1);
                  }
                  | BOOLEAN {
                    $$ = (char*)malloc(5); 
                    strcpy($$, "true");
                  }
                  | /*epsilon*/ {
                    $$ = (char*)malloc(2); 
                    strcpy($$, "");
                   }
                  ;


//function_call : ID '(' call_list ')' {
//             $$ = new Node(FUNCTION_CALL, UNDEFINED, $1, (ValueType)NULL);
//             }


// call_list : call_list_element {
//                 $$ = new std::vector<Node*>();
//                 $$->push_back($1);
//             }
//             | call_list ',' call_list_element {
//                 $1->push_back($3);
//             }
//             | /*epsilon*/ { 
//                 $$ = new std::vector<Node*>(); 
//             }


// call_list_element : expression
//                   ;

expression: expression '+' expression   { /*$$ = $1 + $3;*/ $$ = new Node(OPERATOR, $1, $3, "+");
                                        }
          | expression '-' expression   { /*$$ = $1 - $3;*/ $$ = new Node(OPERATOR, $1, $3, "-");
                                        }
          | expression '*' expression   { /*$$ = $1 * $3;*/ $$ = new Node(OPERATOR, $1, $3, "*");
                                        }
          | expression '/' expression   { /*$$ = $1 / $3;*/ $$ = new Node(OPERATOR, $1, $3, "/");
                                        }
          | expression EQ expression    { /*$$ = $1 == $3;*/ $$ = new Node(OPERATOR, $1, $3, "==");
                                        }
          | expression LT expression    { /*$$ = $1 < $3;*/ $$ = new Node(OPERATOR, $1, $3, "<");
                                        }
          | expression LE expression    { /*$$ = $1 <= $3;*/ $$ = new Node(OPERATOR, $1, $3, "<=");
                                        }
          | expression GT expression    { /*$$ = $1 > $3;*/ $$ = new Node(OPERATOR, $1, $3, ">");
                                        }
          | expression GE expression    { /*$$ = $1 >= $3;*/ $$ = new Node(OPERATOR, $1, $3, ">=");
                                        }
          | expression OR expression    { /*$$ = $1 || $3;*/ $$ = new Node(OPERATOR, $1, $3, "||");
                                        }
          | expression AND expression   { /*$$ = $1 && $3;*/ $$ = new Node(OPERATOR, $1, $3, "&&");
                                        }
          | NOT expression              { /*$$ = !$2;*/ $$ = new Node(OPERATOR, nullptr, $2, "!");
                                        }
          | '(' expression ')'          { $$ = $2; }
          | BOOLEAN                     { /*$$ = atoi($1);*/ $$ = new Node(VALUE, BOOL, $1, 0);
                                            Typeof_expression += $1;
                                            Typeof_expression += " ";
                                         }
          | NR                          { /*$$ = atoi($1);*/ 
          
                                            string mystring = string($1);
                                            size_t foundPos = mystring.find(".");

                                            if (foundPos != std::string::npos) {
                                                $$ = new Node(VALUE, FLOAT, $1, 0);
                                            } else {
                                                $$ = new Node(VALUE, INT, $1, 0);
                                            }

                                            Typeof_expression += $1;
                                            Typeof_expression += " ";
                                         }
          | CHAR                        { /*$$ = atoi($1);*/ $$ = new Node(VALUE, CHAR_TYPE, $1, 0);
                                            Typeof_expression += $1;
                                            Typeof_expression += " ";
                                        }
          | ID                          { /*$$ = 1;*/ $$ = new Node(IDENTIFIER, UNDEFINED, $1, 0);
                                            Typeof_expression += $1;
                                            Typeof_expression += " ";         
                                        }
          | ID '.' ID                   { /*$$ = 1;*/ $$ = new Node(IDENTIFIER, UNDEFINED, $3, $1, 0); 
                                            Typeof_expression += $1;
                                            Typeof_expression += "."; 
                                            Typeof_expression += $3;
                                            Typeof_expression += " ";             
                                        }
          | function_call               {   $$ = $1;
                                            // Typeof_expression += $1;
                                            // Typeof_expression += " ";
                                        }
          | ID '<' NR '>'               { /*$$ = 1;*/ $$ = new Node(IDENTIFIER, UNDEFINED, $1, std::stoi($3));
                                            Typeof_expression += $1;
                                            Typeof_expression += " ";         
                                        }
          | ID '.' ID '<' NR '>'          {/*$$ = 1;*/ $$ = new Node(IDENTIFIER, UNDEFINED, $3, $1, std::stoi($5)); 
                                            Typeof_expression += $3;
                                            Typeof_expression += " ";  
                                        }
          ;

%%

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


