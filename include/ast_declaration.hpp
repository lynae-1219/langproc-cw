#pragma once
#include "ast_node.hpp"

namespace ast {

class Declaration : public Node {
private:
    NodePtr init_declarator_list_;

public:
    Declaration(NodePtr init_declarator_list);
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

   
    const Node* GetDeclarator() const { return init_declarator_list_.get(); }
};

} // namespace ast