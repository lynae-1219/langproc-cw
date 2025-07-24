#include "ast_init_declarator.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

InitDeclarator::InitDeclarator(NodePtr declarator, NodePtr initializer)
    : declarator_(std::move(declarator)), initializer_(std::move(initializer)) {}

void InitDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
    if (auto id = dynamic_cast<Identifier*>(declarator_.get())) {
        context.AddVariable(id->GetName(), Context::Type::INT, 4);
        int offset = context.GetVariableOffset(id->GetName());
        
        if (initializer_) {
            initializer_->EmitRISC(stream, context);
            stream << "sw a0, " << offset << "(sp)\n";
        } else {
            stream << "sw zero, " << offset << "(sp)\n";
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