#include "avl_tree.h"

template <typename TypeKey, typename TypeData>
AVLTree<TypeKey, TypeData>::AVLTree() : root{ nullptr }, size{ 0 } {}

template <typename TypeKey, typename TypeData>
AVLTree<TypeKey, TypeData>::~AVLTree() {
    
}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::insert(const TypeKey& key, const TypeData& data) {
    root = insertHelper(root, key, data);
    if (root != nullptr) {
        size++;
        return true;
    }
    return false;
}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::remove(const TypeKey& key) {
    if (removeHelper(root, key)) {
        size--;
        return true;
    }
    return false;
}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::search(const TypeKey& key) const {
    return searchHelper(root, key);
}

template <typename TypeKey, typename TypeData>
TypeKey AVLTree<TypeKey, TypeData>::min() const {
    return minHelper(root)->key;
}

template <typename TypeKey, typename TypeData>
TypeKey AVLTree<TypeKey, TypeData>::max() const {
    return maxHelper(root)->key;
}

template <typename TypeKey, typename TypeData>
AVLTreeIterator<TypeKey, TypeData> AVLTree<TypeKey, TypeData>::begin() const {
    return AVLTreeIterator<TypeKey, TypeData>(minHelper(root));
}

template <typename TypeKey, typename TypeData>
AVLTreeIterator<TypeKey, TypeData> AVLTree<TypeKey, TypeData>::end() const {
    return AVLTreeIterator<TypeKey, TypeData>(nullptr);
}

template <typename TypeKey, typename TypeData>
Node<TypeKey, TypeData>* AVLTree<TypeKey, TypeData>::insertHelper(Node<TypeKey, TypeData>* node, const TypeKey& key, const TypeData& data) {
    if (node == nullptr) {
        return new Node<TypeKey, TypeData>(key, data);
    }

    if (key <= node->key) {
        node->left = insertHelper(node->left, key, data);
        node->left->parent = node;
    }
    else if (key > node->key) {
        node->right = insertHelper(node->right, key, data);
        node->right->parent = node;
    }

    updateHeight(node);
    return rebalance(node);
}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::removeHelper(Node<TypeKey, TypeData>*& node, const TypeKey& key) {
    if (node == nullptr) {
        return false;
    }

    if (key < node->key) {
        return removeHelper(node->left, key);
    }
    else if (key > node->key) {
        return removeHelper(node->right, key);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
            return true;
        }
        else if (node->left == nullptr) {
            Node<TypeKey, TypeData>* temp = node;
            node = node->right;
            node->parent = temp->parent;
            delete temp;
            return true;
        }
        else if (node->right == nullptr) {
            Node<TypeKey, TypeData>* temp = node;
            node = node->left;
            node->parent = temp->parent;
            delete temp;
            return true;
        }
        else {
            Node<TypeKey, TypeData>* successor = minHelper(node->right);
            node->key = successor->key;
            return removeHelper(node->right, successor->key);
        }
    }
}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::searchHelper(Node<TypeKey, TypeData>* node, const TypeKey& key) const {
    if (node == nullptr) {
        return false;
    }

    if (key < node->key) {
        return searchHelper(node->left, key);
    }
    else if (key > node->key) {
        return searchHelper(node->right, key);
    }
    else {
        return true;
    }
}

template <typename TypeKey, typename TypeData>
Node<TypeKey, TypeData>* AVLTree<TypeKey, TypeData>::minHelper(Node<TypeKey, TypeData>* node) const {
    if (node->left == nullptr) {
        return node;
    }
    else {
        return minHelper(node->left);
    }
}

template <typename TypeKey, typename TypeData>
Node<TypeKey, TypeData>* AVLTree<TypeKey, TypeData>::maxHelper(Node<TypeKey, TypeData>* node) const {
    if (node->right == nullptr) {
        return node;
    }
    else {
        return maxHelper(node->right);
    }
}

template <typename TypeKey, typename TypeData>
int AVLTree<TypeKey, TypeData>::heightHelper(Node<TypeKey, TypeData>* node) const {
    return node == nullptr ? 0 : node->height;
}

template <typename TypeKey, typename TypeData>
int AVLTree<TypeKey, TypeData>::getBalance(Node<TypeKey, TypeData>* node) const {
    return heightHelper(node->left) - heightHelper(node->right);
}

template <typename TypeKey, typename TypeData>
void AVLTree<TypeKey, TypeData>::updateHeight(Node<TypeKey, TypeData>* node) {
    node->height = std::max(heightHelper(node->left), heightHelper(node->right)) + 1;
}

template <typename TypeKey, typename TypeData>
Node<TypeKey, TypeData>* AVLTree<TypeKey, TypeData>::rotateLeft(Node<TypeKey, TypeData>* node) {
    Node<TypeKey, TypeData>* right = node->right;
    node->right = right->left;
    if (right->left != nullptr) {
        right->left->parent = node;
    }
    right->left = node;
    node->parent = right;
    right->parent = node->parent;
    updateHeight(node);
    updateHeight(right);
    return right;
}

template <typename TypeKey, typename TypeData>
Node<TypeKey, TypeData>* AVLTree<TypeKey, TypeData>::rotateRight(Node<TypeKey, TypeData>* node) {
    Node<TypeKey, TypeData>* left = node->left;
    node->left = left->right;
    if (left->right != nullptr) {
        left->right->parent = node;
    }
    left->right = node;
    node->parent = left;
    left->parent = node->parent;
    updateHeight(node);
    updateHeight(left);
    return left;
}

template <typename TypeKey, typename TypeData>
Node<TypeKey, TypeData>* AVLTree<TypeKey, TypeData>::rebalance(Node<TypeKey, TypeData>* node) {
    int balance = getBalance(node);

    if (balance > 1) {
        if (getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    else if (balance < -1) {
        if (getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
};