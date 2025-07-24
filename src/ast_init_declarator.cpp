#include "ast_init_declarator.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

InitDeclarator::InitDeclarator(NodePtr declarator, NodePtr initializer)
    : declarator_(std::move(declarator)), initializer_(std::move(initializer)) {}

void InitDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
    if (auto id = dynamic_cast<Identifier*>(declarator_.get())) {
        // Add the variable to the context first
        context.AddVariable(id->GetName(), Context::Type::INT);
        int offset = context.GetVariableOffset(id->GetName());
        
        // If there's an initializer, evaluate it and store the result
        if (initializer_) {
            initializer_->EmitRISC(stream, context); // Result of RHS is in a0
            stream << "  sw a0, " << offset << "(sp)" << std::endl;
        }
        // If no initializer, it's just allocated space (no need to store zero)
    }
}

void InitDeclarator::Print(std::ostream& stream) const {
    if (declarator_) {
        declarator_->Print(stream);
    }
    if (initializer_) {
        stream << " = ";
        initializer_->Print(stream);
    }
}

} // namespace ast