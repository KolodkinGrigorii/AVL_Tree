#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <vector>
using namespace std;

template <typename TypeKey, typename TypeData>
class Node {
public:
    TypeKey key;
    TypeData data;
    Node* left;
    Node* right;
    Node* parent;
    int height;

    Node(const TypeKey& key, const TypeData& data) : key{ key }, data{ data }, left { nullptr }, right{ nullptr }, height{ 1 }, parent{ nullptr } {}
};

template <typename TypeKey, typename TypeData>
class AVLTreeIterator {
public:
    AVLTreeIterator(Node<TypeKey, TypeData>* node) : current{ node } {}

    TypeData& operator*() { return current->data; }
    Node<TypeKey, TypeData>* operator->() { return current; }
    Node<TypeKey, TypeData>* getcurrent() { return current; }

    AVLTreeIterator<TypeKey, TypeData>& operator++() {
        current = successorHelper(current);
        return *this;
    }
    Node<TypeKey, TypeData>* minHelper(Node<TypeKey, TypeData>* node) const {
        if (node->left == nullptr) {
            return node;
        }
        else {
            return minHelper(node->left);
        }
    }
    Node<TypeKey, TypeData>* maxHelper(Node<TypeKey, TypeData>* node) const {
        if (node->right == nullptr) {
            return node;
        }
        else {
            return maxHelper(node->right);
        }
    }
    bool operator==(const AVLTreeIterator<TypeKey, TypeData>& other) const {
        return current == other.current;
    }

    bool operator!=(const AVLTreeIterator<TypeKey, TypeData>& other) const {
        return current != other.current;
    }

private:
    Node<TypeKey, TypeData>* current;
    Node<TypeKey, TypeData>* successorHelper(Node<TypeKey, TypeData>* node) const {
        if (node->right != nullptr) {
            return minHelper(node->right);
        }
        Node<TypeKey, TypeData>* temp = node->parent;
        while (temp!=nullptr && temp->parent!=nullptr && temp->key != temp->parent->key) {
            temp = temp->parent;
        }
        if (node->key == maxHelper(temp)->key) {
            return nullptr;
        }
        Node<TypeKey, TypeData>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        return parent;
    }
};
template <typename TypeKey, typename TypeData>
class AVLTree {
public:
    vector<Node<TypeKey, TypeData>*> order;
    vector<bool> was = vector<bool>(10000);
    AVLTree();
    ~AVLTree();

    bool insert(const TypeKey& key, const TypeData& data);
    bool remove(const TypeKey& key);
    bool search(const TypeKey& key) const;
    AVLTreeIterator<TypeKey, TypeData> find(const TypeKey& key) {
        Node<TypeKey, TypeData>* node = root;
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
        return AVLTreeIterator<TypeKey, TypeData>(node);
    }
    TypeData& operator[](TypeKey fkey) {
        Node<TypeKey, TypeData>* node = root;
        while (node->key != fkey) {
            if (fkey <= node->key && node->left != NULL) {
                node = node->left;
            }
            else if (fkey > node->key && node->right != NULL) {
                node = node->right;
            }
            else {
                throw "Invalid key";
            }
        }
        return node->data;
    }
    int getsize() {
        return size;
    }
    void DFS(Node<TypeKey, TypeData>* node) {
        if (node->left != nullptr && !was[node->left->key]) {
            DFS(node->left);
        }
        if (node->right != nullptr && !was[node->right->key]) {
            DFS(node->right);
        }
        was[node->key] = true;
        order.push_back(node);
    }
    bool checkbalance(){
        DFS(root);
        for (int i = 0; i < order.size(); i++) {
            if (getBalance(order[i]) > 1 || getBalance(order[i]) < -1) {
                return false;
            }
        }
        return true;
    }
    int getheight() {
        return root->height;
    }
    Node<TypeKey, TypeData>* rootfind(){
        return root;
    }

    TypeKey min() const;
    TypeKey max() const;

    AVLTreeIterator<TypeKey, TypeData> begin() const;
    AVLTreeIterator<TypeKey, TypeData> end() const;

private:
    Node<TypeKey, TypeData>* root;
    int size;

    Node<TypeKey, TypeData>* insertHelper(Node<TypeKey, TypeData>* node, const TypeKey& key, const TypeData& data);
    bool removeHelper(Node<TypeKey, TypeData>*& node, const TypeKey& key);
    bool searchHelper(Node<TypeKey, TypeData>* node, const TypeKey& key) const;

    Node<TypeKey, TypeData>* minHelper(Node<TypeKey, TypeData>* node) const;
    Node<TypeKey, TypeData>* maxHelper(Node<TypeKey, TypeData>* node) const;

    int heightHelper(Node<TypeKey, TypeData>* node) const;
    int getBalance(Node<TypeKey, TypeData>* node) const;
    void updateHeight(Node<TypeKey, TypeData>* node);

    Node<TypeKey, TypeData>* rotateLeft(Node<TypeKey, TypeData>* node);
    Node<TypeKey, TypeData>* rotateRight(Node<TypeKey, TypeData>* node);
    Node<TypeKey, TypeData>* bigRotateRight(Node<TypeKey, TypeData>* node);
    Node<TypeKey, TypeData>* bigRotateLeft(Node<TypeKey, TypeData>* node);
    Node<TypeKey, TypeData>* rebalance(Node<TypeKey, TypeData>* node);
};

#endif
