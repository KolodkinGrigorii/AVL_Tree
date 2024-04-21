#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>

template <typename T>
class Node {
public:
    T key;
    Node* left;
    Node* right;
    Node* parent;
    int height;

    Node(const T& key) : key{ key }, left{ nullptr }, right{ nullptr }, height{ 1 }, parent{ nullptr } {}
};

template <typename T>
class AVLTreeIterator {
public:
    AVLTreeIterator(Node<T>* node) : current{ node } {}

    T& operator*() { return current->key; }
    T* operator->() { return &(current->key); }

    AVLTreeIterator<T>& operator++() {
        current = successorHelper(current);
        return *this;
    }
    Node<T>* minHelper(Node<T>* node) const {
        if (node->left == nullptr) {
            return node;
        }
        else {
            return minHelper(node->left);
        }
    }
    bool operator==(const AVLTreeIterator<T>& other) const {
        return current == other.current;
    }

    bool operator!=(const AVLTreeIterator<T>& other) const {
        return current != other.current;
    }

private:
    Node<T>* current;

    Node<T>* successorHelper(Node<T>* node) const {

        if (node->right != nullptr) {
            return minHelper(node->right);
        }

        Node<T>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }
};
template <typename T>
class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    bool insert(const T& key);
    bool remove(const T& key);
    bool search(const T& key) const;
    Node<T>* find(const T& key) {
        Node<T>* node = root;
        while (node->key != key) {
            if (node->key == key) {
                return node;
            }
            else if (key <= node->key) {
                node = node->left;
            }
            else if (key > node->key) {
                node = node->right;
            }
        }
    }
    Node<T>* rootfind(){
        return root;
    }
    T min() const;
    T max() const;

    AVLTreeIterator<T> begin() const;
    AVLTreeIterator<T> end() const;

private:
    Node<T>* root;
    int size;

    Node<T>* insertHelper(Node<T>* node, const T& key);
    bool removeHelper(Node<T>*& node, const T& key);
    bool searchHelper(Node<T>* node, const T& key) const;

    Node<T>* minHelper(Node<T>* node) const;
    Node<T>* maxHelper(Node<T>* node) const;

    int heightHelper(Node<T>* node) const;
    int getBalance(Node<T>* node) const;
    void updateHeight(Node<T>* node);

    Node<T>* rotateLeft(Node<T>* node);
    Node<T>* rotateRight(Node<T>* node);
    Node<T>* rebalance(Node<T>* node);
};

#endif