#pragma once
#include "ast_node.hpp"

namespace ast {

class AddressOf : public Node {
private:
    NodePtr operand_;

public:
    AddressOf(NodePtr operand) : operand_(std::move(operand)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

}
