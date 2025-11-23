#include "ast_enum.hpp"
#include "ast_constant.hpp"
#include "ast_context.hpp"

namespace ast {

void EnumDeclaration::EmitRISC(std::ostream& stream, Context& context) const {
    (void)stream;
    if (!enumerator_list_) return;

    auto* list = dynamic_cast<const NodeList*>(enumerator_list_.get());
    if (!list) return;

    int current_value = 0;
    for (const auto& node : list->GetNodes()) {
        if (auto* en = dynamic_cast<const Enumerator*>(node.get())) {
            if (en->GetValue()) {
                if (auto* ic = dynamic_cast<const IntConstant*>(en->GetValue())) {
                    current_value = ic->GetValue();
                }
            }
            context.AddEnumConstant(en->GetName(), current_value);
            ++current_value;
        }
    }
}

void EnumDeclaration::Print(std::ostream& stream) const {
    stream << "enum";
    if (!name_.empty()) {
        stream << " " << name_;
    }
    stream << " { ";
    if (enumerator_list_) {
        enumerator_list_->Print(stream);
    }
    stream << " }";
}

void Enumerator::EmitRISC(std::ostream& stream, Context& context) const {
    (void)stream;
    (void)context;
}

void Enumerator::Print(std::ostream& stream) const {
    stream << name_;
    if (value_) {
        stream << " = ";
        value_->Print(stream);
    }
}

}
