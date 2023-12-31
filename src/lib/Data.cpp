#include "Data.h"

using namespace std;

Data::Data() : data_type("NONE"), actual_val("NONE"), bool_val(false), double_val(0), is_array_val_(false) { }

Data::Data(string raw_val, string in_data_type): data_type(in_data_type), actual_val(raw_val), is_array_val_(false) { }

map<string,Data> Data::curr_variables;

ostream& operator<<(ostream& out, const Data& val) {
    if (val.data_type == "DOUBLE") {
        out << val.double_val;
    }
    else if (val.data_type == "BOOL") {
        if (val.bool_val) {
            out << "true";
        }
        else  {
            out << "false";
        }
    }
    else if (val.data_type == "ARRAY") {
        out << "[";
        if (!val.array_elements.empty()) {
            for (size_t i = 0; i < val.array_elements.size(); ++i) {
                out << val.array_elements.at(i);
                if (i + 1 != val.array_elements.size()) {
                    out << ", ";
                }
            }
        }
        out << "]";
    }
    else if (val.data_type == "NULL") {
        out << "null";
    }
    return out;
}

bool Data::operator==(const Data& val) {
    if (data_type != val.data_type) {
        return false;
    }
    if (data_type == "BOOL") {
        if (bool_val == val.bool_val) {
            return true;
        }
    }
    else if (data_type == "DOUBLE") {
        if (double_val == val.double_val) {
            return true;
        }
    }
    else if (data_type == "ARRAY") {
        if (array_elements.empty() && val.array_elements.empty()) {
            return true;
        }
        else if (array_elements.size() != val.array_elements.size()) {
            return false;
        }
        for (size_t i = 0; i < array_elements.size(); ++i) {
            if (!(array_elements.at(i) == val.array_elements.at(i))) {
                return false;
            }
        }
        return true;
    }
    else if (data_type == "NULL") {
        return true;
    }
    return false;
}


