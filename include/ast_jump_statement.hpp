#pragma once

#include "ast_node.hpp"

namespace ast {

class ReturnStatement : public Node
{
private:
    NodePtr expression_;

public:
    ReturnStatement(NodePtr expression) : expression_(std::move(expression)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class BreakStatement : public Node
{
public:
    BreakStatement() = default;

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class ContinueStatement : public Node
{
public:
    ContinueStatement() = default;

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class GotoStatement : public Node
{
private:
    std::string label_;

public:
    GotoStatement(const std::string& label) : label_(label) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class LabelStatement : public Node
{
private:
    std::string label_;
    NodePtr statement_;

public:
    LabelStatement(const std::string& label, NodePtr statement)
        : label_(label), statement_(std::move(statement)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast