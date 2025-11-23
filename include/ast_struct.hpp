#pragma once
#include "ast_node.hpp"
#include <string>
#include <vector>

namespace ast {

class StructDefinition : public Node {
private:
    std::string name_;
    NodePtr members_;

public:
    StructDefinition(std::string name, NodePtr members)
        : name_(std::move(name)), members_(std::move(members)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const std::string& GetName() const { return name_; }
};

class StructMemberAccess : public Node {
private:
    NodePtr object_;
    std::string member_;

public:
    StructMemberAccess(NodePtr object, std::string member)
        : object_(std::move(object)), member_(std::move(member)) {}

    Context::Type GetType(Context& context) const override;
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void EmitAddress(std::ostream& stream, Context& context) const;
    void Print(std::ostream& stream) const override;

    const std::string& GetMember() const { return member_; }
    const Node* GetObject() const { return object_.get(); }
};

class StructVariableDecl : public Node {
private:
    std::string struct_name_;
    std::string var_name_;

public:
    StructVariableDecl(std::string struct_name, std::string var_name)
        : struct_name_(std::move(struct_name)), var_name_(std::move(var_name)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::STRUCT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

}
