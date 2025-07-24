#pragma once
#include "ast_node.hpp"

namespace ast {

class Assign : public Node {
private:
    NodePtr lhs_;
    NodePtr rhs_;
public:
    Assign(NodePtr lhs, NodePtr rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    
    void EmitRISC(std::ostream& stream, Context& context) const override {
        rhs_->EmitRISC(stream, context); // Evaluate RHS (result in a0)
        
        auto id = dynamic_cast<Identifier*>(lhs_.get());
        int offset = context.GetVariableOffset(id->GetName());
        stream << "sw a0, " << offset << "(sp)" << std::endl;
    }
    
    void Print(std::ostream& stream) const override {
        lhs_->Print(stream);
        stream << " = ";
        rhs_->Print(stream);
    }
};

} // namespace ast