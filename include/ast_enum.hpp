#pragma once
#include "ast_node.hpp"
#include <vector>

namespace ast {

class EnumDeclaration : public Node {
private:
    std::string name_;
    NodePtr enumerator_list_;

public:
    EnumDeclaration(std::string name, NodePtr enumerator_list)
        : name_(std::move(name)), enumerator_list_(std::move(enumerator_list)) {}

    EnumDeclaration(NodePtr enumerator_list)
        : name_(""), enumerator_list_(std::move(enumerator_list)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class Enumerator : public Node {
private:
    std::string name_;
    NodePtr value_;

public:
    Enumerator(std::string name, NodePtr value = nullptr)
        : name_(std::move(name)), value_(std::move(value)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const std::string& GetName() const { return name_; }
    const Node* GetValue() const { return value_.get(); }
};

}
