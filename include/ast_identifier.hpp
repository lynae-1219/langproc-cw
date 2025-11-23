#pragma once
#include "ast_node.hpp"

namespace ast {

class Identifier : public Node {
private:
    std::string identifier_;

public:
    Identifier(std::string identifier);
    
    Context::Type GetType(Context& context) const override;
    bool IsPointer(Context& context) const override;

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
    
    const std::string& GetName() const { return identifier_; }
};

} // namespace ast