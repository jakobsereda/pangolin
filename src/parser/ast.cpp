#include <iostream>

#include "ast.hpp"

std::string build_indent(int level)
{
	return std::string(level, '\t');
}

void ProgramNode::pretty_print(int level)
{
	std::cout << build_indent(level) << "PROGRAM\n";
	for (auto& s : statements)
		s->pretty_print(level + 1);
}

void ProgramNode::add_statement(std::unique_ptr<ASTNode> statement)
{
	statements.push_back(std::move(statement));
}

void LiteralNode::pretty_print(int level)
{
	std::cout << build_indent(level) << "LITERAL | value: " << value.literal << "\n";
}

void UnaryOpNode::pretty_print(int level)
{
	std::cout << build_indent(level) << "UNARY | op: " << op.literal << "\n";
	body->pretty_print(level + 1);
}

void BinaryOpNode::pretty_print(int level)
{
	std::string op_type = op.literal;
    if (op_type == "<" || op_type == ">" || op_type == "==" ||
        op_type == "!=" || op_type == "<=" || op_type == ">=")
        std::cout << build_indent(level) << "BINARY (comparison) | op: " << op.literal << "\n";
    else
        std::cout << build_indent(level) << "BINARY | op: " << op.literal << "\n";
    left->pretty_print(level + 1);
    right->pretty_print(level + 1);
}

void ParenthesesNode::pretty_print(int level)
{
	std::cout << build_indent(level) << "PARENTHESES (\n";
	body->pretty_print(level + 1);
	std::cout << build_indent(level) << ")\n";
}

void AssignmentNode::pretty_print(int level)
{
	std::cout << build_indent(level) << "ASSIGNMENT | name: " << name.literal << "\n";
	value->pretty_print(level + 1);
}

void ErrorNode::pretty_print(int level)
{
	std::cout << build_indent(level) << "ERROR | message: " << message << "\n";
}

void IfNode::pretty_print(int level)
{
    std::string indent(level * 2, ' ');
    std::cout << indent << "IfNode:\n";
    
    std::cout << indent << "  Condition:\n";
    condition->pretty_print(level + 2);
    
    std::cout << indent << "  Then:\n";
    then_branch->pretty_print(level + 2);
    
    if (else_branch) {
        std::cout << indent << "  Else:\n";
        else_branch->pretty_print(level + 2);
    }
}

void BlockNode::pretty_print(int level)
{
    std::string indent(level * 2, ' ');
    std::cout << indent << "BlockNode:\n";
    
    for (const auto& statement : statements) {
        statement->pretty_print(level + 1);
    }
}

void BlockNode::add_statement(std::unique_ptr<ASTNode> statement)
{
    statements.push_back(std::move(statement));
}
