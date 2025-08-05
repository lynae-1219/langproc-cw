#include "ast_context.hpp"
#include <stdexcept>

namespace ast {

// CRITICAL FIX: The context must be initialized with a global scope.
Context::Context() { PushScope(); }

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

void Context::AddVariable(const std::string& name, Type type, int size) {
    if (scopes_.empty()) {
        throw std::runtime_error("No scope available to add variable.");
    }
    int var_size = (type == Context::Type::DOUBLE) ? 8 : size;
    current_stack_offset_ += var_size;
    
    scopes_.back()[name] = {type, -current_stack_offset_, var_size};

    if (current_stack_offset_ > max_stack_offset_) {
        max_stack_offset_ = current_stack_offset_;
    }
}

int Context::GetVariableOffset(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).stack_offset;
        }
    }
    throw std::runtime_error("Variable not found: " + name);
}

Context::Type Context::GetVariableType(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).type;
        }
    }
    throw std::runtime_error("Variable type not found: " + name);
}

int Context::GetMaxStackSize() const {
    int aligned_size = (max_stack_offset_ + 15) & ~15;
    return aligned_size == 0 ? 16 : aligned_size;
}

void Context::SetEpilogueLabel(const std::string& label) {
    current_function_epilogue_label = label;
}

const std::string& Context::GetEpilogueLabel() const {
    return current_function_epilogue_label;
}

int Context::GetUniqueLabelId() {
    return label_counter_++;
}

} // namespace ast