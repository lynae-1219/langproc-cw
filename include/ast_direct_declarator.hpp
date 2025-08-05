#pragma once
#include "ast_node.hpp"
#include "ast_identifier.hpp"

namespace ast {

class DirectDeclarator : public Node
{
private:
    NodePtr identifier_;
    NodePtr parameters_;

public:
    DirectDeclarator(NodePtr identifier) : identifier_(std::move(identifier)), parameters_(nullptr) {};

    Context::Type GetType(Context& context) const override {
        return identifier_->GetType(context);
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const Node* GetIdentifier() const { return identifier_.get(); }
    void SetParameters(NodePtr params) { parameters_ = std::move(params); }
    NodePtr TakeParameters() { return std::move(parameters_); }
};

} // namespace ast