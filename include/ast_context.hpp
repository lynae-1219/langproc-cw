// ast_context.hpp
#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
#include <ostream>

namespace ast {

class Context {
public:
    enum class Type { INT, FLOAT, CHAR }; 
    
    struct Variable {
        Type type;
        int stack_offset;
        int size;
    };

private:
    std::vector<std::map<std::string, Variable>> scopes_;
    std::vector<int> scope_offsets_; 
    int current_stack_offset_ = 0;
    int max_stack_offset_ = 0; 
    std::string current_function_epilogue_label;

public:
    Context();
    
    void PushScope();
    void PopScope();
    void AddVariable(const std::string& name, Type type, int size = 4);
    int GetVariableOffset(const std::string& name) const;
    int GetMaxStackSize() const;
    
    void SetEpilogueLabel(const std::string& label);
    const std::string& GetEpilogueLabel() const;
};

} // namespace ast