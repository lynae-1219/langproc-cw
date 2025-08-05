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

    Context::Type GetType(Context& context) const override {
        if (op_ == "==" || op_ == "!=" || op_ == "<" || op_ == "<=" || op_ == ">" || op_ == ">=") {
            return Context::Type::INT;
        }
        return lhs_->GetType(context);
    }

    void EmitRISC(std::ostream& stream, Context& context) const override {
        Context::Type operand_type = lhs_->GetType(context);

        // --- Step 1: Evaluate LHS and push result to stack ---
        lhs_->EmitRISC(stream, context);
        if (operand_type == Context::Type::DOUBLE) {
            stream << "  addi sp, sp, -8" << std::endl;
            stream << "  fsd fa0, 0(sp)" << std::endl;
        } else {
            stream << "  addi sp, sp, -4" << std::endl;
            if (operand_type == Context::Type::FLOAT) {
                stream << "  fsw fa0, 0(sp)" << std::endl;
            } else { // INT
                stream << "  sw a0, 0(sp)" << std::endl;
            }
        }

        // --- Step 2: Evaluate RHS, result is now in a0/fa0 ---
        rhs_->EmitRISC(stream, context);

        // --- Step 3: Pop LHS from stack into a temporary register ---
        if (operand_type == Context::Type::DOUBLE) {
            stream << "  fld ft0, 0(sp)" << std::endl;
            stream << "  addi sp, sp, 8" << std::endl;
        } else {
            if (operand_type == Context::Type::FLOAT) {
                stream << "  flw ft0, 0(sp)" << std::endl;
            } else { // INT
                stream << "  lw t0, 0(sp)" << std::endl;
            }
            stream << "  addi sp, sp, 4" << std::endl;
        }

        // --- Step 4: Perform the operation ---
        if (op_ == "+" || op_ == "-" || op_ == "*" || op_ == "/" || op_ == "%" ||
            op_ == "&" || op_ == "|" || op_ == "^" || op_ == "<<" || op_ == ">>")
        {
            if (operand_type == Context::Type::INT) {
                if (op_ == "+") { stream << "  add a0, t0, a0" << std::endl; }
                else if (op_ == "-") { stream << "  sub a0, t0, a0" << std::endl; }
                else if (op_ == "*") { stream << "  mul a0, t0, a0" << std::endl; }
                else if (op_ == "/") { stream << "  div a0, t0, a0" << std::endl; }
                else if (op_ == "%") { stream << "  rem a0, t0, a0" << std::endl; }
                else if (op_ == "&") { stream << "  and a0, t0, a0" << std::endl; }
                else if (op_ == "|") { stream << "  or a0, t0, a0" << std::endl; }
                else if (op_ == "^") { stream << "  xor a0, t0, a0" << std::endl; }
                else if (op_ == "<<") { stream << "  sll a0, t0, a0" << std::endl; }
                else if (op_ == ">>") { stream << "  sra a0, t0, a0" << std::endl; }
            } else if (operand_type == Context::Type::FLOAT) {
                if (op_ == "+") { stream << "  fadd.s fa0, ft0, fa0" << std::endl; }
                else if (op_ == "-") { stream << "  fsub.s fa0, ft0, fa0" << std::endl; }
                else if (op_ == "*") { stream << "  fmul.s fa0, ft0, fa0" << std::endl; }
                else if (op_ == "/") { stream << "  fdiv.s fa0, ft0, fa0" << std::endl; }
            } else if (operand_type == Context::Type::DOUBLE) {
                if (op_ == "+") { stream << "  fadd.d fa0, ft0, fa0" << std::endl; }
                else if (op_ == "-") { stream << "  fsub.d fa0, ft0, fa0" << std::endl; }
                else if (op_ == "*") { stream << "  fmul.d fa0, ft0, fa0" << std::endl; }
                else if (op_ == "/") { stream << "  fdiv.d fa0, ft0, fa0" << std::endl; }
            }
        }
        else // Relational Operations
        {
            if (operand_type == Context::Type::INT) {
                if (op_ == "==") { stream << "  sub a0, t0, a0\n  seqz a0, a0" << std::endl; }
                else if (op_ == "!=") { stream << "  sub a0, t0, a0\n  snez a0, a0" << std::endl; }
                else if (op_ == "<") { stream << "  slt a0, t0, a0" << std::endl; }
                else if (op_ == "<=") { stream << "  sgt a0, t0, a0\n  xori a0, a0, 1" << std::endl; }
                else if (op_ == ">") { stream << "  sgt a0, t0, a0" << std::endl; }
                else if (op_ == ">=") { stream << "  slt a0, t0, a0\n  xori a0, a0, 1" << std::endl; }
            } else if (operand_type == Context::Type::FLOAT) {
                if (op_ == "==") { stream << "  feq.s a0, ft0, fa0" << std::endl; }
                else if (op_ == "<") { stream << "  flt.s a0, ft0, fa0" << std::endl; }
                else if (op_ == "<=") { stream << "  fle.s a0, ft0, fa0" << std::endl; }
                else if (op_ == ">") { stream << "  flt.s a0, fa0, ft0" << std::endl; }
                else if (op_ == ">=") { stream << "  fle.s a0, fa0, ft0" << std::endl; }
                else if (op_ == "!=") { stream << "  feq.s a0, ft0, fa0\n  seqz a0, a0" << std::endl; }
            } else if (operand_type == Context::Type::DOUBLE) {
                if (op_ == "==") { stream << "  feq.d a0, ft0, fa0" << std::endl; }
                else if (op_ == "<") { stream << "  flt.d a0, ft0, fa0" << std::endl; }
                else if (op_ == "<=") { stream << "  fle.d a0, ft0, fa0" << std::endl; }
                else if (op_ == ">") { stream << "  flt.d a0, fa0, ft0" << std::endl; }
                else if (op_ == ">=") { stream << "  fle.d a0, fa0, ft0" << std::endl; }
                else if (op_ == "!=") { stream << "  feq.d a0, ft0, fa0\n  seqz a0, a0" << std::endl; }
            }
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