#include "Node.h"
#include <unordered_map>
#include <vector>
#include <string>

std::unordered_map<std::string, SymbolInfo> symbolTable;
std::unordered_map<std::string, VariableInfo> variableTable;
std::unordered_map<std::string, FunctionInfo> functionTable;

// ValueType applyFunction(const std::string& functionName, const std::vector<double>& args) {
//     auto funcIter = functionTable.find(functionName);
//     if (funcIter == functionTable.end()) {
//         throw std::runtime_error("Funcția nu este definită: " + functionName);
//     }

//     const FunctionInfo& funcInfo = funcIter->second;

//     if (args.size() != funcInfo.expectedArgCount) {
//         throw std::runtime_error("Număr incorect de argumente pentru funcția: " + functionName);
//     }

//     return funcInfo.implementation(args);
// }

//modificat
// static ValueType applyFunction(const std::string& functionName, const std::vector<Node*>& args) {
//     auto funcIter = functionTable.find(functionName);
//     if (funcIter == functionTable.end()) {
//         throw std::runtime_error("Funcția nu este definită: " + functionName);
//     }

//     const FunctionInfo& funcInfo = funcIter->second;

    // // Map function types to predefined ValueType values
    // if (funcInfo.type == "basic") {
    //     return INT;
    // } else if (funcInfo.type == "smart") {
    //     return FLOAT;
    // } else if (funcInfo.type == "chatgpt") {
    //     return DOUBLE;
    // } else if (funcInfo.type == "singurel") {
    //     return CHAR_TYPE;
    // } else if (funcInfo.type == "multicei") {
    //     return STRING_VAL;
    // } else if (funcInfo.type == "maybe") {
    //     return BOOL;
    // } else {
    //     return UNDEFINED;
    // }
// }


ValueType applyFunction(const std::string& functionName, const std::vector<Node*>& args) {
    auto funcIter = functionTable.find(functionName);
    if (funcIter == functionTable.end()) {
        throw std::runtime_error("Function not defined: " + functionName);
    }

    const FunctionInfo& funcInfo = funcIter->second;

    if (funcInfo.type == "basic") {
        return INT;
    } else if (funcInfo.type == "smart") {
        return FLOAT;
    } else if (funcInfo.type == "chatgpt") {
        return DOUBLE;
    } else if (funcInfo.type == "singurel") {
        return CHAR_TYPE;
    } else if (funcInfo.type == "multicei") {
        return STRING_VAL;
    } else if (funcInfo.type == "maybe") {
        return BOOL;
    } else {
        return UNDEFINED;
    }

}


extern ValueType TypeOf(Node* node) {
    if (!node) {
        throw std::runtime_error("Nodul este null");
    }

    switch (node->nodeType) {
        case VALUE:
            return node->valueType;

        case IDENTIFIER: {
            auto varIter = variableTable.find(node->value);
            if (varIter != variableTable.end()) {
                if (varIter->second.type == "basic") return INT;
                else if (varIter->second.type == "smart") return FLOAT;
                else if (varIter->second.type == "chatgpt") return DOUBLE;
                else if (varIter->second.type == "singurel") return CHAR_TYPE;
                else if (varIter->second.type == "multicei") return STRING_VAL;
                else if (varIter->second.type == "maybe") return BOOL;
                else return UNDEFINED;
            }
            throw std::runtime_error("Variabilă nedefinită: " + node->value);
        }
        case OPERATOR:
            if (node->value == "+" || node->value == "-" || node->value == "*" || node->value == "/") {
                ValueType leftType = TypeOf(node->left);
                ValueType rightType = TypeOf(node->right);

                if (leftType == DOUBLE && rightType == DOUBLE){
                    return DOUBLE;
                } else if (leftType == FLOAT && rightType == FLOAT) {
                    return FLOAT;
                } else if (leftType == INT && rightType == INT) {
                    return INT;
                } else if (leftType == CHAR_TYPE && rightType == CHAR_TYPE){
                    return CHAR_TYPE;
                } else {
                    throw std::runtime_error("Tipuri incompatibile pentru operație");
                }
            } else if (node->value == "&&" || node->value == "||" || node->value == "!") {
                return BOOL;
            } else if (node->value == "==" || node->value == "<=" || node->value == ">=" || node->value == ">" || node->value == "<"){
                return BOOL;
            }
            // case FUNCTION_CALL: {
            //     auto funcIter = functionTable.find(node->value);
            //     if (funcIter == functionTable.end()) {
            //         throw std::runtime_error("Funcția nu este definită: " + node->value);
            //     }
            //     std::vector<double> dummyArgs(funcIter->second.expectedArgCount, 0.0);
            //     return applyFunction(node->value, dummyArgs);
            // }
            //modificat
            case FUNCTION_CALL: {
                auto funcIter = functionTable.find(node->value);
                if (funcIter != functionTable.end()) {
                    const FunctionInfo& funcInfo = funcIter->second;
                    if (funcInfo.type == "basic") return INT;
                    else if (funcInfo.type == "smart") return FLOAT;
                    else if (funcInfo.type == "chatgpt") return DOUBLE;
                    else if (funcInfo.type == "singurel") return CHAR_TYPE;
                    else if (funcInfo.type == "multicei") return STRING_VAL;
                    else if (funcInfo.type == "maybe") return BOOL;
                    else return UNDEFINED;
                }
                throw std::runtime_error("Funcție nedefinită: " + node->value);
            }
            break;
    }
    return UNDEFINED;
}


double Node::Eval() {
    std::unordered_map<std::string, VariableInfo>::iterator varIter;
    std::unordered_map<std::string, FunctionInfo>::iterator funcIter;

    switch (nodeType) {
        case VALUE:
            switch (valueType) {
                case INT: return std::stoi(value);
                case FLOAT: return std::stof(value);
                case DOUBLE: return std::stod(value);
                case CHAR_TYPE: return static_cast<double>(value[1]);
                case STRING_VAL: throw std::runtime_error("Operația nu este permisă pe string-uri");
                case BOOL: return value == "true" ? 1.0 : 0.0;
                default: throw std::runtime_error("Tip nevalid pentru valoare");
            }

        // case OPERATOR:
        //     if (value == "+") return left->Eval() + right->Eval();
        //     if (value == "-") return left->Eval() - right->Eval();
        //     if (value == "*") return left->Eval() * right->Eval();
        //     if (value == "/") {
        //         double rightVal = right->Eval();
        //         if (rightVal == 0) throw std::runtime_error("Division by zero");
        //         return left->Eval() / rightVal;
        //     }
        //     if (value == "&&") return left->Eval() && right->Eval();
        //     if (value == "||") return left->Eval() || right->Eval();
        //     if (value == "!") return !right->Eval(); 
        //     if (value == "==") return left->Eval() == right->Eval();
        //     if (value == "<") return left->Eval() < right->Eval();
        //     if (value == "<=") return left->Eval() <= right->Eval();
        //     if (value == ">") return left->Eval() > right->Eval();
        //     if (value == ">=") return left->Eval() >= right->Eval();
        //     break;

        case OPERATOR:
            if (value == "!") {
                double rightVal = right ? right->Eval() : 0;
                return !rightVal;
            }

            if (value == "&&") {
                double leftVal = left ? left->Eval() : 0;
                if(leftVal == 0) return leftVal;
                double rightVal = right ? right->Eval() : 0;
                return leftVal && rightVal;
            }

            if (value == "||") {
                double leftVal = left ? left->Eval() : 0;
                double rightVal = right ? right->Eval() : 0;
                return leftVal || rightVal;
            }

            if (value == "==" || value == "<" || value == "<=" ||
                value == ">" || value == ">=") {
                double leftVal = left ? left->Eval() : 0;
                double rightVal = right ? right->Eval() : 0;
                if (value == "==") return leftVal == rightVal;
                else if (value == "<") return leftVal < rightVal;
                else if (value == "<=") return leftVal <= rightVal;
                else if (value == ">") return leftVal > rightVal;
                else if (value == ">=") return leftVal >= rightVal;
            }

            if (value == "*" || value == "/") {
                double leftVal = left->Eval();
                double rightVal = right->Eval();
                if (value == "*") return leftVal * rightVal;
                if (value == "/") {
                    if (rightVal == 0) throw std::runtime_error("Division by zero");
                    return leftVal / rightVal;
                }
            }
            if (value == "+" || value == "-") {
                double leftVal = left->Eval();
                double rightVal = right->Eval();
                if (value == "+") return leftVal + rightVal;
                if (value == "-") return leftVal - rightVal;
            }
            break;

        case IDENTIFIER:
            varIter = variableTable.find(value);
            if (varIter != variableTable.end()) {
                if(varIter->second.value[position].empty())
                    throw std::runtime_error("Variabilă nulă: " + value);
                if(varIter->second.type == "singurel")
                    return static_cast<double>(varIter->second.value[position][1]);
                if(varIter->second.value[position] == "true")
                    return 1;
                else if(varIter->second.value[position] == "false")
                    return 0;
                return std::stod(varIter->second.value[position]);

            }
            throw std::runtime_error("Variabilă nedefinită: " + value);

        // case FUNCTION_CALL: {
        //     auto funcIter = functionTable.find(value);
        //     if (funcIter == functionTable.end()) {
        //         throw std::runtime_error("Funcție nedefinită: " + value);
        //     }

        //     std::vector<double> evaluatedArgs;
        //     for (auto& arg : arguments) {
        //         evaluatedArgs.push_back(arg->Eval());
        //     }

        //     return applyFunction(value, evaluatedArgs);
        // }
        case FUNCTION_CALL: {
            auto funcIter = functionTable.find(value);
            if (funcIter == functionTable.end()) {
                throw std::runtime_error("Funcție nedefinită: " + value);
            }

            const FunctionInfo& funcInfo = funcIter->second;

            if (funcInfo.type == "basic") {
                return INT; 
            } else if (funcInfo.type == "smart") {
                return FLOAT;  
            } else if (funcInfo.type == "chatgpt") {
                return DOUBLE;  
            } else if (funcInfo.type == "singurel") {
                return CHAR_TYPE;  
            } else if (funcInfo.type == "multicei") {
                return STRING_VAL;  
            } else if (funcInfo.type == "maybe") {
                return BOOL;  
            } else {
                return UNDEFINED;  
            }
        }
        case CLASS_MEMBER: {
            varIter = variableTable.find(value);
            if (varIter != variableTable.end() && varIter->second.family_block == familyBlock) {
                if(varIter->second.value[position].empty())
                    throw std::runtime_error("Variabilă nulă: " + value);
                if(varIter->second.type == "singurel")
                    return static_cast<double>(varIter->second.value[position][1]);

                return std::stod(varIter->second.value[position]);
               //return 1;
            }
            throw std::runtime_error("Variabilă nedefinită: " + value);
        }
    

    }
    return 0;
}

ValueType Node::getType() {
    return valueType;
}

Node::~Node() {
    delete left;
    delete right;
    for (auto& arg : arguments) {
        delete arg;
    }
}