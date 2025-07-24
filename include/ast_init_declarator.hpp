#pragma once
#include "ast_node.hpp"

namespace ast {

class InitDeclarator : public Node {
private:
    NodePtr declarator_;
    NodePtr initializer_;

public:
    InitDeclarator(NodePtr declarator, NodePtr initializer);
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast