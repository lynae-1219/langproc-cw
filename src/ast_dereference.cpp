#include "ast_dereference.hpp"
#include "ast_context.hpp"

namespace ast {

void Dereference::EmitRISC(std::ostream& stream, Context& context) const {
    Context::Type type = operand_->GetType(context);
    operand_->EmitRISC(stream, context);
    if (type == Context::Type::CHAR) {
        stream << "  lbu a0, 0(a0)" << std::endl;
    } else {
        stream << "  lw a0, 0(a0)" << std::endl;
    }
}

void Dereference::EmitAddress(std::ostream& stream, Context& context) const {
    operand_->EmitRISC(stream, context);
}

void Dereference::Print(std::ostream& stream) const {
    stream << "(*";
    operand_->Print(stream);
    stream << ")";
}

}
