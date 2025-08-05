#include "ast_assign.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

void Assign::EmitRISC(std::ostream& stream, Context& context) const {
    rhs_->EmitRISC(stream, context);
    
    auto id = dynamic_cast<const Identifier*>(lhs_.get());
    if (!id) {
        throw std::runtime_error("LHS of assignment is not an identifier.");
    }
    const std::string& var_name = id->GetName();
    int offset = context.GetVariableOffset(var_name);
    Context::Type type = context.GetVariableType(var_name);

    if (type == Context::Type::INT) {
        stream << "  sw a0, " << offset << "(s0)" << std::endl;
    } else if (type == Context::Type::FLOAT) {
        stream << "  fsw fa0, " << offset << "(s0)" << std::endl;
    } else if (type == Context::Type::DOUBLE) {
        stream << "  fsd fa0, " << offset << "(s0)" << std::endl;
    }
}

void Assign::Print(std::ostream& stream) const {
    lhs_->Print(stream);
    stream << " = ";
    rhs_->Print(stream);
}

} // namespace ast