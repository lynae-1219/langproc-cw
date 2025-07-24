#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
#include <ostream>

namespace ast {

class Context {
public:
    enum class Type { INT, FLOAT, CHAR }; // Basic types for now
    
    struct Variable {
        Type type;
        int stack_offset;
        int size;
    };

private:
    std::vector<std::map<std::string, Variable>> scopes_;
    int stack_offset_ = 0;
    int scope_depth_ = 0;
    std::set<std::string> free_registers_ = {"t0", "t1", "t2", "a0", "a1"};

public:
    Context();
    
    void PushScope();
    void PopScope();
    void AddVariable(const std::string& name, Type type, int size = 4);
    int GetVariableOffset(const std::string& name) const;
    int GetCurrentStackSize() const;
    
    std::string AllocRegister();
    void FreeRegister(const std::string& reg);
};

} // namespace ast