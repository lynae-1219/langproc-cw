#pragma once
#include "ast_node.hpp"
#include "ast_identifier.hpp" // Added this include

namespace ast {

class Assign : public Node {
private:
    NodePtr lhs_;
    NodePtr rhs_;
public:
    Assign(NodePtr lhs, NodePtr rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    
    // Declarations only, definitions are in the .cpp file
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast