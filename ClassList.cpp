#include "IdList.h"

using namespace std;

void ClassList::addCls(const char* name, const char* body) {

        ClassInfo newClass;
        newClass.name = string(name);
        newClass.body = string(body);

        classes.push_back(newClass);
    
}

bool ClassList::existsCls(const char* name) {
    for (const ClassInfo& cls : classes) {
        if (strcmp(name, cls.name.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

void ClassList::printCls() {
    for (ClassInfo& cls : classes) {
        cout << "Class Name: " << cls.name << "\n";
        cout << "Class Body: " << cls.body << "\n";
        cout << endl;
    }
}



ClassList::~ClassList() { 
    classes.clear();
}