#include "ast_array_access.hpp"

namespace ast {

Context::Type ArrayAccess::GetType(Context& context) const {
    auto* id = dynamic_cast<const Identifier*>(array_.get());
    if (!id) {
        throw std::runtime_error("Array base is not an identifier");
    }
    return context.GetVariableType(id->GetName());
}

const std::string& ArrayAccess::GetArrayName() const {
    auto* id = dynamic_cast<const Identifier*>(array_.get());
    if (!id) {
        throw std::runtime_error("Array base is not an identifier");
    }
    return id->GetName();
}

void ArrayAccess::EmitAddress(std::ostream& stream, Context& context) const {
    auto* id = dynamic_cast<const Identifier*>(array_.get());
    if (!id) {
        throw std::runtime_error("Array base is not an identifier");
    }

    const std::string& name = id->GetName();
    Context::Type type = context.GetVariableType(name);
    int element_size = (type == Context::Type::DOUBLE) ? 8 : (type == Context::Type::CHAR) ? 1 : 4;

    bool is_pointer = context.IsPointerVariable(name);

    if (context.IsGlobalVariable(name)) {
        index_->EmitRISC(stream, context);
        stream << "  li t1, " << element_size << std::endl;
        stream << "  mul t0, a0, t1" << std::endl;
        if (is_pointer) {
            stream << "  la t1, " << name << std::endl;
            stream << "  lw t1, 0(t1)" << std::endl;
        } else {
            stream << "  la t1, " << name << std::endl;
        }
        stream << "  add a0, t1, t0" << std::endl;
    } else {
        int base_offset = context.GetVariableOffset(name);
        index_->EmitRISC(stream, context);
        stream << "  li t1, " << element_size << std::endl;
        stream << "  mul t0, a0, t1" << std::endl;
        if (is_pointer) {
            stream << "  lw t1, " << base_offset << "(s0)" << std::endl;
        } else {
            stream << "  addi t1, s0, " << base_offset << std::endl;
        }
        stream << "  add a0, t1, t0" << std::endl;
    }
}

void ArrayAccess::EmitRISC(std::ostream& stream, Context& context) const {
    EmitAddress(stream, context);

    Context::Type type = GetType(context);
    if (type == Context::Type::CHAR) {
        stream << "  lbu a0, 0(a0)" << std::endl;
    } else if (type == Context::Type::INT) {
        stream << "  lw a0, 0(a0)" << std::endl;
    } else if (type == Context::Type::FLOAT) {
        stream << "  flw fa0, 0(a0)" << std::endl;
    } else if (type == Context::Type::DOUBLE) {
        stream << "  fld fa0, 0(a0)" << std::endl;
    }
}

void ArrayAccess::Print(std::ostream& stream) const {
    array_->Print(stream);
    stream << "[";
    index_->Print(stream);
    stream << "]";
}

}
