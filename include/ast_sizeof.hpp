#pragma once
#include "ast_node.hpp"

namespace ast {

class SizeofExpr : public Node {
private:
    NodePtr operand_;
    TypeSpecifier type_spec_;
    bool is_type_;

public:
    SizeofExpr(NodePtr operand) : operand_(std::move(operand)), type_spec_(TypeSpecifier::INT), is_type_(false) {}
    SizeofExpr(TypeSpecifier type) : operand_(nullptr), type_spec_(type), is_type_(true) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

}
