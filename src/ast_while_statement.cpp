#include "ast_while_statement.hpp"
#include "ast_context.hpp"

namespace ast {

void WhileStatement::EmitRISC(std::ostream& stream, Context& context) const {
    int label_id = context.GetUniqueLabelId();
    std::string start_label = ".L_WHILE_START_" + std::to_string(label_id);
    std::string end_label = ".L_WHILE_END_" + std::to_string(label_id);

    stream << start_label << ":" << std::endl;

    condition_->EmitRISC(stream, context);

    stream << "  beq a0, zero, " << end_label << std::endl;

    body_->EmitRISC(stream, context);

    stream << "  j " << start_label << std::endl;

    stream << end_label << ":" << std::endl;
}

void WhileStatement::Print(std::ostream& stream) const {
    stream << "while (";
    condition_->Print(stream);
    stream << ") ";
    body_->Print(stream);
}

} // namespace ast