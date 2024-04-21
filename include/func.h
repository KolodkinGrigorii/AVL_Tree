#include "AVLTree.h"

template <typename T>
AVLTree<T>::AVLTree() : root{ nullptr }, size{ 0 } {}

template <typename T>
AVLTree<T>::~AVLTree() {
    
}

template <typename T>
bool AVLTree<T>::insert(const T& key) {
    root = insertHelper(root, key);
    if (root != nullptr) {
        size++;
        return true;
    }
    return false;
}

template <typename T>
bool AVLTree<T>::remove(const T& key) {
    if (removeHelper(root, key)) {
        size--;
        return true;
    }
    return false;
}

template <typename T>
bool AVLTree<T>::search(const T& key) const {
    return searchHelper(root, key);
}

template <typename T>
T AVLTree<T>::min() const {
    return minHelper(root)->key;
}

template <typename T>
T AVLTree<T>::max() const {
    return maxHelper(root)->key;
}

template <typename T>
AVLTreeIterator<T> AVLTree<T>::begin() const {
    return AVLTreeIterator<T>(minHelper(root));
}

template <typename T>
AVLTreeIterator<T> AVLTree<T>::end() const {
    return AVLTreeIterator<T>(nullptr);
}

template <typename T>
Node<T>* AVLTree<T>::insertHelper(Node<T>* node, const T& key) {
    if (node == nullptr) {
        return new Node<T>(key);
    }

    if (key <= node->key) {
        node->left = insertHelper(node->left, key);
        node->left->parent = node;
    }
    else if (key > node->key) {
        node->right = insertHelper(node->right, key);
        node->right->parent = node;
    }

    updateHeight(node);
    return rebalance(node);
}

template <typename T>
bool AVLTree<T>::removeHelper(Node<T>*& node, const T& key) {
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
            Node<T>* temp = node;
            node = node->right;
            delete temp;
            return true;
        }
        else if (node->right == nullptr) {
            Node<T>* temp = node;
            node = node->left;
            delete temp;
            return true;
        }
        else {
            Node<T>* successor = minHelper(node->right);
            node->key = successor->key;
            return removeHelper(node->right, successor->key);
        }
    }
}

template <typename T>
bool AVLTree<T>::searchHelper(Node<T>* node, const T& key) const {
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

template <typename T>
Node<T>* AVLTree<T>::minHelper(Node<T>* node) const {
    if (node->left == nullptr) {
        return node;
    }
    else {
        return minHelper(node->left);
    }
}

template <typename T>
Node<T>* AVLTree<T>::maxHelper(Node<T>* node) const {
    if (node->right == nullptr) {
        return node;
    }
    else {
        return maxHelper(node->right);
    }
}

template <typename T>
int AVLTree<T>::heightHelper(Node<T>* node) const {
    return node == nullptr ? 0 : node->height;
}

template <typename T>
int AVLTree<T>::getBalance(Node<T>* node) const {
    return heightHelper(node->left) - heightHelper(node->right);
}

template <typename T>
void AVLTree<T>::updateHeight(Node<T>* node) {
    node->height = std::max(heightHelper(node->left), heightHelper(node->right)) + 1;
}

template <typename T>
Node<T>* AVLTree<T>::rotateLeft(Node<T>* node) {
    Node<T>* right = node->right;
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

template <typename T>
Node<T>* AVLTree<T>::rotateRight(Node<T>* node) {
    Node<T>* left = node->left;
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

template <typename T>
Node<T>* AVLTree<T>::rebalance(Node<T>* node) {
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