#ifndef AST_H
#define AST_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

extern std::map<std::string, int> symbolTable;

class Node {
public:
    virtual ~Node() {}
    virtual int evaluate() = 0;
    virtual void printJSON(std::ostream& os) = 0;
};

class NumberNode : public Node {
    int value;
public:
    NumberNode(int v) : value(v) {}
    int evaluate() override { return value; }
    void printJSON(std::ostream& os) override {
        os << "{\"name\": \"Number: " << value << "\"}";
    }
};

class IdNode : public Node {
    std::string name;
public:
    IdNode(const std::string& n) : name(n) {}
    int evaluate() override { return symbolTable[name]; }
    void printJSON(std::ostream& os) override {
        os << "{\"name\": \"Var: " << name << "\"}";
    }
};

class OpNode : public Node {
    char op;
    Node* left;
    Node* right;
public:
    OpNode(char o, Node* l, Node* r) : op(o), left(l), right(r) {}

    int evaluate() override {
        if (op == '=') {

            int val = right->evaluate();
            return val;
        }
        int l = left->evaluate();
        int r = right->evaluate();
        switch(op) {
            case '+': return l + r;
            case '-': return l - r;
            case '*': return l * r;
            case '/': return r != 0 ? l / r : 0;
            default: return 0;
        }
    }

    void printJSON(std::ostream& os) override {
        os << "{\"name\": \"Op: " << op << "\", \"children\": [";
        left->printJSON(os);
        os << ", ";
        right->printJSON(os);
        os << "]}";
    }
};

#endif