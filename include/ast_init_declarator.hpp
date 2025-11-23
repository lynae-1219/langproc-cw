#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class InitDeclarator : public Node {
private:
    NodePtr declarator_;
    NodePtr initializer_;

public:
    InitDeclarator(NodePtr declarator, NodePtr initializer);

    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on InitDeclarator");
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    std::string GetName() const;
    int GetArraySize() const;
    bool IsFunction() const;
    bool IsPointer() const;
};

}
