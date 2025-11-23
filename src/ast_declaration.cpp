#include "ast_declaration.hpp"
#include "ast_init_declarator.hpp"
#include "ast_context.hpp"
#include "ast_node.hpp"

namespace ast {

Declaration::Declaration(TypeSpecifier type_specifier, NodePtr init_declarator_list, bool is_global)
    : type_specifier_(type_specifier),
      init_declarator_list_(std::move(init_declarator_list)),
      is_global_(is_global) {}

void Declaration::EmitRISC(std::ostream& stream, Context& context) const {
    if (!init_declarator_list_) return;

    Context::Type var_type = SpecifierToContextType(type_specifier_);
    const auto& declarators = dynamic_cast<const NodeList*>(init_declarator_list_.get())->GetNodes();

    for (const auto& decl_node : declarators) {
        const auto* init_declarator = dynamic_cast<const InitDeclarator*>(decl_node.get());

        if (init_declarator->IsFunction()) {
            continue;
        }

        std::string name = init_declarator->GetName();
        int array_size = init_declarator->GetArraySize();
        bool is_pointer = init_declarator->IsPointer();

        if (is_global_) {
            context.AddGlobalVariable(name, var_type, array_size, is_pointer);
            int element_size = (var_type == Context::Type::DOUBLE) ? 8 : 4;
            int total_size = (array_size > 0) ? element_size * array_size : element_size;
            stream << ".data" << std::endl;
            stream << ".globl " << name << std::endl;
            stream << name << ":" << std::endl;
            stream << "  .zero " << total_size << std::endl;
            stream << ".text" << std::endl;
        } else {
            context.AddVariable(name, var_type, 4, array_size, is_pointer);
        }
    }

    if (!is_global_) {
        init_declarator_list_->EmitRISC(stream, context);
    }
}

void Declaration::Print(std::ostream& stream) const {
    stream << type_specifier_ << " ";
    if (init_declarator_list_) {
        init_declarator_list_->Print(stream);
    }
    stream << ";\n";
}

}
