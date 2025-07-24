#include "ast_jump_statement.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"

namespace ast {

ReturnStatement::ReturnStatement(std::unique_ptr<Node> expression)
    : expression_(std::move(expression)) {}

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const {
    if (expression_) {
        expression_->EmitRISC(stream, context);
        
        // If returning a variable, load from stack
        if (dynamic_cast<Identifier*>(expression_.get())) {
            std::string reg = context.AllocRegister();
            stream << "lw " << reg << ", " 
                   << context.GetVariableOffset(dynamic_cast<Identifier*>(expression_.get())->GetName())
                   << "(sp)\n";
            stream << "mv a0, " << reg << "\n";
            context.FreeRegister(reg);
        }
    } else {
        // Default return 0 if no expression
        stream << "li a0, 0\n";
    }
    // Return will be handled by function epilogue
}

void ReturnStatement::Print(std::ostream& stream) const {
    stream << "return";
    if (expression_) {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";\n";
}

} // namespace ast