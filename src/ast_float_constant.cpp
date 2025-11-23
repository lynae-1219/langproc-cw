#include <iomanip>
#include "ast_float_constant.hpp" 
#include "ast_context.hpp"  

namespace ast {


void FloatConstant::EmitRISC(std::ostream& stream, Context& context) const {
    int label_id = context.GetUniqueLabelId();
    stream << ".section .rodata" << std::endl;
    stream << ".LC" << label_id << ":" << std::endl;
    stream << "  .float " << std::fixed << std::setprecision(6) << value_ << std::endl;
    stream << ".text" << std::endl;
    stream << "  la t0, .LC" << label_id << std::endl;
    stream << "  flw fa0, 0(t0)" << std::endl;
}

void FloatConstant::Print(std::ostream& stream) const {
    stream << value_;
}

} 