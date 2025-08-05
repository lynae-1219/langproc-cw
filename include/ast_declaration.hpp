#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class Declaration : public Node {
private:
    TypeSpecifier type_specifier_;
    NodePtr init_declarator_list_;

public:
    Declaration(TypeSpecifier type_specifier, NodePtr init_declarator_list);
    
    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on Declaration");
    }
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    // get method
    const Node* GetDeclarator() const { return init_declarator_list_.get(); }
    TypeSpecifier GetTypeSpecifier() const { return type_specifier_; }
};

} // namespace ast