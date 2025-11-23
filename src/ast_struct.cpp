#include "ast_struct.hpp"
#include "ast_declaration.hpp"
#include "ast_init_declarator.hpp"
#include "ast_identifier.hpp"

namespace ast {

void StructDefinition::EmitRISC(std::ostream& stream, Context& context) const {
    (void)stream;

    Context::StructType st;
    st.total_size = 0;

    if (members_) {
        auto* list = dynamic_cast<const NodeList*>(members_.get());
        if (list) {
            for (const auto& member : list->GetNodes()) {
                auto* decl = dynamic_cast<const Declaration*>(member.get());
                if (decl) {
                    TypeSpecifier spec = decl->GetTypeSpecifier();
                    Context::Type type = SpecifierToContextType(spec);
                    int size = (type == Context::Type::DOUBLE) ? 8 : 4;

                    auto* init_list = dynamic_cast<const NodeList*>(decl->GetDeclarator());
                    if (init_list) {
                        for (const auto& init_node : init_list->GetNodes()) {
                            auto* init_decl = dynamic_cast<const InitDeclarator*>(init_node.get());
                            if (init_decl) {
                                Context::StructMember sm;
                                sm.name = init_decl->GetName();
                                sm.type = type;
                                sm.offset = st.total_size;
                                sm.size = size;
                                st.members.push_back(sm);
                                st.total_size += size;
                            }
                        }
                    }
                }
            }
        }
    }

    context.AddStructType(name_, st);
}

void StructDefinition::Print(std::ostream& stream) const {
    stream << "struct " << name_ << " { ... }";
}

Context::Type StructMemberAccess::GetType(Context& context) const {
    auto* id = dynamic_cast<const Identifier*>(object_.get());
    if (!id) return Context::Type::INT;

    const std::string& sn = context.GetStructName(id->GetName());
    const Context::StructType& st = context.GetStructType(sn);

    for (const auto& m : st.members) {
        if (m.name == member_) {
            return m.type;
        }
    }
    return Context::Type::INT;
}

void StructMemberAccess::EmitAddress(std::ostream& stream, Context& context) const {
    auto* id = dynamic_cast<const Identifier*>(object_.get());
    if (!id) return;

    const std::string& var_name = id->GetName();
    const std::string& sn = context.GetStructName(var_name);
    int base_offset = context.GetVariableOffset(var_name);
    int member_offset = context.GetStructMemberOffset(sn, member_);

    stream << "  addi a0, s0, " << (base_offset + member_offset) << std::endl;
}

void StructMemberAccess::EmitRISC(std::ostream& stream, Context& context) const {
    auto* id = dynamic_cast<const Identifier*>(object_.get());
    if (!id) return;

    const std::string& var_name = id->GetName();
    const std::string& sn = context.GetStructName(var_name);
    int base_offset = context.GetVariableOffset(var_name);
    int member_offset = context.GetStructMemberOffset(sn, member_);
    int total_offset = base_offset + member_offset;

    Context::Type type = GetType(context);
    if (type == Context::Type::DOUBLE) {
        stream << "  fld fa0, " << total_offset << "(s0)" << std::endl;
    } else if (type == Context::Type::FLOAT) {
        stream << "  flw fa0, " << total_offset << "(s0)" << std::endl;
    } else {
        stream << "  lw a0, " << total_offset << "(s0)" << std::endl;
    }
}

void StructMemberAccess::Print(std::ostream& stream) const {
    object_->Print(stream);
    stream << "." << member_;
}

void StructVariableDecl::EmitRISC(std::ostream& stream, Context& context) const {
    (void)stream;
    context.AddStructVariable(var_name_, struct_name_);
}

void StructVariableDecl::Print(std::ostream& stream) const {
    stream << "struct " << struct_name_ << " " << var_name_;
}

}
