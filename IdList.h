#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unordered_set>
#include "Node.h"
#include <fstream>

using namespace std;

///////////////////////////Variable Class

// struct VariableInfo {
//     string type;
//     string name;
//     bool is_mutable;
//     int size;
//     vector<string> value;
//     string family_block;
// };

class VariableList {
    public:
    vector<VariableInfo> vars;
   
    bool existsVar() const;
    void addVar(const char* type, const char* name,bool mut,int size);
    void add_value(const char* name, const char* value);
    void add_assign_value(const char* name, double value,string type,const char* family_block,const char* position);
    void add_assign_string(const char* name, const char * value,const char* family_block);
    void add_family_block(const char* family_block);
    void printVars();
    ~VariableList();
};

///////////////////////////Function Class

// struct FunctionInfo {
//     string type;
//     string name;
//     string body;
//     vector<string> args;
//     bool is_mutable;
//     string family_block;
// };

//modificat
// struct FunctionInfo {
//     std::string type;
//     std::string name;
//     std::string body; 
//     std::vector<std::string> args;
//     bool is_mutable;
//     std::string family_block;
//     std::function<ValueType(const std::vector<double>&)> implementation; 
// };


class FunctionList {
    public:
    vector<FunctionInfo> vars;
   
    bool existsFct() const;
    //int addFct(const char* type, const char* name,const char* body,const char* args,bool mut,VariableList variables);
    //modificat
    int call_has_same_params(const char* function_name,const char* call_params,const char* clasa,VariableList variables);
    int addFct(const char* type, const char* name, const char* body, const char* args, bool mut, VariableList variables, std::function<ValueType(const std::vector<double>&)> impl);
    void add_family_block(const char* family_block);
    bool existsWasDeclared(const char* name);
    void printFct();
    ~FunctionList();
};

//////////////////////////////Class Class

struct ClassInfo {
    string name;
    string body;
};

class ClassList{
    public:
    vector<ClassInfo> classes;
    int class_index = -1;

    bool existsCls(const char* name);  
    void addCls(const char* name, const char* body); 
    void printCls();
    ~ClassList();
};