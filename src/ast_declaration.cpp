#include "ast_declaration.hpp"
#include "ast_init_declarator.hpp"
#include "ast_context.hpp"

namespace ast {

Declaration::Declaration(NodePtr init_declarator_list)
    : init_declarator_list_(std::move(init_declarator_list)) {}

void Declaration::EmitRISC(std::ostream& stream, Context& context) const {
    if (init_declarator_list_) {
        init_declarator_list_->EmitRISC(stream, context);
    }
}

void Declaration::Print(std::ostream& stream) const {
    stream << "int ";
    if (init_declarator_list_) {
        init_declarator_list_->Print(stream);
    }
    stream << ";\n";
}

} // namespace ast