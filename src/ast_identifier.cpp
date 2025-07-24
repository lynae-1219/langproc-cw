#include "ast_identifier.hpp"

namespace ast {

Identifier::Identifier(std::string identifier) 
    : identifier_(std::move(identifier)) {}

void Identifier::EmitRISC(std::ostream& stream, Context& context) const {
    int offset = context.GetVariableOffset(identifier_);
    if (offset >= 0) {
        stream << "lw a0, " << offset << "(sp)\n";
    }
}

void Identifier::Print(std::ostream& stream) const {
    stream << identifier_;
}

} // namespace ast