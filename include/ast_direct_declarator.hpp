#pragma once
#include "ast_node.hpp"
#include "ast_identifier.hpp"

namespace ast {

class DirectDeclarator : public Node {
private:
    NodePtr identifier_;
    NodePtr parameters_;
    int array_size_;
    bool is_function_;
    bool is_pointer_;

public:
    DirectDeclarator(NodePtr identifier)
        : identifier_(std::move(identifier)), parameters_(nullptr), array_size_(0), is_function_(false), is_pointer_(false) {}

    DirectDeclarator(NodePtr identifier, int array_size)
        : identifier_(std::move(identifier)), parameters_(nullptr), array_size_(array_size), is_function_(false), is_pointer_(false) {}

    Context::Type GetType(Context& context) const override {
        (void)context;
        throw std::runtime_error("GetType called on DirectDeclarator");
    }

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

    const Node* GetIdentifier() const { return identifier_.get(); }
    void SetParameters(NodePtr params) { parameters_ = std::move(params); }
    NodePtr TakeParameters() { return std::move(parameters_); }
    int GetArraySize() const { return array_size_; }
    void SetArraySize(int size) { array_size_ = size; }
    bool IsFunction() const { return is_function_; }
    void SetIsFunction(bool val) { is_function_ = val; }
    bool IsPointer() const { return is_pointer_; }
    void SetIsPointer(bool val) { is_pointer_ = val; }
};

}
