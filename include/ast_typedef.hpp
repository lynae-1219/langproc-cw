#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class TypedefDeclaration : public Node {
private:
    TypeSpecifier base_type_;
    std::string new_name_;
    bool is_pointer_;

public:
    TypedefDeclaration(TypeSpecifier base_type, std::string new_name, bool is_pointer = false)
        : base_type_(base_type), new_name_(std::move(new_name)), is_pointer_(is_pointer) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return SpecifierToContextType(base_type_);
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

}
