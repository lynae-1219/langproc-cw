#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

FunctionDefinition::FunctionDefinition(TypeSpecifier declaration_specifiers,
                                     NodePtr declarator,
                                     NodePtr compound_statement)
    : declaration_specifiers_(declaration_specifiers),
      declarator_(std::move(declarator)),
      compound_statement_(std::move(compound_statement)) {}

std::string FunctionDefinition::GetFunctionName() const {
    if (auto id = dynamic_cast<Identifier*>(declarator_.get())) {
        return id->GetName();
    }
    return "unknown_function";
}

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context) const {
    // Function prologue
    std::string func_name = GetFunctionName();
    stream << ".text\n";
    stream << ".globl " << func_name << "\n";
    stream << func_name << ":\n";
    
    // Setup stack frame
    context.PushScope();
    int stack_size = context.GetCurrentStackSize();
    
    // Allocate stack space (8-byte aligned)
    int total_size = ((stack_size + 15) / 16) * 16; // Round up to 16 bytes
    stream << "addi sp, sp, -" << total_size << "\n";
    
    // Save return address if needed
    if (total_size > 0) {
        stream << "sw ra, " << (total_size - 4) << "(sp)\n";
    }
    
    // Function body
    if (compound_statement_) {
        compound_statement_->EmitRISC(stream, context);
    } else {
        // Default return 0 if no body
        stream << "li a0, 0\n";
    }
    
    // Function epilogue
    if (total_size > 0) {
        stream << "lw ra, " << (total_size - 4) << "(sp)\n";
    }
    stream << "addi sp, sp, " << total_size << "\n";
    stream << "ret\n";
    
    context.PopScope();
}

void FunctionDefinition::Print(std::ostream& stream) const {
    stream << declaration_specifiers_ << " ";
    declarator_->Print(stream);
    stream << "() {\n";
    if (compound_statement_) {
        compound_statement_->Print(stream);
    }
    stream << "}\n";
}

} // namespace ast