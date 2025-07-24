#include "ast_compound_statement.hpp"
#include "ast_context.hpp"

namespace ast {

void CompoundStatement::EmitRISC(std::ostream& stream, Context& context) const {
    context.PushScope();
    if (statement_list_) {
        statement_list_->EmitRISC(stream, context);
    }
    context.PopScope();
}

void CompoundStatement::Print(std::ostream& stream) const {
    stream << "{\n";
    if (statement_list_) {
        statement_list_->Print(stream);
    }
    stream << "}\n";
}

} // namespace ast