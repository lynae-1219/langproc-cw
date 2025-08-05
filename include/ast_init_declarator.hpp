#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp" // For TypeSpecifier

namespace ast {

class InitDeclarator : public Node {
private:
    NodePtr declarator_;
    NodePtr initializer_;

public:
    InitDeclarator(NodePtr declarator, NodePtr initializer);
    
    // An InitDeclarator doesn't have a type itself.
    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on InitDeclarator");
    }

    // AMENDED: EmitRISC is now only responsible for the initializer part.
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    // Helper to get the variable name
    std::string GetName() const;
};

} // namespace ast