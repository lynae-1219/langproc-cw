#include "ast_declaration.hpp"
#include "ast_init_declarator.hpp"
#include "ast_context.hpp"
#include "ast_node.hpp" 

namespace ast {

Declaration::Declaration(TypeSpecifier type_specifier, NodePtr init_declarator_list)
    : type_specifier_(type_specifier), init_declarator_list_(std::move(init_declarator_list)) {}

void Declaration::EmitRISC(std::ostream& stream, Context& context) const {
    if (!init_declarator_list_) return;

    Context::Type var_type = SpecifierToContextType(type_specifier_);
    const auto& declarators = dynamic_cast<const NodeList*>(init_declarator_list_.get())->GetNodes();
    for (const auto& decl_node : declarators) {
        const auto* init_declarator = dynamic_cast<const InitDeclarator*>(decl_node.get());
        context.AddVariable(init_declarator->GetName(), var_type);
    }

    init_declarator_list_->EmitRISC(stream, context);
}

void Declaration::Print(std::ostream& stream) const {
    stream << type_specifier_ << " ";
    if (init_declarator_list_) {
        init_declarator_list_->Print(stream);
    }
    stream << ";\n";
}

} // namespace ast