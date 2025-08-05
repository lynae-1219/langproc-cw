#pragma once
#include "ast_node.hpp"
#include "ast_identifier.hpp"

namespace ast {

class Assign : public Node {
private:
    NodePtr lhs_;
    NodePtr rhs_;
public:
    Assign(NodePtr lhs, NodePtr rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    
    // AMENDED: Added GetType implementation.
    // The type of an assignment is the type of the left-hand side.
    Context::Type GetType(Context& context) const override {
        return lhs_->GetType(context);
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast