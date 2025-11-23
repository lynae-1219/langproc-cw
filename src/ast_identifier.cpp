#include "ast_identifier.hpp"

namespace ast {

Identifier::Identifier(std::string identifier)
    : identifier_(std::move(identifier)) {}

Context::Type Identifier::GetType(Context& context) const {
    if (context.HasEnumConstant(identifier_)) {
        return Context::Type::INT;
    }
    return context.GetVariableType(identifier_);
}

bool Identifier::IsPointer(Context& context) const {
    if (context.HasEnumConstant(identifier_)) {
        return false;
    }
    return context.IsPointerVariable(identifier_);
}

void Identifier::EmitRISC(std::ostream& stream, Context& context) const {
    if (context.HasEnumConstant(identifier_)) {
        int value = context.GetEnumConstant(identifier_);
        stream << "  li a0, " << value << std::endl;
        return;
    }

    Context::Type type = this->GetType(context);

    if (context.IsGlobalVariable(identifier_)) {
        bool is_ptr = context.IsPointerVariable(identifier_);
        stream << "  la t0, " << identifier_ << std::endl;
        if (type == Context::Type::INT || is_ptr) {
            stream << "  lw a0, 0(t0)" << std::endl;
        } else if (type == Context::Type::CHAR) {
            stream << "  lbu a0, 0(t0)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  flw fa0, 0(t0)" << std::endl;
        } else if (type == Context::Type::DOUBLE) {
            stream << "  fld fa0, 0(t0)" << std::endl;
        }
    } else {
        int offset = context.GetVariableOffset(identifier_);
        bool is_ptr = context.IsPointerVariable(identifier_);
        if (type == Context::Type::INT || is_ptr) {
            stream << "  lw a0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::CHAR) {
            stream << "  lbu a0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  flw fa0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::DOUBLE) {
            stream << "  fld fa0, " << offset << "(s0)" << std::endl;
        }
    }
}

void Identifier::Print(std::ostream& stream) const {
    stream << identifier_;
}

}
