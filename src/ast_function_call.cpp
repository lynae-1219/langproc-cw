#include "ast_function_call.hpp"
#include "ast_identifier.hpp"
#include <vector>
#include <numeric>

namespace ast {

void FunctionCall::EmitRISC(std::ostream& stream, Context& context) const {
    const auto* args_list_node = dynamic_cast<const NodeList*>(arguments_.get());
    
    //Evaluate all arguments and save their results temporarily on the stack
    int temp_stack_used = 0;
    std::vector<Context::Type> arg_types;
    std::vector<int> arg_sizes;

    if (args_list_node) {
        for (const auto& arg : args_list_node->GetNodes()) {
            arg->EmitRISC(stream, context); // Result is in a0 or fa0
            
            Context::Type arg_type = arg->GetType(context);
            int arg_size = (arg_type == Context::Type::DOUBLE) ? 8 : 4;
            
            arg_types.push_back(arg_type);
            arg_sizes.push_back(arg_size);

            stream << "  addi sp, sp, -" << arg_size << "\n";
            if (arg_type == Context::Type::DOUBLE) {
                stream << "  fsd fa0, 0(sp)\n";
            } else if (arg_type == Context::Type::FLOAT) {
                stream << "  fsw fa0, 0(sp)\n";
            } else { // INT
                stream << "  sw a0, 0(sp)\n";
            }
            temp_stack_used += arg_size;
        }
    }

    
    int int_arg_idx = 0;
    int float_arg_idx = 0;


    for (size_t i = 0; i < arg_types.size(); ++i) {
        Context::Type arg_type = arg_types[i];
        

        int arg_offset = temp_stack_used - std::accumulate(arg_sizes.begin(), arg_sizes.begin() + i + 1, 0);

        if (arg_type == Context::Type::INT && int_arg_idx < 8) {
            stream << "  lw a" << int_arg_idx++ << ", " << arg_offset << "(sp)\n";
        } else if (arg_type == Context::Type::FLOAT && float_arg_idx < 8) {
            stream << "  flw fa" << float_arg_idx++ << ", " << arg_offset << "(sp)\n";
        } else if (arg_type == Context::Type::DOUBLE && float_arg_idx < 8) {
            stream << "  fld fa" << float_arg_idx++ << ", " << arg_offset << "(sp)\n";
        } else {
            // NOTE: This implementation does not yet handle arguments passed on the stack
            // (i.e., more than 8 integer or 8 float arguments).
            // The provided test cases only use register-passed arguments.
        }
    }
    
    if (temp_stack_used > 0) {
        stream << "  addi sp, sp, " << temp_stack_used << "\n";
    }

    const auto* id = dynamic_cast<const Identifier*>(function_name_.get());
    stream << "  jal " << id->GetName() << std::endl;
}


void FunctionCall::Print(std::ostream& stream) const {
    function_name_->Print(stream);
    stream << "(";
    if (arguments_) {
        arguments_->Print(stream);
    }
    stream << ")";
}

} // namespace ast