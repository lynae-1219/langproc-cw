#pragma once
#include "ast_node.hpp"

namespace ast {

class BinaryOp : public Node {
private:
    NodePtr lhs_;
    NodePtr rhs_;
    std::string op_;

public:
    BinaryOp(NodePtr lhs, NodePtr rhs, std::string op)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op) {}

    void EmitRISC(std::ostream& stream, Context& context) const override {
        
        lhs_->EmitRISC(stream, context);
        
        stream << "  mv t0, a0" << std::endl;

        rhs_->EmitRISC(stream, context);
        
        if (op_ == "+") {
            stream << "  add a0, t0, a0" << std::endl;
        }
        // Add other operations as needed
    }

    void Print(std::ostream& stream) const override {
        stream << "(";
        lhs_->Print(stream);
        stream << " " << op_ << " ";
        rhs_->Print(stream);
        stream << ")";
    }
};

} // namespace ast