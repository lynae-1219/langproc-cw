#pragma once
#include <unordered_map>
#include <stack>
#include <set>
#include <string>

namespace ast {

class Context {
public:
    enum class Type {
        UNSIGNED_CHAR, UNSIGNED_SHORT, UNSIGNED_INT, UNSIGNED_LONG,
        CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, LONG_DOUBLE, VOID, STRUCT
    };

    struct Variable {
        Type type;
        int stack_offset;
    };

    // Register allocation
    std::string AllocRegister();
    void FreeRegister(const std::string& reg);

    // Scoping & variables
    void PushScope();
    void PopScope();
    void AddVariable(const std::string& name, Type type, int size);
    Variable LookupVariable(const std::string& name);  // Now Variable is known

    // Labels
    std::string NewLabel();

    // Type system
    int SizeOf(Type type) const;
    bool IsCompatible(Type a, Type b) const;

private:
    std::set<std::string> free_registers_ = {"t0", "t1", "t2", "a0", "a1"};
    std::stack<std::unordered_map<std::string, Variable>> symbol_tables_;
    int stack_offset_ = 0;
    int label_counter_ = 0;
};

} // namespace ast