#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
#include <ostream>
#include <atomic>

#include "ast_type_specifier.hpp"

namespace ast {

class Context {
public:
    enum class Type { INT, FLOAT, DOUBLE, CHAR };
    
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
    std::atomic<int> label_counter_ = 0;

    // These members are now managed directly by FunctionDefinition
    friend class FunctionDefinition;

public:
    Context();
    
    void PushScope();
    void PopScope();
    void AddVariable(const std::string& name, Type type, int size = 4);
    int GetVariableOffset(const std::string& name) const;
    Type GetVariableType(const std::string& name) const;
    int GetMaxStackSize() const;
    int GetUniqueLabelId();
    
    void SetEpilogueLabel(const std::string& label);
    const std::string& GetEpilogueLabel() const;
};

inline Context::Type SpecifierToContextType(TypeSpecifier spec) {
    switch (spec) {
        case TypeSpecifier::INT: return Context::Type::INT;
        case TypeSpecifier::FLOAT: return Context::Type::FLOAT;
        case TypeSpecifier::DOUBLE: return Context::Type::DOUBLE;
        default: throw std::runtime_error("Unsupported type specifier");
    }
}

} // namespace ast