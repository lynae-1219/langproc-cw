#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class FunctionDefinition : public Node {
private:
    const TypeSpecifier declaration_specifiers_;
    NodePtr declarator_;
    NodePtr parameters_; // Added
    NodePtr compound_statement_;

public:
    FunctionDefinition(TypeSpecifier declaration_specifiers, 
                      NodePtr declarator, 
                      NodePtr parameters, // Added
                      NodePtr compound_statement);
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
    
    std::string GetFunctionName() const;
};

} // namespace ast