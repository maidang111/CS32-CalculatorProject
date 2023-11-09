#ifndef DATA_H
#define DATA_H
#include <string>
#include <map>

using namespace std;

struct Data {
    string data_type;
    string actual_val;
    bool bool_val = false;
    double double_val = 0;
    Data();
    Data(string raw_val, string in_data_type);
    static map<string, Data> curr_variables;
};

#endif 