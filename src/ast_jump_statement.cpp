#include "ast_jump_statement.hpp"

namespace ast {

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    if (expression_ != nullptr)
    {
        expression_->EmitRISC(stream, context);
        // Assuming result is in a0 
    }
    stream << "ret" << std::endl;
}

void ReturnStatement::Print(std::ostream& stream) const
{
    stream << "return";
    if (expression_ != nullptr)
    {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";" << std::endl;
}

void BreakStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    (void)context; // Mark context as unused to suppress warning
    stream << "# break statement - loop label tracking needed" << std::endl;
    stream << "j BREAK_LABEL" << std::endl; // Placeholder
}

void BreakStatement::Print(std::ostream& stream) const
{
    stream << "break;" << std::endl;
}

void ContinueStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    (void)context; // Mark context as unused to suppress warning
    // This feels bad placeholder
    stream << "# continue statement - loop label tracking needed" << std::endl;
    stream << "j CONTINUE_LABEL" << std::endl; // Placeholder
}

void ContinueStatement::Print(std::ostream& stream) const
{
    stream << "continue;" << std::endl;
}

void GotoStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    (void)context; // Mark context as unused to suppress warning
    stream << "j " << label_ << std::endl;
}

void GotoStatement::Print(std::ostream& stream) const
{
    stream << "goto " << label_ << ";" << std::endl;
}

void LabelStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    stream << label_ << ":" << std::endl;
    if (statement_ != nullptr)
    {
        statement_->EmitRISC(stream, context);
    }
}

void LabelStatement::Print(std::ostream& stream) const
{
    stream << label_ << ": ";
    if (statement_ != nullptr)
    {
        statement_->Print(stream);
    }
    else
    {
        stream << ";" << std::endl;
    }
}

} // namespace ast