#include "ast_assign.hpp"
#include "ast_identifier.hpp"
#include "ast_array_access.hpp"
#include "ast_dereference.hpp"
#include "ast_struct.hpp"
#include "ast_context.hpp"

namespace ast {

void Assign::EmitRISC(std::ostream& stream, Context& context) const {
    if (auto* member = dynamic_cast<const StructMemberAccess*>(lhs_.get())) {
        rhs_->EmitRISC(stream, context);
        stream << "  addi sp, sp, -4" << std::endl;
        stream << "  sw a0, 0(sp)" << std::endl;

        member->EmitAddress(stream, context);
        stream << "  mv t1, a0" << std::endl;

        stream << "  lw a0, 0(sp)" << std::endl;
        stream << "  addi sp, sp, 4" << std::endl;
        stream << "  sw a0, 0(t1)" << std::endl;
        return;
    }

    if (auto* deref = dynamic_cast<const Dereference*>(lhs_.get())) {
        rhs_->EmitRISC(stream, context);
        stream << "  addi sp, sp, -4" << std::endl;
        stream << "  sw a0, 0(sp)" << std::endl;

        deref->EmitAddress(stream, context);
        stream << "  mv t1, a0" << std::endl;

        stream << "  lw a0, 0(sp)" << std::endl;
        stream << "  addi sp, sp, 4" << std::endl;
        stream << "  sw a0, 0(t1)" << std::endl;
        return;
    }

    if (auto* arr = dynamic_cast<const ArrayAccess*>(lhs_.get())) {
        rhs_->EmitRISC(stream, context);

        Context::Type type = arr->GetType(context);
        if (type == Context::Type::DOUBLE) {
            stream << "  addi sp, sp, -8" << std::endl;
            stream << "  fsd fa0, 0(sp)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  addi sp, sp, -4" << std::endl;
            stream << "  fsw fa0, 0(sp)" << std::endl;
        } else {
            stream << "  addi sp, sp, -4" << std::endl;
            stream << "  sw a0, 0(sp)" << std::endl;
        }

        arr->EmitAddress(stream, context);
        stream << "  mv t1, a0" << std::endl;

        if (type == Context::Type::DOUBLE) {
            stream << "  fld fa0, 0(sp)" << std::endl;
            stream << "  addi sp, sp, 8" << std::endl;
            stream << "  fsd fa0, 0(t1)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  flw fa0, 0(sp)" << std::endl;
            stream << "  addi sp, sp, 4" << std::endl;
            stream << "  fsw fa0, 0(t1)" << std::endl;
        } else {
            stream << "  lw a0, 0(sp)" << std::endl;
            stream << "  addi sp, sp, 4" << std::endl;
            stream << "  sw a0, 0(t1)" << std::endl;
        }
        return;
    }

    auto* id = dynamic_cast<const Identifier*>(lhs_.get());
    if (!id) {
        throw std::runtime_error("LHS of assignment is not an identifier or array access.");
    }

    rhs_->EmitRISC(stream, context);

    const std::string& var_name = id->GetName();
    Context::Type type = context.GetVariableType(var_name);

    bool is_ptr = context.IsPointerVariable(var_name);

    if (context.IsGlobalVariable(var_name)) {
        stream << "  la t0, " << var_name << std::endl;
        if (type == Context::Type::INT || is_ptr) {
            stream << "  sw a0, 0(t0)" << std::endl;
        } else if (type == Context::Type::CHAR) {
            stream << "  sb a0, 0(t0)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  fsw fa0, 0(t0)" << std::endl;
        } else if (type == Context::Type::DOUBLE) {
            stream << "  fsd fa0, 0(t0)" << std::endl;
        }
    } else {
        int offset = context.GetVariableOffset(var_name);
        if (type == Context::Type::INT || is_ptr) {
            stream << "  sw a0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::CHAR) {
            stream << "  sb a0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::FLOAT) {
            stream << "  fsw fa0, " << offset << "(s0)" << std::endl;
        } else if (type == Context::Type::DOUBLE) {
            stream << "  fsd fa0, " << offset << "(s0)" << std::endl;
        }
    }
}

void Assign::Print(std::ostream& stream) const {
    lhs_->Print(stream);
    stream << " = ";
    rhs_->Print(stream);
}

}
