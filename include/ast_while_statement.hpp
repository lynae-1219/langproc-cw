#pragma once
#include "ast_node.hpp"

namespace ast {

class WhileStatement : public Node {
private:
    NodePtr condition_;
    NodePtr body_;

public:
    WhileStatement(NodePtr condition, NodePtr body)
        : condition_(std::move(condition)), body_(std::move(body)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on WhileStatement");
    }
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast