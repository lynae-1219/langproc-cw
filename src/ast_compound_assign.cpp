#include "ast_compound_assign.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

void CompoundAssign::EmitRISC(std::ostream& stream, Context& context) const {
    auto* id = dynamic_cast<const Identifier*>(lhs_.get());
    if (!id) {
        throw std::runtime_error("LHS of compound assignment must be an identifier");
    }

    const std::string& var_name = id->GetName();
    Context::Type type = context.GetVariableType(var_name);
    int offset = context.GetVariableOffset(var_name);
    bool is_global = context.IsGlobalVariable(var_name);

    if (is_global) {
        stream << "  la t1, " << var_name << std::endl;
        if (type == Context::Type::INT) {
            stream << "  lw t0, 0(t1)" << std::endl;
        }
    } else {
        if (type == Context::Type::INT) {
            stream << "  lw t0, " << offset << "(s0)" << std::endl;
        }
    }

    stream << "  addi sp, sp, -4" << std::endl;
    stream << "  sw t0, 0(sp)" << std::endl;

    rhs_->EmitRISC(stream, context);

    stream << "  lw t0, 0(sp)" << std::endl;
    stream << "  addi sp, sp, 4" << std::endl;

    if (op_ == "+=") {
        stream << "  add a0, t0, a0" << std::endl;
    } else if (op_ == "-=") {
        stream << "  sub a0, t0, a0" << std::endl;
    } else if (op_ == "*=") {
        stream << "  mul a0, t0, a0" << std::endl;
    } else if (op_ == "/=") {
        stream << "  div a0, t0, a0" << std::endl;
    } else if (op_ == "%=") {
        stream << "  rem a0, t0, a0" << std::endl;
    }

    if (is_global) {
        stream << "  la t0, " << var_name << std::endl;
        stream << "  sw a0, 0(t0)" << std::endl;
    } else {
        stream << "  sw a0, " << offset << "(s0)" << std::endl;
    }
}

void CompoundAssign::Print(std::ostream& stream) const {
    lhs_->Print(stream);
    stream << " " << op_ << " ";
    rhs_->Print(stream);
}

}
