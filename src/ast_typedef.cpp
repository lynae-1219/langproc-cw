#include "ast_typedef.hpp"
#include "ast_context.hpp"

namespace ast {

void TypedefDeclaration::EmitRISC(std::ostream& stream, Context& context) const {
    (void)stream;
    context.AddTypedef(new_name_, SpecifierToContextType(base_type_));
}

void TypedefDeclaration::Print(std::ostream& stream) const {
    stream << "typedef " << base_type_;
    if (is_pointer_) {
        stream << " *";
    }
    stream << " " << new_name_ << ";";
}

}
