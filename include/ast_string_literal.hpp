#pragma once
#include "ast_node.hpp"
#include <string>

namespace ast {

class StringLiteral : public Node {
private:
    std::string value_;
    static int string_counter_;

public:
    StringLiteral(std::string value) : value_(std::move(value)) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        return Context::Type::INT;
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const std::string& GetValue() const { return value_; }
};

}
