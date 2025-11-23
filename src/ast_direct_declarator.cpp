#include "ast_direct_declarator.hpp"

namespace ast {

void DirectDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
    (void)stream;
    (void)context;
}

void DirectDeclarator::Print(std::ostream& stream) const {
    identifier_->Print(stream);
    if (array_size_ > 0) {
        stream << "[" << array_size_ << "]";
    }
}

}
