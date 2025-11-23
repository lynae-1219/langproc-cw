#include "ast_jump_statement.hpp"
#include "ast_context.hpp"
#include "ast_constant.hpp"
#include "ast_node.hpp"
#include <map>

namespace ast {


ReturnStatement::ReturnStatement(std::unique_ptr<Node> expression)
    : expression_(std::move(expression)) {}

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const {
    if (expression_) {

        expression_->EmitRISC(stream, context);
    } else {
        // If no return expression (e.g., `return;`), default to returning 0.
        stream << "  li a0, 0\n";
    }

    stream << "  j " << context.GetEpilogueLabel() << std::endl;
}

void ReturnStatement::Print(std::ostream& stream) const {
    stream << "return";
    if (expression_) {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";\n";
}

void BreakStatement::EmitRISC(std::ostream& stream, Context& context) const {
    stream << "  j " << context.GetBreakLabel() << std::endl;
}
void BreakStatement::Print(std::ostream& stream) const {
    stream << "break;\n";
}

void ContinueStatement::EmitRISC(std::ostream& stream, Context& context) const {
    stream << "  j " << context.GetContinueLabel() << std::endl;
}
void ContinueStatement::Print(std::ostream& stream) const {
    stream << "continue;\n";
}

CaseStatement::CaseStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void CaseStatement::EmitRISC(std::ostream& stream, Context& context) const {
    if (statement_) statement_->EmitRISC(stream, context);
}

int CaseStatement::GetCaseValue() const {
    if (auto* ic = dynamic_cast<const IntConstant*>(expression_.get())) {
        return ic->GetValue();
    }
    return 0;
}

void CaseStatement::Print(std::ostream& stream) const {
    stream << "case ";
    expression_->Print(stream);
    stream << ": ";
    statement_->Print(stream);
}

DefaultCaseStatement::DefaultCaseStatement(std::unique_ptr<Node> statement)
    : statement_(std::move(statement)) {}

void DefaultCaseStatement::EmitRISC(std::ostream& stream, Context& context) const {
    if (statement_) statement_->EmitRISC(stream, context);
}
void DefaultCaseStatement::Print(std::ostream& stream) const {
    stream << "default: ";
    statement_->Print(stream);
}

SwitchStatement::SwitchStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void SwitchStatement::EmitRISC(std::ostream& stream, Context& context) const {
    int label_id = context.GetUniqueLabelId();
    std::string end_label = ".L_SWITCH_END_" + std::to_string(label_id);

    std::string saved_break = context.GetBreakLabel();
    context.SetBreakLabel(end_label);

    expression_->EmitRISC(stream, context);
    stream << "  mv t2, a0" << std::endl;

    auto* body_list = dynamic_cast<const NodeList*>(statement_.get());
    if (!body_list) {
        context.SetBreakLabel(saved_break);
        stream << end_label << ":" << std::endl;
        return;
    }

    std::vector<std::pair<int, std::string>> case_labels;
    std::string default_label;
    std::map<size_t, std::string> idx_to_label;
    int case_num = 0;

    for (size_t i = 0; i < body_list->GetNodes().size(); ++i) {
        const auto& node = body_list->GetNodes()[i];

        if (auto* cs = dynamic_cast<const CaseStatement*>(node.get())) {
            std::string lbl = ".L_CASE_" + std::to_string(label_id) + "_" + std::to_string(case_num++);
            case_labels.push_back({cs->GetCaseValue(), lbl});
            idx_to_label[i] = lbl;
        } else if (dynamic_cast<const DefaultCaseStatement*>(node.get())) {
            std::string lbl = ".L_DEFAULT_" + std::to_string(label_id);
            default_label = lbl;
            idx_to_label[i] = lbl;
        }
    }

    for (const auto& [val, label] : case_labels) {
        stream << "  li t0, " << val << std::endl;
        stream << "  beq t2, t0, " << label << std::endl;
    }

    if (!default_label.empty()) {
        stream << "  j " << default_label << std::endl;
    } else {
        stream << "  j " << end_label << std::endl;
    }

    for (size_t i = 0; i < body_list->GetNodes().size(); ++i) {
        const auto& node = body_list->GetNodes()[i];
        if (idx_to_label.count(i)) {
            stream << idx_to_label[i] << ":" << std::endl;
        }
        node->EmitRISC(stream, context);
    }

    stream << end_label << ":" << std::endl;
    context.SetBreakLabel(saved_break);
}
void SwitchStatement::Print(std::ostream& stream) const {
    stream << "switch (";
    expression_->Print(stream);
    stream << ") ";
    statement_->Print(stream);
}

} // namespace ast