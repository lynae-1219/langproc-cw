#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class Declaration : public Node {
private:
    TypeSpecifier type_specifier_;
    NodePtr init_declarator_list_;
    bool is_global_;

public:
    Declaration(TypeSpecifier type_specifier, NodePtr init_declarator_list, bool is_global = false);

    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on Declaration");
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const Node* GetDeclarator() const { return init_declarator_list_.get(); }
    TypeSpecifier GetTypeSpecifier() const { return type_specifier_; }
    void SetGlobal(bool is_global) { is_global_ = is_global; }
    bool IsGlobal() const { return is_global_; }
};

}
