#include "ast_init_declarator.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_context.hpp"

namespace ast {

InitDeclarator::InitDeclarator(NodePtr declarator, NodePtr initializer)
    : declarator_(std::move(declarator)), initializer_(std::move(initializer)) {}

// AMENDED: This logic now only handles emitting the code for the initializer,
// as the variable has already been added to the context by the parent Declaration node.
void InitDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
    if (initializer_) {
        // Evaluate the initializer expression (e.g., the '10' in 'int x = 10;')
        initializer_->EmitRISC(stream, context);

        // Get variable info from context
        std::string var_name = GetName();
        int offset = context.GetVariableOffset(var_name);
        Context::Type type = context.GetVariableType(var_name);

        // Use the correct store instruction based on type
        if (type == Context::Type::INT) {
            stream << "  sw a0, " << offset << "(sp)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  fsw fa0, " << offset << "(sp)" << std::endl;
        }
    }
    // If no initializer, do nothing. The space is already allocated.
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

std::string InitDeclarator::GetName() const {
    // This logic navigates the declarator to find the identifier string.
    if (const auto* dd = dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
        if (const auto* id = dynamic_cast<const Identifier*>(dd->GetIdentifier())) {
            return id->GetName();
        }
    }
    throw std::runtime_error("Could not find name in InitDeclarator");
}

} // namespace ast