#include "ast_unary_expression.hpp"
#include "ast_context.hpp"

namespace ast {

void UnaryExpression::EmitRISC(std::ostream& stream, Context& context) const {
    operand_->EmitRISC(stream, context);
    Context::Type type = operand_->GetType(context);

    if (op_ == "-") {
        if (type == Context::Type::DOUBLE) {
            stream << "  fneg.d fa0, fa0" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  fneg.s fa0, fa0" << std::endl;
        } else {
            stream << "  neg a0, a0" << std::endl;
        }
    } else if (op_ == "!") {
        stream << "  seqz a0, a0" << std::endl;
    } else if (op_ == "~") {
        stream << "  not a0, a0" << std::endl;
    } else if (op_ == "+") {
    }
}

void UnaryExpression::Print(std::ostream& stream) const {
    stream << "(" << op_;
    operand_->Print(stream);
    stream << ")";
}

}
