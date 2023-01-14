
#include <cassert>
#include <iostream>

template <typename T>
struct TreeNode {
    T value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

template <typename T>
void DeleteTree(TreeNode<T>* node) {
    if (!node) {
        return;
    }
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) {
    if (node == nullptr) {
        return true;
    }

    // right branch
    if (min != nullptr && node->value <= *min) {
        return false;
    }

    // left branch
    if (max != nullptr && node->value >= *max) {
        return false;
    }

    return CheckTreeProperty(node->left, min, &node->value) && CheckTreeProperty(node->right, &node->value, max);
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

int main() {
    using T = TreeNode<int>;
    T* root1 = new T{6,
        new T{4, new T{3}, new T{5}}, new T{7}};
    assert(CheckTreeProperty(root1));

    T* root2 = new T{6,
        new T{4, new T{3}, new T{5}}, new T{7, new T{8}}};
    assert(!CheckTreeProperty(root2));

    T* root3 = new T{19,
            new T{8, new T{0}, new T{11, nullptr, new T{16}}}, new T{23, new T{22, nullptr, new T{41}}, new T{25, nullptr ,new T{27}}}}; //
    assert(!CheckTreeProperty(root3));

    T* root4 = nullptr;
    assert(CheckTreeProperty(root4));

    DeleteTree(root1);
    DeleteTree(root2);
    DeleteTree(root3);
    DeleteTree(root4);

    std::cout << "Tests is ok" << std::endl;
}
