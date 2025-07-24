#include "ast_assign.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

void Assign::EmitRISC(std::ostream& stream, Context& context) const {
    // Evaluate the right-hand side, the result will be in a0
    rhs_->EmitRISC(stream, context);
    
    // Get the memory location of the left-hand side and store the result
    auto id = dynamic_cast<Identifier*>(lhs_.get());
    int offset = context.GetVariableOffset(id->GetName());
    stream << "  sw a0, " << offset << "(sp)" << std::endl;
}

void Assign::Print(std::ostream& stream) const {
    lhs_->Print(stream);
    stream << " = ";
    rhs_->Print(stream);
}

} // namespace ast