#include "ast_constant.hpp"

namespace ast {

void IntConstant::EmitRISC(std::ostream& stream, Context& context) const {
    // Suppress unused parameter warning
    (void)context;
    // Load immediate value into register a0 (return value register)
    stream << "  li a0, " << value_ << std::endl;
}

void IntConstant::Print(std::ostream& stream) const {
    stream << value_;
}

} // namespace ast