#include "ast_context.hpp"
#include <stdexcept>

namespace ast {

Context::Context() {
    PushScope();
}

void Context::PushScope() {
    scopes_.push_back({});
    scope_offsets_.push_back(current_stack_offset_);
}

void Context::PopScope() {
    if (scopes_.empty() || scope_offsets_.empty()) {
        throw std::runtime_error("Attempted to pop from empty scope.");
    }
    scopes_.pop_back();
    current_stack_offset_ = scope_offsets_.back();
    scope_offsets_.pop_back();
}

void Context::AddVariable(const std::string& name, Type type, int size, int array_size, bool is_pointer) {
    if (scopes_.empty()) {
        throw std::runtime_error("No scope available to add variable.");
    }

    int element_size = (type == Type::DOUBLE) ? 8 : size;
    int total_size = (array_size > 0) ? element_size * array_size : element_size;

    current_stack_offset_ += total_size;
    scopes_.back()[name] = {type, -current_stack_offset_, total_size, array_size, false, is_pointer, ""};

    if (current_stack_offset_ > max_stack_offset_) {
        max_stack_offset_ = current_stack_offset_;
    }
}

void Context::AddGlobalVariable(const std::string& name, Type type, int array_size, bool is_pointer) {
    int element_size = (type == Type::DOUBLE) ? 8 : 4;
    int total_size = (array_size > 0) ? element_size * array_size : element_size;
    globals_[name] = {type, 0, total_size, array_size, true, is_pointer, ""};
}

bool Context::IsPointerVariable(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).is_pointer;
        }
    }
    if (globals_.count(name)) {
        return globals_.at(name).is_pointer;
    }
    return false;
}

int Context::GetVariableOffset(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).stack_offset;
        }
    }
    if (globals_.count(name)) {
        return 0;
    }
    throw std::runtime_error("Variable not found: " + name);
}

Context::Type Context::GetVariableType(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).type;
        }
    }
    if (globals_.count(name)) {
        return globals_.at(name).type;
    }
    throw std::runtime_error("Variable type not found: " + name);
}

int Context::GetArraySize(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).array_size;
        }
    }
    if (globals_.count(name)) {
        return globals_.at(name).array_size;
    }
    throw std::runtime_error("Variable not found: " + name);
}

bool Context::IsGlobalVariable(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return false;
        }
    }
    return globals_.count(name) > 0;
}

int Context::GetMaxStackSize() const {
    int aligned_size = (max_stack_offset_ + 15) & ~15;
    return aligned_size == 0 ? 16 : aligned_size;
}

void Context::SetEpilogueLabel(const std::string& label) {
    epilogue_label_ = label;
}

const std::string& Context::GetEpilogueLabel() const {
    return epilogue_label_;
}

void Context::SetBreakLabel(const std::string& label) {
    break_label_ = label;
}

const std::string& Context::GetBreakLabel() const {
    return break_label_;
}

void Context::SetContinueLabel(const std::string& label) {
    continue_label_ = label;
}

const std::string& Context::GetContinueLabel() const {
    return continue_label_;
}

int Context::GetUniqueLabelId() {
    return label_counter_++;
}

void Context::AddEnumConstant(const std::string& name, int value) {
    enum_constants_[name] = value;
}

bool Context::HasEnumConstant(const std::string& name) const {
    return enum_constants_.count(name) > 0;
}

int Context::GetEnumConstant(const std::string& name) const {
    return enum_constants_.at(name);
}

void Context::AddTypedef(const std::string& name, Type type) {
    typedef_names_[name] = type;
}

bool Context::HasTypedef(const std::string& name) const {
    return typedef_names_.count(name) > 0;
}

Context::Type Context::GetTypedefType(const std::string& name) const {
    return typedef_names_.at(name);
}

void Context::AddStructType(const std::string& name, const StructType& st) {
    struct_types_[name] = st;
}

bool Context::HasStructType(const std::string& name) const {
    return struct_types_.count(name) > 0;
}

const Context::StructType& Context::GetStructType(const std::string& name) const {
    return struct_types_.at(name);
}

void Context::AddStructVariable(const std::string& name, const std::string& struct_name) {
    if (scopes_.empty()) {
        throw std::runtime_error("No scope available.");
    }
    const StructType& st = struct_types_.at(struct_name);
    current_stack_offset_ += st.total_size;
    scopes_.back()[name] = {Type::STRUCT, -current_stack_offset_, st.total_size, 0, false, false, struct_name};
    if (current_stack_offset_ > max_stack_offset_) {
        max_stack_offset_ = current_stack_offset_;
    }
}

const std::string& Context::GetStructName(const std::string& var_name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(var_name)) {
            return it->at(var_name).struct_name;
        }
    }
    if (globals_.count(var_name)) {
        return globals_.at(var_name).struct_name;
    }
    throw std::runtime_error("Variable not found: " + var_name);
}

int Context::GetStructMemberOffset(const std::string& struct_name, const std::string& member) const {
    const StructType& st = struct_types_.at(struct_name);
    for (const auto& m : st.members) {
        if (m.name == member) {
            return m.offset;
        }
    }
    throw std::runtime_error("Member not found: " + member);
}

}

