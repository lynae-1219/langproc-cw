#include "ast_identifier.hpp"

namespace ast {

Identifier::Identifier(std::string identifier) 
    : identifier_(std::move(identifier)) {}

void Identifier::EmitRISC(std::ostream& stream, Context& context) const {
    // When an identifier is used in an expression, load its value from the stack.
    int offset = context.GetVariableOffset(identifier_);
    stream << "  lw a0, " << offset << "(sp)" << std::endl;
}

void Identifier::Print(std::ostream& stream) const {
    stream << identifier_;
}

} // namespace ast