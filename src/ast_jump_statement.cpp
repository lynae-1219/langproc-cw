
#include "ast_jump_statement.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"

namespace ast {


ReturnStatement::ReturnStatement(std::unique_ptr<Node> expression)
    : expression_(std::move(expression)) {}

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const {
    if (expression_) {
        expression_->EmitRISC(stream, context);
    } else {
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
    (void)context;
    stream << "# BreakStatement EmitRISC" << std::endl;
}
void BreakStatement::Print(std::ostream& stream) const {
    stream << "break;\n";
}


void ContinueStatement::EmitRISC(std::ostream& stream, Context& context) const {
    (void)context;
    stream << "# ContinueStatement EmitRISC" << std::endl;
}
void ContinueStatement::Print(std::ostream& stream) const {
    stream << "continue;\n";
}


CaseStatement::CaseStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void CaseStatement::EmitRISC(std::ostream& stream, Context& context) const {
    expression_->EmitRISC(stream, context);
    statement_->EmitRISC(stream, context);
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
    statement_->EmitRISC(stream, context);
}
void DefaultCaseStatement::Print(std::ostream& stream) const {
    stream << "default: ";
    statement_->Print(stream);
}


SwitchStatement::SwitchStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void SwitchStatement::EmitRISC(std::ostream& stream, Context& context) const {
    expression_->EmitRISC(stream, context);
    statement_->EmitRISC(stream, context);
}
void SwitchStatement::Print(std::ostream& stream) const {
    stream << "switch (";
    expression_->Print(stream);
    stream << ") ";
    statement_->Print(stream);
}

} // namespace ast