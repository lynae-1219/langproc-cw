#pragma once
#include <memory>
#include <vector>
#include <ostream>
#include "ast_context.hpp"

namespace ast {

class Node {
public:
    virtual ~Node() = default;

    virtual Context::Type GetType(Context& context) const = 0;
    virtual bool IsPointer(Context& context) const { (void)context; return false; }

    virtual void EmitRISC(std::ostream& stream, Context& context) const = 0;
    virtual void Print(std::ostream& stream) const = 0;
};

using NodePtr = std::unique_ptr<Node>;

class NodeList : public Node {
public:
    NodeList() = default;
    explicit NodeList(NodePtr first_node) { nodes_.push_back(std::move(first_node)); }

    // A NodeList doesn't have a type itself, so this will throw an error.
    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on NodeList, which has no type.");
    }

    void PushBack(NodePtr item);
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const std::vector<NodePtr>& GetNodes() const { return nodes_; }

private:
    std::vector<NodePtr> nodes_;
};

} // namespace ast