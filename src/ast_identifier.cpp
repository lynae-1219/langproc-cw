#include "ast_identifier.hpp"

namespace ast {

Identifier::Identifier(std::string identifier) 
    : identifier_(std::move(identifier)) {}

Context::Type Identifier::GetType(Context& context) const {
    return context.GetVariableType(identifier_);
}


void Identifier::EmitRISC(std::ostream& stream, Context& context) const {
    int offset = context.GetVariableOffset(identifier_);
    Context::Type type = this->GetType(context);

    if (type == Context::Type::INT) {
        stream << "  lw a0, " << offset << "(sp)" << std::endl;
    } else if (type == Context::Type::FLOAT) {
        stream << "  flw fa0, " << offset << "(sp)" << std::endl;
    }
}

void Identifier::Print(std::ostream& stream) const {
    stream << identifier_;
}

} // namespace ast