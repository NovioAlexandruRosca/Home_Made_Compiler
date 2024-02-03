#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <functional>

enum ValueType { INT, FLOAT, DOUBLE, CHAR_TYPE, STRING_VAL, BOOL, UNDEFINED };
enum NodeType { OPERATOR, VALUE, IDENTIFIER, FUNCTION_CALL, CLASS_MEMBER};

struct SymbolInfo {
    double value;
    ValueType type;
};

struct VariableInfo {
    std::string type;
    std::string name;
    bool is_mutable;
    int size;
    std::vector<std::string> value;
    std::string family_block;
};

// struct FunctionInfo {
//     int expectedArgCount; 
//     std::function<ValueType(const std::vector<double>&)> implementation; 
// };

//modificat
struct FunctionInfo {
    std::string type;
    std::string name;
    std::string body; 
    //std::vector<std::pair<std::string, ValueType>> args; 
    std::vector<std::string> args; 
    bool is_mutable;
    std::string family_block;
};


extern std::unordered_map<std::string, SymbolInfo> symbolTable;
extern std::unordered_map<std::string, VariableInfo> variableTable;
extern std::unordered_map<std::string, FunctionInfo> functionTable;



extern ValueType applyFunction(const std::string& functionName, const std::vector<double>& args);


struct Node {
    NodeType nodeType;
    ValueType valueType;
    std::string value; 
    Node* left;
    Node* right;
    std::vector<Node*> arguments; 
    Node* object; 
    std::string familyBlock = "global";
    int position = 1;

    // Constructor pentru noduri simple
    Node(NodeType nodeType, ValueType valueType, std::string value, int position)
        : nodeType(nodeType), valueType(valueType), value(value), position(position), left(nullptr), right(nullptr) {}

    // Constructor pentru noduri operator (expr1 op expr2)
    Node(NodeType nodeType, Node* left, Node* right, std::string opSymbol)
        : nodeType(nodeType), valueType(UNDEFINED), value(opSymbol), left(left), right(right) {}

    // Constructor pentru apeluri de funcții
    Node(NodeType nodeType, ValueType returnType, std::string functionName, std::vector<Node*> args)
        : nodeType(nodeType), valueType(returnType), value(functionName), left(nullptr), right(nullptr), arguments(args) {}

    Node(NodeType nodeType, ValueType valueType, std::string value, std::string familyBlock, int position)
        : nodeType(nodeType), valueType(valueType), value(value), familyBlock(familyBlock), position(position), left(nullptr), right(nullptr) {}

    ~Node();
    double Eval();
    ValueType getType();
};

#endif // NODE_H