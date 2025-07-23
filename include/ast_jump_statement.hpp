#pragma once

#include "ast_node.hpp"
#include <memory>
#include <string>

namespace ast {

class ReturnStatement : public Node
{
public:
    ReturnStatement(std::unique_ptr<Node> expression);
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

private:
    std::unique_ptr<Node> expression_;
};

class BreakStatement : public Node
{
public:
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class ContinueStatement : public Node
{
public:
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class CaseStatement : public Node
{
public:
    CaseStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement);
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

private:
    std::unique_ptr<Node> expression_;
    std::unique_ptr<Node> statement_;
};

class DefaultCaseStatement : public Node
{
public:
    DefaultCaseStatement(std::unique_ptr<Node> statement);
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

private:
    std::unique_ptr<Node> statement_;
};

class SwitchStatement : public Node
{
public:
    SwitchStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement);
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

private:
    std::unique_ptr<Node> expression_;
    std::unique_ptr<Node> statement_;
};

} // namespace ast