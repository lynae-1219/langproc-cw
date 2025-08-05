#pragma once
#include "ast_node.hpp"
#include <atomic>

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

    Context::Type GetType(Context& context) const override {
        (void)context; // context is not needed, but required by the virtual function signature
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override {
        int label_id = logical_op_counter++;
        std::string false_label = ".L_LOGICAL_FALSE_" + std::to_string(label_id);
        std::string end_label = ".L_LOGICAL_END_" + std::to_string(label_id);

        if (op_ == "&&") {
            
            lhs_->EmitRISC(stream, context);
        
            stream << "  beq a0, zero, " << false_label << std::endl;

            
            rhs_->EmitRISC(stream, context);
            
            stream << "  beq a0, zero, " << false_label << std::endl;

            
            stream << "  li a0, 1" << std::endl;
            stream << "  j " << end_label << std::endl;

           
            stream << false_label << ":" << std::endl;
            stream << "  li a0, 0" << std::endl;

           
            stream << end_label << ":" << std::endl;

        } else if (op_ == "||") {
            std::string true_label = ".L_LOGICAL_TRUE_" + std::to_string(label_id);

           
            lhs_->EmitRISC(stream, context);
           
            stream << "  bne a0, zero, " << true_label << std::endl;

        
            rhs_->EmitRISC(stream, context);
            
            stream << "  bne a0, zero, " << true_label << std::endl;
            
        
            stream << "  li a0, 0" << std::endl;
            stream << "  j " << end_label << std::endl;

            stream << true_label << ":" << std::endl;
            stream << "  li a0, 1" << std::endl;

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