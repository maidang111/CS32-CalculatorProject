#include "Data.h"

using namespace std;

Data::Data() : data_type("NONE"), actual_val("NONE"), bool_val(false), double_val(0) { }

Data::Data(string raw_val, string in_data_type): data_type(in_data_type), actual_val(raw_val) { }

map<string,Data> Data::curr_variables;

