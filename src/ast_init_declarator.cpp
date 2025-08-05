#include "ast_init_declarator.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_context.hpp"

namespace ast {

InitDeclarator::InitDeclarator(NodePtr declarator, NodePtr initializer)
    : declarator_(std::move(declarator)), initializer_(std::move(initializer)) {}

void InitDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
    if (initializer_) {
        initializer_->EmitRISC(stream, context);

        std::string var_name = GetName();
        int offset = context.GetVariableOffset(var_name);
        Context::Type type = context.GetVariableType(var_name);

        if (type == Context::Type::INT) {
            stream << "  sw a0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  fsw fa0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::DOUBLE) {
            stream << "  fsd fa0, " << offset << "(s0)" << std::endl;
        }
    }
}

void InitDeclarator::Print(std::ostream& stream) const {
    if (declarator_) {
        declarator_->Print(stream);
    }
    if (initializer_) {
        stream << " = ";
        initializer_->Print(stream);
    }
}

std::string InitDeclarator::GetName() const {
    if (const auto* dd = dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
        if (const auto* id = dynamic_cast<const Identifier*>(dd->GetIdentifier())) {
            return id->GetName();
        }
    }
    throw std::runtime_error("Could not find name in InitDeclarator");
}

} // namespace ast