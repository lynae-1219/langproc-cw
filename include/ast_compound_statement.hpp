#pragma once
#include "ast_node.hpp"

namespace ast {

class CompoundStatement : public Node {
private:
    NodePtr statement_list_;

public:
    CompoundStatement(NodePtr statement_list) : statement_list_(std::move(statement_list)) {}
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast