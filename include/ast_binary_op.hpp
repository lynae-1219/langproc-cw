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
        } else if (op_ == "-") {
            stream << "  sub a0, t0, a0" << std::endl;
        } else if (op_ == "*") {
            stream << "  mul a0, t0, a0" << std::endl;
        } else if (op_ == "/") {
            stream << "  div a0, t0, a0" << std::endl;
        } else if (op_ == "%") {
            stream << "  rem a0, t0, a0" << std::endl;
        } else if (op_ == "<<") {
            stream << "  sll a0, t0, a0" << std::endl;
        } else if (op_ == ">>") {
            stream << "  sra a0, t0, a0" << std::endl;
        } else if (op_ == "&") {
            stream << "  and a0, t0, a0" << std::endl;
        } else if (op_ == "|") {
            stream << "  or a0, t0, a0" << std::endl;
        } else if (op_ == "^") {
            stream << "  xor a0, t0, a0" << std::endl;
        } else if (op_ == "==") {
            stream << "  sub a0, t0, a0" << std::endl;
            stream << "  seqz a0, a0" << std::endl; // Set if a0 is zero (x==y)
        } else if (op_ == "!=") {
            stream << "  sub a0, t0, a0" << std::endl;
            stream << "  snez a0, a0" << std::endl; // Set if a0 is not zero (x!=y)
        } else if (op_ == "<") {
            stream << "  slt a0, t0, a0" << std::endl; // Set if t0 < a0 (x<y)
        } else if (op_ == "<=") {
            stream << "  sgt a0, t0, a0" << std::endl; // Set if t0 > a0 (x>y)
            stream << "  xori a0, a0, 1" << std::endl; // Flip the result for <=
        } else if (op_ == ">") {
            stream << "  sgt a0, t0, a0" << std::endl; // Set if t0 > a0 (x>y)
        } else if (op_ == ">=") {
            stream << "  slt a0, t0, a0" << std::endl; // Set if t0 < a0 (x<y)
            stream << "  xori a0, a0, 1" << std::endl; // Flip the result for >=
        }
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