#include "ast_function_call.hpp"
#include "ast_identifier.hpp"

namespace ast {

void FunctionCall::EmitRISC(std::ostream& stream, Context& context) const {
    const auto* args_list_node = dynamic_cast<const NodeList*>(arguments_.get());
    
    std::vector<std::string> int_arg_regs = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"};
    std::vector<std::string> float_arg_regs = {"fa0", "fa1", "fa2", "fa3", "fa4", "fa5", "fa6", "fa7"};
    size_t int_arg_count = 0;
    size_t float_arg_count = 0;

    if (args_list_node) {
        // We need to evaluate all arguments first, saving their results to the stack,
        // because the evaluation of one argument might clobber registers needed for another.
        int total_arg_stack_space = 0;
        for (const auto& arg : args_list_node->GetNodes()) {
            arg->EmitRISC(stream, context);
            Context::Type arg_type = arg->GetType(context);
            if (arg_type == Context::Type::INT) {
                stream << "  addi sp, sp, -4\n";
                stream << "  sw a0, 0(sp)\n";
                total_arg_stack_space += 4;
            } else { // FLOAT or DOUBLE
                stream << "  addi sp, sp, -8\n";
                stream << "  fsw fa0, 0(sp)\n"; // Assuming float for now
                total_arg_stack_space += 8;
            }
        }

        // Now pop them from the stack into the correct argument registers.
        // We iterate in reverse to pop in the correct order.
        const auto& args = args_list_node->GetNodes();
        for (int i = args.size() - 1; i >= 0; --i) {
            Context::Type arg_type = args[i]->GetType(context);
            if (arg_type == Context::Type::INT) {
                if (int_arg_count < int_arg_regs.size()) {
                    stream << "  lw " << int_arg_regs[int_arg_count++] << ", " << (args.size() - 1 - i) * 4 << "(sp)\n";
                }
            } else {
                 if (float_arg_count < float_arg_regs.size()) {
                    // This part needs adjustment for mixed float/double sizes
                    stream << "  flw " << float_arg_regs[float_arg_count++] << ", " << (args.size() - 1 - i) * 4 << "(sp)\n";
                 }
            }
        }
        // Deallocate the temporary stack space for arguments
        stream << "  addi sp, sp, " << total_arg_stack_space << "\n";
    }

    // Get function name and call
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