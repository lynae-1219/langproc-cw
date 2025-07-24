// ast_init_declarator.cpp
#include "ast_init_declarator.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"
#include "ast_direct_declarator.hpp" // Added

namespace ast {

InitDeclarator::InitDeclarator(NodePtr declarator, NodePtr initializer)
    : declarator_(std::move(declarator)), initializer_(std::move(initializer)) {}

void InitDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
    // Find the identifier name
    const Identifier* id = nullptr;
    if (auto dd = dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
        id = dynamic_cast<const Identifier*>(dd->GetIdentifier());
    } else if (auto ident = dynamic_cast<const Identifier*>(declarator_.get())) {
        id = ident;
    }
    
    if (id) {
        // Add the variable to the context first
        context.AddVariable(id->GetName(), Context::Type::INT);
        
        // If there's an initializer, evaluate it and store the result
        if (initializer_) {
            initializer_->EmitRISC(stream, context); // Result of RHS is in a0
            int offset = context.GetVariableOffset(id->GetName());
            stream << "  sw a0, " << offset << "(sp)" << std::endl;
        }
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