#pragma once

#include <memory>
#include <vector>

#include "../tokens/token.hpp"

class ASTNode {
public:
	virtual ~ASTNode() = default;
	virtual void pretty_print(int level = 0) = 0;
};

class ProgramNode : public ASTNode {
public:
	void pretty_print(int level) override;
	void add_statement(std::unique_ptr<ASTNode> statement);

private:
	std::vector<std::unique_ptr<ASTNode>> statements;
};

class LiteralNode : public ASTNode {
public:
	LiteralNode(Token value)
		: value(value) {}

	void pretty_print(int level) override;

private:
	Token value;
};

class UnaryOpNode : public ASTNode {
public:
	UnaryOpNode(Token op, std::unique_ptr<ASTNode> body)
		: op(op), body(std::move(body)) {}

	void pretty_print(int level) override;

private:
	Token                    op;
	std::unique_ptr<ASTNode> body;
};

class BinaryOpNode : public ASTNode {
public:
	BinaryOpNode(Token op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
		: op(op), left(std::move(left)),  right(std::move(right)) {}

	void pretty_print(int level) override;

private:
	Token                    op;
	std::unique_ptr<ASTNode> left;
	std::unique_ptr<ASTNode> right;
};

class ParenthesesNode : public ASTNode {
public:
	ParenthesesNode(std::unique_ptr<ASTNode> body)
		: body(std::move(body)) {}

	void pretty_print(int level) override;

private:
	std::unique_ptr<ASTNode> body;
};

class AssignmentNode : public ASTNode {
public:
	AssignmentNode(Token name, std::unique_ptr<ASTNode> value)
		: name(name), value(std::move(value)) {}

	void pretty_print(int level) override;

private:
	Token                    name;
	std::unique_ptr<ASTNode> value;
};

class IfNode : public ASTNode {
public:
	IfNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> then_branch, std::unique_ptr<ASTNode> else_branch = nullptr) : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}

	void pretty_print(int level) override;

private:
	std::unique_ptr<ASTNode> condition;
	std::unique_ptr<ASTNode> then_branch;
	std::unique_ptr<ASTNode> else_branch; // optional ?
};

class BlockNode : public ASTNode {
public:
	void pretty_print(int level) override;
	void add_statement(std::unique_ptr<ASTNode> statement);

private:
	std::vector<std::unique_ptr<ASTNode>> statements;
};

class ErrorNode : public ASTNode {
public:
	ErrorNode(std::string message)
		: message(message) {}

	void pretty_print(int level) override;

private:
	std::string message;
};
