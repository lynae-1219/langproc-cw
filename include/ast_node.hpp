#pragma once
#include <memory>
#include <vector>
#include <ostream>
#include "ast_context.hpp"

namespace ast {

class Node {
public:
    virtual ~Node() = default;
    virtual void EmitRISC(std::ostream& stream, Context& context) const = 0;
    virtual void Print(std::ostream& stream) const = 0;
};

using NodePtr = std::unique_ptr<Node>;

class NodeList : public Node {
public:
    NodeList() = default;
    explicit NodeList(NodePtr first_node) { nodes_.push_back(std::move(first_node)); }

    void PushBack(NodePtr item);  // Defined in your .cpp
    void EmitRISC(std::ostream& stream, Context& context) const override;  // Defined in your .cpp
    void Print(std::ostream& stream) const override;  // Defined in your .cpp

private:
    std::vector<NodePtr> nodes_;
};

} // namespace ast