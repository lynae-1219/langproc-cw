#include "ast_update_expression.hpp" 
#include "ast_identifier.hpp"
#include "ast_context.hpp"

//Better than Reaver?
namespace ast {

void UpdateExpression::EmitRISC(std::ostream& stream, Context& context) const {
    auto id = dynamic_cast<const Identifier*>(expr_.get());
    if (!id) {
        throw std::runtime_error("Operand of ++/-- must be an identifier.");
    }
    const std::string& var_name = id->GetName();
    int offset = context.GetVariableOffset(var_name);
    Context::Type type = context.GetVariableType(var_name);

    if (type != Context::Type::INT) {
        throw std::runtime_error("Operand of ++/-- must be of type int.");
    }


    int immediate = is_decrement_ ? -1 : 1;

    if (is_prefix_) {

        stream << "  lw t0, " << offset << "(s0)" << std::endl;
        stream << "  addi a0, t0, " << immediate << std::endl;
        stream << "  sw a0, " << offset << "(s0)" << std::endl;
    } else {

        stream << "  lw a0, " << offset << "(s0)" << std::endl;
        stream << "  addi t0, a0, " << immediate << std::endl;
        stream << "  sw t0, " << offset << "(s0)" << std::endl;
    }
}

void UpdateExpression::Print(std::ostream& stream) const {
    std::string op = is_decrement_ ? "--" : "++";
    if (is_prefix_) {
        stream << op;
        expr_->Print(stream);
    } else {
        expr_->Print(stream);
        stream << op;
    }
}

} // namespace ast