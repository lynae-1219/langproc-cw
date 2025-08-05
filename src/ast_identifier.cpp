#include "ast_identifier.hpp"

namespace ast {

Identifier::Identifier(std::string identifier) 
    : identifier_(std::move(identifier)) {}

// ADDED: New implementation for GetType
Context::Type Identifier::GetType(Context& context) const {
    return context.GetVariableType(identifier_);
}

// AMENDED: Emits different code based on variable type.
void Identifier::EmitRISC(std::ostream& stream, Context& context) const {
    int offset = context.GetVariableOffset(identifier_);
    Context::Type type = this->GetType(context); // Use our new method

    if (type == Context::Type::INT) {
        // Load an integer variable into an integer register (a0)
        stream << "  lw a0, " << offset << "(sp)" << std::endl;
    } else if (type == Context::Type::FLOAT) {
        // Load a float variable into a float register (fa0)
        stream << "  flw fa0, " << offset << "(sp)" << std::endl;
    }
}

void Identifier::Print(std::ostream& stream) const {
    stream << identifier_;
}

} // namespace ast