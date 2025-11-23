#include "ast_string_literal.hpp"

namespace ast {

int StringLiteral::string_counter_ = 0;

void StringLiteral::EmitRISC(std::ostream& stream, Context& context) const {
    int id = context.GetUniqueLabelId();
    std::string label = ".LC" + std::to_string(id);

    stream << "  .section .rodata" << std::endl;
    stream << label << ":" << std::endl;
    stream << "  .string " << value_ << std::endl;
    stream << "  .text" << std::endl;
    stream << "  la a0, " << label << std::endl;
}

void StringLiteral::Print(std::ostream& stream) const {
    stream << value_;
}

}
