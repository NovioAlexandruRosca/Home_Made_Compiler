#include "IdList.h"
#include <sstream>
using namespace std;


vector<string> parseMessages(const string& input)
{

    vector<string> messages;
    istringstream iss(input);

    char qoute;
    string message;

    while(iss >> qoute && qoute == '"'){
        getline(iss,message,'"');
        messages.push_back(message);
        iss >> ws;
    }

    return messages;
    
}

vector<string> parseMessagesBySpaces(const string& input)
{
    vector<string> messages;
    istringstream iss(input);

    string message;

    while(iss >> message){
        messages.push_back(message);
    }

    return messages;
}    


void VariableList::addVar(const char* type, const char* name,bool mut,int size) {
    vector<string> decoy;
    decoy.reserve(size);
    VariableInfo var = {string(type), string(name),mut,size,decoy,""};
    vars.push_back(var);
    
    variableTable[string(name)] = var;
    
}


bool VariableList::existsVar() const {
    for(int i = vars.size() ; i > 0 ; i--)
    {
        int j = i - 2;
        while(vars[j].family_block == "")
        {
            if(vars[i - 1].name == vars[j].name)
                return true;
            j--;
        }
    }

    // for (const auto& var : vars) {
    //     if (variableTable.find(var.name) != variableTable.end()) {
    //         return true; 
    //     }
    // }

    return false;
}

void VariableList::add_value(const char* name, const char* value) {
    string strvar = string(name);
    vector<string> parsedMessages;

    if (string(value).find('"') != string::npos) {
        parsedMessages = parseMessages(string(value));
        for (auto& x : parsedMessages) {
            x = "\"" + x + "\"";
        }
    } else {
        parsedMessages = parseMessagesBySpaces(string(value));
    }

    for (VariableInfo& v : vars) {
        if (strvar == v.name && v.family_block == "") {
            v.value.insert(v.value.end(), parsedMessages.begin(), parsedMessages.end());
        }
    }

    auto it = variableTable.find(strvar);
    if (it != variableTable.end()) {
        it->second.value.insert(it->second.value.end(), parsedMessages.begin(), parsedMessages.end());
    }
}


void VariableList::add_family_block(const char* family_block) {
    string strvar = string(family_block);

    for (auto& v : vars) {
        if (v.family_block.empty()) {
            v.family_block = strvar;
            auto it = variableTable.find(v.name);
            if (it != variableTable.end()) {
                it->second.family_block = strvar;
            }
        }
    }
}

void VariableList::printVars() {

    const char* filename = "Variabile.txt";

    remove(filename);

    std::fstream fileStream;

    fileStream.open(filename, std::ios::out | std::ios::app);

    for (const VariableInfo& v : vars) {
        fileStream << "name: " << v.name;
    
        if (v.size != 1)
            fileStream << "<" << v.size << ">";

        fileStream << " |" << " const:" << (v.is_mutable ? " false" : " true");
        fileStream << " |" << " type: " << v.type << " |" << " block: " << v.family_block; 
        fileStream << " |" << " value: ";
        for (const auto& x : v.value) {
            if (!x.empty())
                fileStream << x << " ";
        }
        fileStream << endl;
    }

    fileStream.close();

}

void VariableList::add_assign_value(const char* name, double value,string type,const char* family_block,const char* position){

    string string_name = string(name);
    string string_family_block = string(family_block);
    int pos = stoi(position);
    vector<VariableInfo> possible_variables;
    string string_value;

    bool there_is_a_local_variabile = false;


    for(auto& entry : vars)
    {
        if(entry.name == string_name && entry.type == type)
        {
            possible_variables.push_back(entry);
            if(entry.family_block.empty())
            there_is_a_local_variabile = true;
        }
        
    }

    char dec[100];

    if(type == "basic")
    {
        sprintf(dec,"%.0f",value);
        string_value = string(dec);
    }
    else if(type == "smart")
    {
        sprintf(dec,"%.5f",value);
        string_value = string(dec);
    }
    else if(type == "chatgpt")
    {
        sprintf(dec,"%.5f",value);
        string_value = string(dec);
    }
    else if(type == "singurel")
    {
        char asciiChar = static_cast<char>(value);
        string_value = asciiChar;
    }
    else if(type == "maybe")
    {
        if(value == 1)
            string_value = "true";
        else
            string_value = "false";
    }

    int OK = 1;

    for(auto& entry1: vars)
    {
        for(auto& entry : possible_variables)
        {
            if(entry.name == entry1.name && entry.type == entry1.type && entry.family_block == entry1.family_block)
            {
                
                if(!string_family_block.empty())
                {

                    if(entry1.family_block == string_family_block)
                    {
 
                        if(entry1.is_mutable){
                            auto it = variableTable.find(entry1.name);
                            if (it != variableTable.end() && it->second.type == type && it->second.family_block == string_family_block){
                                    it->second.value.resize(pos + 500);
                                    it->second.value[pos] = string_value;
                                }
                            
                            entry1.value.resize(pos + 500);
                            entry1.value[pos] = string_value;
                        }
                        OK = 0;
                        
                        break;
                    }   
                }
                else if(there_is_a_local_variabile){
                    if(entry1.family_block.empty())
                    {
                        if(entry1.is_mutable){
                            auto it = variableTable.find(entry1.name);
                            if (it != variableTable.end() && it->second.type == type && it->second.family_block == string_family_block){
                                    it->second.value.resize(pos + 500);
                                    it->second.value[pos] = string_value;
                                }
                            
                            entry1.value.resize(pos + 500);
                            entry1.value[pos] = string_value;
                        }
                        OK = 0;

                        break;
                    }   
                }
                else
                {
                    if(entry1.family_block == "global")
                    {
                        if(entry1.is_mutable){
                            auto it = variableTable.find(entry1.name);
                            if (it != variableTable.end() && it->second.type == type && it->second.family_block == "global"){
                                    it->second.value.resize(pos + 500);
                                    it->second.value[pos] = string_value;
                                }
                            
                            entry1.value.resize(pos + 500);
                            entry1.value[pos] = string_value;
                        }
                        OK = 0;

                        break;
                    }  
                }
            }
        }
        if(OK == 0)
            break;

    }


}
void VariableList::add_assign_string(const char* name, const char * value,const char* family_block){
    string string_name = string(name);
    string string_family_block = string(family_block);
    vector<VariableInfo> possible_variables;
    string string_value = string(value);

    bool there_is_a_local_variabile = false;


    for(auto& entry : vars)
    {
        if(entry.name == string_name && entry.type == "multicei")
        {
            possible_variables.push_back(entry);
            if(entry.family_block.empty())
            there_is_a_local_variabile = true;
        }
        
    }

    int OK = 1;

    for(auto& entry1: vars)
    {
        for(auto& entry : possible_variables)
        {
            if(entry.name == entry1.name && entry.type == entry1.type && entry.family_block == entry1.family_block)
            {
                
                if(there_is_a_local_variabile){
                    if(entry1.family_block.empty())
                    {
                        if(entry1.is_mutable) {
                            entry1.value.resize(500);
                            entry1.value[0] = string_value;
                        }
                        OK = 0;

                        break;
                    }   
                }
                else
                {
                    if(entry1.family_block == "global")
                    {
                        if(entry1.is_mutable){
                            entry1.value.resize( 500);
                            entry1.value[0] = string_value;
                        }
                        OK = 0;

                        break;
                    }  
                }
            }
        }
        if(OK == 0)
            break;

    }
}

VariableList::~VariableList() {
    vars.clear();
    //variableTable.clear();
}
