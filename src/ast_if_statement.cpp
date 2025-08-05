#include "ast_if_statement.hpp"

namespace ast {

void IfStatement::EmitRISC(std::ostream& stream, Context& context) const {
    int label_id = context.GetUniqueLabelId();
    std::string else_label = ".L_IF_ELSE_" + std::to_string(label_id);
    std::string end_label = ".L_IF_END_" + std::to_string(label_id);

    condition_->EmitRISC(stream, context);

    stream << "  beq a0, zero, " << else_label << std::endl;

    then_statement_->EmitRISC(stream, context);

    stream << "  j " << end_label << std::endl;

    
    stream << else_label << ":" << std::endl;
    if (else_statement_) {
        else_statement_->EmitRISC(stream, context);
    }

    stream << end_label << ":" << std::endl;
}

void IfStatement::Print(std::ostream& stream) const {
    stream << "if (";
    condition_->Print(stream);
    stream << ") ";
    then_statement_->Print(stream);
    if (else_statement_) {
        stream << " else ";
        else_statement_->Print(stream);
    }
}

} // namespace ast