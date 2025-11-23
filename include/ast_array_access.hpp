#pragma once
#include "ast_node.hpp"
#include "ast_identifier.hpp"

namespace ast {

class ArrayAccess : public Node {
private:
    NodePtr array_;
    NodePtr index_;

public:
    ArrayAccess(NodePtr array, NodePtr index)
        : array_(std::move(array)), index_(std::move(index)) {}

    Context::Type GetType(Context& context) const override;
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    void EmitAddress(std::ostream& stream, Context& context) const;
    const std::string& GetArrayName() const;
};

}
