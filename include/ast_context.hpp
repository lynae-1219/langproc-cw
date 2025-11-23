#pragma once
#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <atomic>

#include "ast_type_specifier.hpp"

namespace ast {

class Context {
public:
    enum class Type { INT, FLOAT, DOUBLE, CHAR, STRUCT };

    struct StructMember {
        std::string name;
        Type type;
        int offset;
        int size;
    };

    struct StructType {
        std::vector<StructMember> members;
        int total_size;
    };

    struct Variable {
        Type type;
        int stack_offset;
        int size;
        int array_size;
        bool is_global;
        bool is_pointer;
        std::string struct_name;
    };

private:
    std::map<std::string, Variable> globals_;
    std::vector<std::map<std::string, Variable>> scopes_;
    std::vector<int> scope_offsets_;
    int current_stack_offset_ = 0;
    int max_stack_offset_ = 0;
    std::string epilogue_label_;
    std::string break_label_;
    std::string continue_label_;
    std::atomic<int> label_counter_{0};
    std::map<std::string, int> enum_constants_;
    std::map<std::string, Type> typedef_names_;
    std::map<std::string, StructType> struct_types_;

    friend class FunctionDefinition;

public:
    Context();

    void PushScope();
    void PopScope();
    void AddVariable(const std::string& name, Type type, int size = 4, int array_size = 0, bool is_pointer = false);
    void AddGlobalVariable(const std::string& name, Type type, int array_size = 0, bool is_pointer = false);
    bool IsPointerVariable(const std::string& name) const;
    int GetVariableOffset(const std::string& name) const;
    Type GetVariableType(const std::string& name) const;
    int GetArraySize(const std::string& name) const;
    bool IsGlobalVariable(const std::string& name) const;
    int GetMaxStackSize() const;
    int GetUniqueLabelId();

    void SetEpilogueLabel(const std::string& label);
    const std::string& GetEpilogueLabel() const;
    void SetBreakLabel(const std::string& label);
    const std::string& GetBreakLabel() const;
    void SetContinueLabel(const std::string& label);
    const std::string& GetContinueLabel() const;

    const std::map<std::string, Variable>& GetGlobals() const { return globals_; }

    void AddEnumConstant(const std::string& name, int value);
    bool HasEnumConstant(const std::string& name) const;
    int GetEnumConstant(const std::string& name) const;

    void AddTypedef(const std::string& name, Type type);
    bool HasTypedef(const std::string& name) const;
    Type GetTypedefType(const std::string& name) const;

    void AddStructType(const std::string& name, const StructType& st);
    bool HasStructType(const std::string& name) const;
    const StructType& GetStructType(const std::string& name) const;
    void AddStructVariable(const std::string& name, const std::string& struct_name);
    const std::string& GetStructName(const std::string& var_name) const;
    int GetStructMemberOffset(const std::string& struct_name, const std::string& member) const;
};

inline Context::Type SpecifierToContextType(TypeSpecifier spec) {
    switch (spec) {
        case TypeSpecifier::INT: return Context::Type::INT;
        case TypeSpecifier::FLOAT: return Context::Type::FLOAT;
        case TypeSpecifier::DOUBLE: return Context::Type::DOUBLE;
        case TypeSpecifier::CHAR: return Context::Type::CHAR;
        case TypeSpecifier::UNSIGNED: return Context::Type::INT;
        case TypeSpecifier::VOID: return Context::Type::INT;
        default: throw std::runtime_error("Unsupported type specifier");
    }
}

}
