#include "avl_tree.h"

template <typename TypeKey, typename TypeData>
AVLTree<TypeKey, TypeData>::AVLTree() : root{ nullptr }, size{ 0 } {}

template <typename TypeKey, typename TypeData>
AVLTree<TypeKey, TypeData>::~AVLTree() {

}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::insert(const TypeKey& key, const TypeData& data) {
    root = insertHelper(root, key, data);
    size++;
    if (root != nullptr) {
        return true;
    }
    return false;
}

template <typename TypeKey, typename TypeData>
bool AVLTree<TypeKey, TypeData>::remove(const TypeKey& key) {
    size--;
    if (removeHelper(root, key)) {
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
        vector<Node<TypeKey, TypeData>*> was;
        Node<TypeKey, TypeData>* temp = root;
        while (temp->key != node->key) {
            was.push_back(temp);
            if (node->key <= temp->key) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        if (node->left == nullptr && node->right == nullptr) {
            Node<TypeKey, TypeData>* parent = node->parent;
            if (parent!=nullptr && key < parent->key) {
                parent->left=nullptr;
            }
            else if (parent != nullptr) {
                parent->right = nullptr;
            }
            else {
                root = nullptr;
            }
            delete node;
            node = nullptr;
        }
        else if (node->left == nullptr) {
            Node<TypeKey, TypeData>* temp = node;
            node = node->right;
            node->parent = temp->parent;
            delete temp;
        }
        else if (node->right == nullptr) {
            Node<TypeKey, TypeData>* temp = node;
            node = node->left;
            node->parent = temp->parent;
            delete temp;
        }
        else {
            Node<TypeKey, TypeData>* successor = minHelper(node->right);
            removeHelper(node->right, successor->key);
            node->key = successor->key;
            node->data = successor->data;
        }
        for (int i = 0; i < was.size(); i++) {
            updateHeight(was[i]);
        }
        for (int i = 0; i < was.size(); i++) {
            if (checkbalance(root)) {
                break;
            }
            rebalance(was[i]);
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
    if (node == nullptr) {
        return 0;
    }
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
    right->parent = node->parent;
    if (node->parent == nullptr) {
        root = right;
    }
    node->parent = right;
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
    left->parent = node->parent;
    if (node->parent == nullptr) {
        root = left;
    }
    node->parent = left;
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