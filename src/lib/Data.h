#ifndef DATA_H
#define DATA_H
#include <string>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

struct Data {
    string data_type;
    string actual_val;
    bool bool_val = false;
    double double_val = 0;
    int index_val = -1;
    bool is_array_val_;
    vector<Data> array_elements;
    Data();
    Data(string raw_val, string in_data_type);
    friend ostream& operator<<(ostream& out, const Data& val);
    bool operator==(const Data& val);
    static map<string, Data> curr_variables;
};

#endif 