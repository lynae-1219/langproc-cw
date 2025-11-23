#include "ast_for_statement.hpp"

namespace ast {

void ForStatement::EmitRISC(std::ostream& stream, Context& context) const {
    int label_id = context.GetUniqueLabelId();
    std::string start_label = ".L_FOR_START_" + std::to_string(label_id);
    std::string end_label = ".L_FOR_END_" + std::to_string(label_id);

    std::string saved_break = context.GetBreakLabel();
    std::string saved_continue = context.GetContinueLabel();
    context.SetBreakLabel(end_label);
    context.SetContinueLabel(start_label);

    if (init_) {
        init_->EmitRISC(stream, context);
    }

    stream << start_label << ":" << std::endl;

    if (condition_) {
        condition_->EmitRISC(stream, context);
        stream << "  beq a0, zero, " << end_label << std::endl;
    }

    if (body_) {
        body_->EmitRISC(stream, context);
    }

    if (update_) {
        update_->EmitRISC(stream, context);
    }

    stream << "  j " << start_label << std::endl;
    stream << end_label << ":" << std::endl;

    context.SetBreakLabel(saved_break);
    context.SetContinueLabel(saved_continue);
}

void ForStatement::Print(std::ostream& stream) const {
    stream << "for (";
    if (init_) init_->Print(stream);
    stream << "; ";
    if (condition_) condition_->Print(stream);
    stream << "; ";
    if (update_) update_->Print(stream);
    stream << ") ";
    if (body_) body_->Print(stream);
}

}
