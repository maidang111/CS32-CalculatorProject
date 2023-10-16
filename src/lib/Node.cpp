#include "AST.h"
using namespace std;

Node::Node() : parent(nullptr), data(nullptr) {}

Node::~Node() {
    delete data;
}