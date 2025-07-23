#include "ast_jump_statement.hpp"
#include "ast_context.hpp"
#include <sstream>

namespace ast {

ReturnStatement::ReturnStatement(std::unique_ptr<Node> expression) 
    : expression_(std::move(expression)) {}

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    if (expression_) {
        expression_->EmitRISC(stream, context);
    }
    stream << "\tret" << std::endl;
}

void ReturnStatement::Print(std::ostream& stream) const
{
    stream << "return";
    if (expression_) {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";" << std::endl;
}

void BreakStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    (void)context; // Mark as used
    stream << "\tj break_label" << std::endl;
}

void BreakStatement::Print(std::ostream& stream) const
{
    stream << "break;" << std::endl;
}

void ContinueStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    (void)context; // Mark as used
    stream << "\tj continue_label" << std::endl;
}

void ContinueStatement::Print(std::ostream& stream) const
{
    stream << "continue;" << std::endl;
}

CaseStatement::CaseStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void CaseStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    std::string case_label = "case_label";
    expression_->EmitRISC(stream, context);
    stream << "\tbeq a0, t0, " << case_label << std::endl;
    stream << case_label << ":" << std::endl;
    statement_->EmitRISC(stream, context);
}

void CaseStatement::Print(std::ostream& stream) const
{
    stream << "case ";
    expression_->Print(stream);
    stream << ": ";
    statement_->Print(stream);
}

DefaultCaseStatement::DefaultCaseStatement(std::unique_ptr<Node> statement)
    : statement_(std::move(statement)) {}

void DefaultCaseStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    std::string default_label = "default_label";
    stream << "\tj " << default_label << std::endl;
    stream << default_label << ":" << std::endl;
    statement_->EmitRISC(stream, context);
}

void DefaultCaseStatement::Print(std::ostream& stream) const
{
    stream << "default: ";
    statement_->Print(stream);
}

SwitchStatement::SwitchStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void SwitchStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    std::string end_label = "switch_end";
    expression_->EmitRISC(stream, context);
    statement_->EmitRISC(stream, context);
    stream << "\tj " << end_label << std::endl;
    stream << end_label << ":" << std::endl;
}

void SwitchStatement::Print(std::ostream& stream) const
{
    stream << "switch (";
    expression_->Print(stream);
    stream << ") { ";
    statement_->Print(stream);
    stream << " }";
}

} // namespace ast