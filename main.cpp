
#include <cassert>
#include <iostream>
#include <memory>

using namespace std;

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    // Используйте TreeNodePtr<T> вместо сырых указателей
    // Примите умные указатели по rvalue-ссылке
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <class T>
void PrintTree(const TreeNode<T>* root, ostream& out = cout) {
    out << " ( "s;
    out << root->value;
    if (root->left || root->right) {
        if (root->left) {
            PrintTree(root->left.get() , out);
        } else {
            out << "*"s;
        }
        if (root->right) {
            PrintTree(root->right.get(), out);
        } else {
            out << "*"s;
        }
    }
    out << " ) "s;
}

template <class T>
ostream& operator << (ostream& out, const TreeNode<T>* node) {
    PrintTree(node, out);
    return out;
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) {

    if (node == nullptr) {
        return true;
    }

    if (min != nullptr && node->value <= *min) {
        return false;
    }

    if (max != nullptr && node->value >= *max) {
        return false;
    }

    return CheckTreeProperty(node->left.get(), min, &node->value) && CheckTreeProperty(node->right.get(), &node->value, max);
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    T* min = nullptr;
    T* max = nullptr;
    return  CheckTreeProperty(node, min, max);
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    if (node) {
       while (node->left) {
           node = node->left.get();
       }
    }

    return node;
}

template <typename T>
TreeNode<T>* leaveVisitedRightSubtree(TreeNode<T>* node) {
    bool is_right_child = node->parent && node->parent->right.get() == node;
    while (is_right_child) {
        node = node->parent;
        is_right_child = node->parent && node->parent->right.get() == node;
    }

    return node->parent;
}

template <class T>
T max(TreeNode<T>* node) {
    if (node->parent == nullptr)
    {
        while (node->right != nullptr)
          node = node->right.get();
           return node->value;
    }
    TreeNode<T>* parent_ = node->parent;
    while(parent_->parent != nullptr)
          parent_ = parent_->parent;
    while (parent_->right != nullptr)
          parent_ = parent_->right.get();
    return parent_->value;
}

template <class T>
TreeNode<T>* next(TreeNode<T>* node) {
    if (node->value == max(node))
        return nullptr;
    if (node->right) {
        return begin(node->right.get());
    } else {
        TreeNode<T>* parent_ = node->parent;
        if(parent_->left.get() == node)
            return parent_;
        if(parent_->right.get() == node){
            while(parent_->parent != nullptr)
                parent_ = parent_->parent;
            return parent_;
        }
    }
    return nullptr;
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    auto res = std::make_unique<TreeNode<int>>(std::move(val), std::move(left), std::move(right));
    if (res.get()->left) {
        res.get()->left->parent = res.get();
    }
    if (res.get()->right) {
        res.get()->right->parent = res.get();
    }
    return res;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    std::cout << "Check" << std::endl;
}

