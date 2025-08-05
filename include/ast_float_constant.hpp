#pragma once
#include "ast_node.hpp"

namespace ast {

class FloatConstant : public Node {
private:
    double value_;

public:
    FloatConstant(double value) : value_(value) {}

    // A FloatConstant node always has the type FLOAT.
    Context::Type GetType(Context& context) const override {
        (void)context; // context is unused
        return Context::Type::FLOAT;
    }
    
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast