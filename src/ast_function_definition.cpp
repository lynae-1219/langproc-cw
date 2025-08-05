#include "ast_function_definition.hpp"
#include "ast_context.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_declaration.hpp"
#include "ast_init_declarator.hpp"
#include "ast_node.hpp" 
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
    

    context.max_stack_offset_ = 0;

    context.current_stack_offset_ = 8; 

    context.SetEpilogueLabel(epilogue_label);

  
    context.PushScope();

    std::stringstream body_stream;
    

    std::vector<std::string> int_arg_regs = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"};
    std::vector<std::string> float_arg_regs = {"fa0", "fa1", "fa2", "fa3", "fa4", "fa5", "fa6", "fa7"};
    size_t int_arg_count = 0;
    size_t float_arg_count = 0;

    if (parameters_) {
        if(auto param_list = dynamic_cast<const NodeList*>(parameters_.get())) {
            for(const auto& param_node : param_list->GetNodes()) {
                const auto* decl = dynamic_cast<const Declaration*>(param_node.get());
                const auto* init_list = dynamic_cast<const NodeList*>(decl->GetDeclarator());
                const auto* init_decl = dynamic_cast<const InitDeclarator*>(init_list->GetNodes()[0].get());
                
                std::string param_name = init_decl->GetName();
                Context::Type param_type = SpecifierToContextType(decl->GetTypeSpecifier());
                
                context.AddVariable(param_name, param_type);
                int offset = context.GetVariableOffset(param_name);

                if (param_type == Context::Type::INT) {
                    if (int_arg_count < int_arg_regs.size()) {
                        body_stream << "  sw " << int_arg_regs[int_arg_count++] << ", " << offset << "(s0)\n";
                    }
                } else if (param_type == Context::Type::FLOAT || param_type == Context::Type::DOUBLE) {
                    if (float_arg_count < float_arg_regs.size()) {
                        if (param_type == Context::Type::FLOAT) {
                           body_stream << "  fsw " << float_arg_regs[float_arg_count++] << ", " << offset << "(s0)\n";
                        } else { // DOUBLE
                           body_stream << "  fsd " << float_arg_regs[float_arg_count++] << ", " << offset << "(s0)\n";
                        }
                    }
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
    stream << "  sw s0, " << stack_size - 8 << "(sp)" << std::endl;
    stream << "  addi s0, sp, " << stack_size << std::endl;

    stream << body_stream.str();


    stream << epilogue_label << ":" << std::endl;
    stream << "  lw ra, " << stack_size - 4 << "(sp)" << std::endl;
    stream << "  lw s0, " << stack_size - 8 << "(sp)" << std::endl;
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
        break; 
    }
    throw std::runtime_error("Function name not found in declarator");
}

} // namespace ast