#include "ast_function_definition.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_declaration.hpp"
#include "ast_init_declarator.hpp"
#include "ast_node.hpp" // For NodeList
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
    std::string func_name = GetFunctionName();
    std::string epilogue_label = ".L_epilogue_" + func_name;
    
    context.SetEpilogueLabel(epilogue_label);

    std::stringstream body_stream;
    context.PushScope(); 

    // Handle parameters
    std::vector<std::string> arg_registers = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"};
    if (parameters_) {
        if(auto param_list = dynamic_cast<const NodeList*>(parameters_.get())) {
            int i = 0;
            for(const auto& param_node : param_list->GetNodes()) {
                if (const auto* decl = dynamic_cast<const Declaration*>(param_node.get())) {
                    const auto* init_list = dynamic_cast<const NodeList*>(decl->GetDeclarator());
                    const auto* init_decl = dynamic_cast<const InitDeclarator*>(init_list->GetNodes()[0].get());
                    const auto* dir_decl = dynamic_cast<const DirectDeclarator*>(init_decl->GetDeclarator());
                    const auto* id = dynamic_cast<const Identifier*>(dir_decl->GetIdentifier());

                    context.AddVariable(id->GetName(), Context::Type::INT);
                    int offset = context.GetVariableOffset(id->GetName());
                    body_stream << "  sw " << arg_registers[i] << ", " << offset << "(sp) # Store parameter " << id->GetName() << std::endl;
                    i++;
                }
            }
        }
    }
    
    compound_statement_->EmitRISC(body_stream, context);

    int stack_size = context.GetMaxStackSize();

    stream << ".globl " << func_name << std::endl;
    stream << func_name << ":" << std::endl;
    stream << "  addi sp, sp, -" << stack_size << std::endl;
    stream << "  sw ra, " << stack_size - 4 << "(sp)" << std::endl;
    stream << body_stream.str();

    stream << epilogue_label << ":" << std::endl;
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
    const Node* current = declarator_.get();
    while(auto* dd = dynamic_cast<const DirectDeclarator*>(current)) {
        if(auto* id = dynamic_cast<const Identifier*>(dd->GetIdentifier())) {
            return id->GetName();
        }
        // This is a simplification; a more robust solution would trace through declarator types.
        break; 
    }
    throw std::runtime_error("Function name not found in declarator");
}

} // namespace ast