#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class FunctionDefinition : public Node {
private:
    TypeSpecifier declaration_specifiers_;
    NodePtr declarator_;
    NodePtr parameters_;
    NodePtr compound_statement_;

public:
    FunctionDefinition(TypeSpecifier declaration_specifiers, 
                      NodePtr declarator, 
                      NodePtr parameters,
                      NodePtr compound_statement);

    // A FunctionDefinition node doesn't have a type itself (its return type is part of its specifiers).
    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on FunctionDefinition");
    }
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
    
    std::string GetFunctionName() const;
};

} // namespace ast