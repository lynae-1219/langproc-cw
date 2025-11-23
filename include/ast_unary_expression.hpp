#pragma once
#include "ast_node.hpp"

namespace ast {

class UnaryExpression : public Node {
private:
    NodePtr operand_;
    std::string op_;

public:
    UnaryExpression(NodePtr operand, std::string op)
        : operand_(std::move(operand)), op_(std::move(op)) {}

    Context::Type GetType(Context& context) const override {
        if (op_ == "!") {
            return Context::Type::INT;
        }
        return operand_->GetType(context);
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

}
