#include "ast_context.hpp"
#include <stdexcept>

namespace ast {

Context::Context() { PushScope(); }

void Context::PushScope() {
    scopes_.push_back({});
    scope_depth_++;
}

void Context::PopScope() {
    if (scope_depth_ > 1) {
        int scope_size = 0;
        for (const auto& [name, var] : scopes_.back()) {
            scope_size += var.size;
        }
        stack_offset_ -= scope_size;
        scopes_.pop_back();
        scope_depth_--;
    }
}

void Context::AddVariable(const std::string& name, Type type, int size) {
    scopes_.back()[name] = {type, stack_offset_, size};
    stack_offset_ += size;
}

int Context::GetVariableOffset(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).stack_offset;
        }
    }
    return -1;
}

int Context::GetCurrentStackSize() const {
    return stack_offset_;
}

std::string Context::AllocRegister() {
    if (free_registers_.empty()) {
        throw std::runtime_error("No free registers available");
    }
    auto reg = *free_registers_.begin();
    free_registers_.erase(free_registers_.begin());
    return reg;
}

void Context::FreeRegister(const std::string& reg) {
    free_registers_.insert(reg);
}

} // namespace ast