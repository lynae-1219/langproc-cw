#include "ast_function_definition.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_declaration.hpp"
#include <stdexcept>
#include <sstream>

namespace ast {

FunctionDefinition::FunctionDefinition(TypeSpecifier declaration_specifiers,
                                       NodePtr declarator,
                                       NodePtr parameters,
                                       NodePtr compound_statement)
    : declaration_specifiers_(declaration_specifiers),
      declarator_(std::move(declarator)),
      parameters_(std::move(parameters)),
      compound_statement_(std::move(compound_statement)) {}

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context) const {
    context.PushScope(); // Push scope for parameters and locals

    std::vector<std::string> arg_registers = {"a0", "a1", "a2", "a3"};
    if (parameters_) {
        if(auto param_list = dynamic_cast<const NodeList*>(parameters_.get())) {
            int i = 0;
            for(const auto& param_node : param_list->GetNodes()) {
                if (const auto* decl = dynamic_cast<const Declaration*>(param_node.get())) {
                    if (const auto* id = dynamic_cast<const Identifier*>(decl->GetDeclarator())) {
                        context.AddVariable(id->GetName(), Context::Type::INT);
                        stream << "  sw " << arg_registers[i] << ", " << context.GetVariableOffset(id->GetName()) << "(sp) # Store parameter " << id->GetName() << std::endl;
                        i++;
                    }
                }
            }
        }
    }
    
    std::stringstream body_stream;
    compound_statement_->EmitRISC(body_stream, context);

    int stack_size = context.GetMaxStackSize();
    if (stack_size == 0) { stack_size = 16; }

    stream << ".globl " << GetFunctionName() << std::endl;
    stream << GetFunctionName() << ":" << std::endl;
    stream << "  addi sp, sp, -" << stack_size << std::endl;
    stream << "  sw ra, " << stack_size - 4 << "(sp)" << std::endl;
    stream << body_stream.str();
    stream << "  lw ra, " << stack_size - 4 << "(sp)" << std::endl;
    stream << "  addi sp, sp, " << stack_size << std::endl;
    stream << "  ret" << std::endl;

    context.PopScope();
}

void FunctionDefinition::Print(std::ostream& stream) const {
    stream << declaration_specifiers_ << " ";
    declarator_->Print(stream);
    stream << " ";
    compound_statement_->Print(stream);
}

std::string FunctionDefinition::GetFunctionName() const {
    if (const auto* dd = dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
        if (const auto* id = dynamic_cast<const Identifier*>(dd->GetIdentifier())) {
            return id->GetName();
        }
    }
    throw std::runtime_error("Function name not found in declarator");
}

} // namespace ast