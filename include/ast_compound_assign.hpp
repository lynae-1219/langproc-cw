#pragma once
#include "ast_node.hpp"

namespace ast {

class CompoundAssign : public Node {
private:
    NodePtr lhs_;
    NodePtr rhs_;
    std::string op_;

public:
    CompoundAssign(NodePtr lhs, NodePtr rhs, std::string op)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(std::move(op)) {}

    Context::Type GetType(Context& context) const override {
        return lhs_->GetType(context);
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

}
