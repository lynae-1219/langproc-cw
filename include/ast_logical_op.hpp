#pragma once
#include "ast_node.hpp"
#include <atomic> // Used for generating unique labels

namespace ast {

inline std::atomic<int> logical_op_counter = 0;

class LogicalOp : public Node {
private:
    NodePtr lhs_;
    NodePtr rhs_;
    std::string op_;

public:
    LogicalOp(NodePtr lhs, NodePtr rhs, std::string op)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op) {}

    void EmitRISC(std::ostream& stream, Context& context) const override {
        int label_id = logical_op_counter++;
        std::string false_label = ".L_LOGICAL_FALSE_" + std::to_string(label_id);
        std::string end_label = ".L_LOGICAL_END_" + std::to_string(label_id);

        if (op_ == "&&") {
            // Evaluate LHS
            lhs_->EmitRISC(stream, context);
            // If LHS is false (0), short-circuit to the false case
            stream << "  beq a0, zero, " << false_label << std::endl;

            // If LHS is true, evaluate RHS
            rhs_->EmitRISC(stream, context);
            // If RHS is false (0), go to the false case
            stream << "  beq a0, zero, " << false_label << std::endl;

            // Both were true, result is 1
            stream << "  li a0, 1" << std::endl;
            stream << "  j " << end_label << std::endl;

            // False case: result is 0
            stream << false_label << ":" << std::endl;
            stream << "  li a0, 0" << std::endl;

            // End of operation
            stream << end_label << ":" << std::endl;

        } else if (op_ == "||") {
            std::string true_label = ".L_LOGICAL_TRUE_" + std::to_string(label_id);

            // Evaluate LHS
            lhs_->EmitRISC(stream, context);
            // If LHS is true (not 0), short-circuit to the true case
            stream << "  bne a0, zero, " << true_label << std::endl;

            // If LHS is false, evaluate RHS
            rhs_->EmitRISC(stream, context);
            // If RHS is true (not 0), go to the true case
            stream << "  bne a0, zero, " << true_label << std::endl;
            
            // Both were false, result is 0
            stream << "  li a0, 0" << std::endl;
            stream << "  j " << end_label << std::endl;

            // True case: result is 1
            stream << true_label << ":" << std::endl;
            stream << "  li a0, 1" << std::endl;

            // End of operation
            stream << end_label << ":" << std::endl;
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