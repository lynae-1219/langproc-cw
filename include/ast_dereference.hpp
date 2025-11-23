#pragma once
#include "ast_node.hpp"

namespace ast {

class Dereference : public Node {
private:
    NodePtr operand_;

public:
    Dereference(NodePtr operand) : operand_(std::move(operand)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    void EmitAddress(std::ostream& stream, Context& context) const;
};

}
