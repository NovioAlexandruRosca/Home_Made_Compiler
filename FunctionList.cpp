#include "IdList.h"
#include <sstream>

using namespace std;

//int FunctionList::addFct(const char* type, const char* name,const char* body,const char* args,bool mut,VariableList variables) {

int FunctionList::addFct(const char* type, const char* name, const char* body, const char* args, bool mut, VariableList variables, std::function<ValueType(const std::vector<double>&)> impl) {
    vector<string> arg;
    if(strcmp(args,"") != 0)
    {
        string str(args);

        size_t pos = 0;
        string token;

        while ((pos = str.find(',')) != string::npos) {
            token = str.substr(0, pos);
            
            arg.push_back(token);

            str.erase(0, pos + 1);
        }

        arg.push_back(str);
    }

    vector<string> args_name_only;
    vector<string> args_const_only;
    vector<string> args_type_only;

    //////// arg are si nume si tip si daca e costant

    for(auto x : arg)
    {
        size_t dotPosition = x.find(':');

        if (dotPosition != string::npos && dotPosition + 1 < x.length()) {

                args_name_only.push_back(x.substr(0,dotPosition));

            }

        x = x.substr(dotPosition + 1);
        
        size_t spacesPosition = x.find(' ');

        if (spacesPosition != string::npos && spacesPosition + 1 < x.length()) {

                args_const_only.push_back(x.substr(0,spacesPosition));
                args_type_only.push_back(x.substr(spacesPosition + 1));
            }
        else
            {
                args_const_only.push_back(string("const"));
                args_type_only.push_back(x);
            }

    }

    unordered_set<string> uniqueElements;

    for(auto nume : args_name_only){

        if(nume.find('<'))
        {
            nume = nume.substr(0,nume.find('<'));
        }

        if(!uniqueElements.insert(nume).second)
            return 1;
    }

    for(auto nume : variables.vars)
    {
        if(nume.family_block == "global")
            if(!uniqueElements.insert(nume.name).second)
                return 2;
    }

    if(!uniqueElements.insert(string(name)).second)
            return 3;

    // for(int i = 0 ; i < args_name_only.size() ; i++)
    // {
    //     cout << args_name_only[i] << " " << args_const_only[i] << " " << args_type_only[i] << endl;
    // }

    FunctionInfo var = {string(type), string(name),string(body),arg,mut,string("")};
    vars.push_back(var);

    functionTable[string(name)] = var;

    return 0;
}

bool FunctionList::existsWasDeclared(const char* name){
    for(auto& x : vars)
    {
        if(x.name == name)
            return true;
    }
    return false;
}

bool FunctionList::existsFct()const {
    for(int i = vars.size() - 1; i >= 0 ; i--)
    {
        int j = i - 2;
        while(vars[j].family_block == "")
        {
            if(vars[i - 1].name == vars[j].name)
                return true;
            j--;
        }
    }
    return false;
}

string valueTypeToString(ValueType type) {
    switch (type) {
        case INT: return "INT";
        case FLOAT: return "FLOAT";
        case DOUBLE: return "DOUBLE";
        case CHAR_TYPE: return "CHAR";
        case STRING_VAL: return "STRING_VAL";
        case BOOL: return "BOOL";
        case UNDEFINED: return "UNDEFINED";
        default: return "UNKNOWN";
    }
}

void FunctionList::printFct() {

    const char* filename = "Functii.txt";

    std::fstream fileStream;

    remove(filename);

    fileStream.open(filename, std::ios::out | std::ios::app);

    for (const FunctionInfo& v : vars) {
        fileStream << "name: " << v.name << " | args: (";
        bool first_space = true;

        if(!v.args.empty())
        {

            for(auto arg: v.args)
            {
                if(first_space)
                {
                    first_space = false;
                    fileStream << arg;
                }
                else
                {
                    fileStream << "," << arg;
                }

            }

        }

        fileStream << ") | const: " << (v.is_mutable ? " false " : "true ") << "| type:" << v.type;
        fileStream << " | block: " << v.family_block << endl; 
     }

    fileStream.close();

}

void FunctionList::add_family_block(const char* family_block) {
    string strvar = string(family_block);

    for (auto& v : vars) {
        if (v.family_block.empty()) {
            v.family_block = strvar;
            auto it = functionTable.find(v.name);
            if (it != functionTable.end()) {
                it->second.family_block = strvar;
            }
        }
    }
}

int FunctionList::call_has_same_params(const char* function_name,const char* call_params,const char* clasa,VariableList variables)
{

    string correct_arguments = string(call_params);

    std::stringstream ss(correct_arguments);
    std::string substring;
    std::vector<std::string> substrings;
    std::vector<bool> substrings_isfunction;
    std::vector<std::string> stored_args;

    //aici se sectioneaza argumentele din apelul de functie

    // while (std::getline(ss, substring, ',')) {

    //     string name;
    //     bool is_function = false;

    //     for(int i = 0 ; i < substring.size() ; i++)
    //         if(substring[i] != '<' && substring[i] != '(')
    //             name += substring[i];
    //         else if(substring[i] == '(')
    //         {
    //             is_function = true;
    //             break;
    //         }
    //         else
    //             break;

    //     substrings_isfunction.push_back(is_function);
    //     substrings.push_back(name);
    // }

    string tok;
    bool dont_divide = false;
    bool is_fct = false;

    for(int l = 0 ; l < correct_arguments.size() ; l++)
    {
        if(correct_arguments[l] == '(')
            {
            dont_divide = true;
            is_fct = true;
            }
        else if(correct_arguments[l] == ')')
            dont_divide = false;

        if(correct_arguments[l] == ',' && dont_divide == false)
        {
            substrings_isfunction.push_back(is_fct);
            substrings.push_back(tok);

            tok.clear();
            is_fct = false;
        }
        else
        {
            if(dont_divide == false && correct_arguments[l] != ')')
                tok += correct_arguments[l];
        }
    }

    if(!tok.empty())
    {
        substrings_isfunction.push_back(is_fct);
        substrings.push_back(tok);
        tok.clear();
    }

    // AICI SE SECTIONEAZA ARGUMENTELE DIN CLASA

    for (auto& v : vars) {
        string fct_name = string(function_name);
        if(fct_name == v.name && (v.family_block == "global" && strcmp(clasa,"") == 0 || v.family_block == clasa && strcmp(clasa,"") != 0))
        {    
            for(auto& argumente : v.args)

                {

                    std::stringstream ss(argumente);
                    std::string token;
                    std::string type;

                    while (std::getline(ss, token, ' ')) {
                        if (token.find(':') != std::string::npos) {

                            size_t pos = token.find(':');
                            type = token.substr(pos + 1);
                        } else {

                            type = token;
                        }
                    }

                    stored_args.push_back(type);

                }
        }
    }

    if(stored_args.size() != substrings.size())
        return -1;

    for(int i = 0 ; i < stored_args.size() ; i++)
    {

        int variable_exists = 0;
        string tip_variabila;

        for(auto var : variables.vars)
        {

            if(substrings[i][0] >= '0' && substrings[i][0] <= '9' && substrings[i].find('.') != string::npos)
            {
                variable_exists = 1;
                tip_variabila = string("smart");
                break;
            }

            if(substrings[i][0] >= '0' && substrings[i][0] <= '9')
            {
                variable_exists = 1;
                tip_variabila = string("basic");
                break;
            }

            if(substrings[i][0] == '\'')
            {
                variable_exists = 1;
                tip_variabila = string("singurel");
                break;
            }

            if(substrings[i][0] == '\"')
            {
                variable_exists = 1;
                tip_variabila = string("multicei");
                break;
            }

            if(var.name == substrings[i] && substrings_isfunction[i] == false)
            {
                variable_exists = 1;
                tip_variabila = var.type;
                break;
            }
        }

        for(auto var : vars)
        {
            if(var.name == substrings[i] && substrings_isfunction[i] == true)
            {
                variable_exists = 1;
                tip_variabila = var.type;
                break;
            }
        }

        if(variable_exists == 0)
            return -2;


        if(tip_variabila != stored_args[i])
            return -3;
    }


    return 1;
}

FunctionList::~FunctionList() { 
    for (auto v : vars) {
        v.args.clear();
    }
    vars.clear();
    functionTable.clear();
}
