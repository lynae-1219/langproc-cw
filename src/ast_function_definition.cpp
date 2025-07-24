#include "ast_function_definition.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"
#include <stdexcept>

namespace ast {

FunctionDefinition::FunctionDefinition(TypeSpecifier declaration_specifiers,
                                       NodePtr declarator,
                                       NodePtr compound_statement)
    : declaration_specifiers_(declaration_specifiers),
      declarator_(std::move(declarator)),
      compound_statement_(std::move(compound_statement)) {}

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context) const {
    // 1. Emit function label and directive
    stream << ".globl " << GetFunctionName() << std::endl;
    stream << GetFunctionName() << ":" << std::endl;

    // 2. Function Prologue
    // Reserve space on the stack. For now, a fixed size.
    // A more advanced compiler would calculate this based on variables.
    int stack_size = 16; // Basic space for return address and frame pointer
    stream << "  addi sp, sp, -" << stack_size << std::endl;
    stream << "  sw ra, " << stack_size - 4 << "(sp)" << std::endl; // Save return address

    // 3. Emit code for the function body
    compound_statement_->EmitRISC(stream, context);

    // 4. Function Epilogue
    stream << ".L_return_" << GetFunctionName() << ":" << std::endl;
    stream << "  lw ra, " << stack_size - 4 << "(sp)" << std::endl; // Restore return address
    stream << "  addi sp, sp, " << stack_size << std::endl;
    stream << "  ret" << std::endl;
}

void FunctionDefinition::Print(std::ostream& stream) const {
    stream << declaration_specifiers_ << " ";
    declarator_->Print(stream);
    stream << " ";
    compound_statement_->Print(stream);
}

// Helper to get the function name from the declarator
std::string FunctionDefinition::GetFunctionName() const {
    if (auto id = dynamic_cast<Identifier*>(declarator_.get())) {
        return id->GetName();
    }
    // Handle other declarator types if necessary
    throw std::runtime_error("Unsupported declarator type for function name extraction");
}

} // namespace ast