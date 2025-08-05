#pragma once
#include "ast_node.hpp"

namespace ast {

class FunctionCall : public Node {
private:
    NodePtr function_name_;
    NodePtr arguments_;

public:
    FunctionCall(NodePtr function_name, NodePtr arguments)
        : function_name_(std::move(function_name)), arguments_(std::move(arguments)) {}

    // For now, we'll assume function calls return INT. A full implementation
    // would look up the function's return type in a symbol table.
    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast