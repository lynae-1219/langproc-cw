#include "ast_jump_statement.hpp"
#include "ast_context.hpp"

namespace ast {

// ReturnStatement
ReturnStatement::ReturnStatement(std::unique_ptr<Node> expression)
    : expression_(std::move(expression)) {}

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const {
    if (expression_) {
        // Evaluate the return expression. The result will be placed in a0 (for int)
        // or fa0 (for float) by the expression node itself.
        expression_->EmitRISC(stream, context);
    } else {
        // If no return expression (e.g., `return;`), default to returning 0.
        stream << "  li a0, 0\n";
    }

    // Unconditionally jump to the function's epilogue for a clean exit.
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

// BreakStatement
void BreakStatement::EmitRISC(std::ostream& stream, Context& context) const {
    (void)context; // context is unused for now
    stream << "# BreakStatement: Not yet implemented" << std::endl;
}
void BreakStatement::Print(std::ostream& stream) const {
    stream << "break;\n";
}

// ContinueStatement
void ContinueStatement::EmitRISC(std::ostream& stream, Context& context) const {
    (void)context; // context is unused for now
    stream << "# ContinueStatement: Not yet implemented" << std::endl;
}
void ContinueStatement::Print(std::ostream& stream) const {
    stream << "continue;\n";
}

// CaseStatement
CaseStatement::CaseStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void CaseStatement::EmitRISC(std::ostream& stream, Context& context) const {
    (void)context; // context is unused for now
    stream << "# CaseStatement: Not yet implemented" << std::endl;
    if (expression_) expression_->EmitRISC(stream, context);
    if (statement_) statement_->EmitRISC(stream, context);
}
void CaseStatement::Print(std::ostream& stream) const {
    stream << "case ";
    expression_->Print(stream);
    stream << ": ";
    statement_->Print(stream);
}

// DefaultCaseStatement
DefaultCaseStatement::DefaultCaseStatement(std::unique_ptr<Node> statement)
    : statement_(std::move(statement)) {}

void DefaultCaseStatement::EmitRISC(std::ostream& stream, Context& context) const {
    (void)context; // context is unused for now
    stream << "# DefaultCaseStatement: Not yet implemented" << std::endl;
    if (statement_) statement_->EmitRISC(stream, context);
}
void DefaultCaseStatement::Print(std::ostream& stream) const {
    stream << "default: ";
    statement_->Print(stream);
}

// SwitchStatement
SwitchStatement::SwitchStatement(std::unique_ptr<Node> expression, std::unique_ptr<Node> statement)
    : expression_(std::move(expression)), statement_(std::move(statement)) {}

void SwitchStatement::EmitRISC(std::ostream& stream, Context& context) const {
    (void)context; // context is unused for now
    stream << "# SwitchStatement: Not yet implemented" << std::endl;
    if (expression_) expression_->EmitRISC(stream, context);
    if (statement_) statement_->EmitRISC(stream, context);
}
void SwitchStatement::Print(std::ostream& stream) const {
    stream << "switch (";
    expression_->Print(stream);
    stream << ") ";
    statement_->Print(stream);
}

} // namespace ast