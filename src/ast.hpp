#pragma once
#include <memory>
#include <stdexcept>
#include <vector>

class ASTNode {
    public:
        virtual ~ASTNode() = default;
        virtual double evaluate() const = 0;
};

// for number literal node
class NumberNode : public ASTNode {
private:
    double value;
public:
    NumberNode(double value) : value(value) {}
    double evaluate() const override { return value; }
};

// for binary operation nodes
class BinaryOpNode: public ASTNode {
private:
    std::unique_ptr<ASTNode> left; // use smart pointer ( wow!)
    std::unique_ptr<ASTNode> right;
    char op;
public:
    BinaryOpNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r, char operation)
        : left(std::move(l)),  right(std::move(r)), op(operation) {}

    double evaluate() const override {
        double l_val = left->evaluate();
        double r_val = right->evaluate();

        switch(op){
            case '+': return l_val + r_val;
            case '-': return l_val - r_val;
            case '*': return l_val * r_val;
            case '/':
                if(r_val == 0){
                    throw std::runtime_error("Cannot divide by zero!");
                } else {
                    return l_val / r_val;
                }
            default:
                throw std::runtime_error("Unknown operator!");
        }
    }
};

// for -NUMBER
class UnaryMinusNode : public ASTNode{
private:
    std::unique_ptr<ASTNode> operand;
public:
    UnaryMinusNode(std::unique_ptr<ASTNode> o) : operand(std::move(o)) {}
    double evaluate() const override { return -operand ->evaluate(); }
};
