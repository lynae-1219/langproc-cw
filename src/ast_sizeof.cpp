#include "ast_sizeof.hpp"
#include "ast_identifier.hpp"

namespace ast {

void SizeofExpr::EmitRISC(std::ostream& stream, Context& context) const {
    int size = 4;

    if (is_type_) {
        switch (type_spec_) {
            case TypeSpecifier::CHAR: size = 1; break;
            case TypeSpecifier::INT: size = 4; break;
            case TypeSpecifier::UNSIGNED: size = 4; break;
            case TypeSpecifier::FLOAT: size = 4; break;
            case TypeSpecifier::DOUBLE: size = 8; break;
            default: size = 4; break;
        }
    } else if (operand_) {
        auto* id = dynamic_cast<const Identifier*>(operand_.get());
        if (id) {
            Context::Type t = context.GetVariableType(id->GetName());
            switch (t) {
                case Context::Type::CHAR: size = 1; break;
                case Context::Type::INT: size = 4; break;
                case Context::Type::FLOAT: size = 4; break;
                case Context::Type::DOUBLE: size = 8; break;
                case Context::Type::STRUCT: {
                    const std::string& sn = context.GetStructName(id->GetName());
                    size = context.GetStructType(sn).total_size;
                    break;
                }
                default: size = 4; break;
            }
        }
    }

    stream << "  li a0, " << size << std::endl;
}

void SizeofExpr::Print(std::ostream& stream) const {
    stream << "sizeof(";
    if (is_type_) {
        stream << type_spec_;
    } else if (operand_) {
        operand_->Print(stream);
    }
    stream << ")";
}

}
