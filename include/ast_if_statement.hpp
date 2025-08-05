#pragma once
#include "ast_node.hpp"

namespace ast {

class IfStatement : public Node {
private:
    NodePtr condition_;
    NodePtr then_statement_;
    NodePtr else_statement_; 

public:
    IfStatement(NodePtr condition, NodePtr then_statement, NodePtr else_statement)
        : condition_(std::move(condition)),
          then_statement_(std::move(then_statement)),
          else_statement_(std::move(else_statement)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on IfStatement");
    }
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast