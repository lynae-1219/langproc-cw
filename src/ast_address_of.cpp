#include "ast_address_of.hpp"
#include "ast_identifier.hpp"
#include "ast_context.hpp"

namespace ast {

void AddressOf::EmitRISC(std::ostream& stream, Context& context) const {
    if (auto* id = dynamic_cast<const Identifier*>(operand_.get())) {
        const std::string& name = id->GetName();
        if (context.IsGlobalVariable(name)) {
            stream << "  la a0, " << name << std::endl;
        } else {
            int offset = context.GetVariableOffset(name);
            stream << "  addi a0, s0, " << offset << std::endl;
        }
    } else {
        throw std::runtime_error("Address-of operator requires an lvalue");
    }
}

void AddressOf::Print(std::ostream& stream) const {
    stream << "(&";
    operand_->Print(stream);
    stream << ")";
}

}
