#pragma once
#include "ast_node.hpp"
// yay incredment decrement prefix postfix
namespace ast {


class UpdateExpression : public Node {
private:
    NodePtr expr_;
    bool is_prefix_;
    bool is_decrement_; 

public:
    UpdateExpression(NodePtr expr, bool is_prefix, bool is_decrement)
        : expr_(std::move(expr)),
          is_prefix_(is_prefix),
          is_decrement_(is_decrement) {}

    Context::Type GetType(Context& context) const override {
        return expr_->GetType(context);
    }
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast