#include "ast_context.hpp"
#include <stdexcept>

namespace ast {

Context::Context() { PushScope(); }

void Context::PushScope() {
    scopes_.push_back({});
}

void Context::PopScope() {
    scopes_.pop_back();
    // Note: A more robust implementation would restore the previous stack offset
}

void Context::AddVariable(const std::string& name, Type type, int size) {
    // Variables are allocated from the top of the frame downwards
    current_stack_offset_ += size;
    scopes_.back()[name] = {type, -current_stack_offset_, size}; // Use negative offsets from Frame Pointer

    // Update max stack size if we've gone deeper
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
    return 0; // Return 0 or throw error if not found
}

int Context::GetMaxStackSize() const {
    // Ensure stack size is aligned to 16 bytes for RISC-V
    return (max_stack_offset_ + 15) & ~15;
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