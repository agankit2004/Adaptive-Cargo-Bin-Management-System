#pragma once
#include <algorithm>
#include <vector>

template<typename K, typename V>                                
class AVLNode {
public:
    K key;
    V value;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(K k, V v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template<typename K, typename V>
class AVLTree {
public:
    AVLNode<K, V>* root;

    AVLTree() : root(nullptr) {}

    int height(AVLNode<K, V>* node) {
        return node ? node->height : 0;
    }

    int balance(AVLNode<K, V>* node) {
        return height(node->left) - height(node->right);
    }

    AVLNode<K, V>* rightRotate(AVLNode<K, V>* y) {
        AVLNode<K, V>* x = y->left;
        AVLNode<K, V>* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = 1 + std::max(height(y->left), height(y->right));
        x->height = 1 + std::max(height(x->left), height(x->right));
        return x;
    }

    AVLNode<K, V>* leftRotate(AVLNode<K, V>* x) {
        AVLNode<K, V>* y = x->right;
        AVLNode<K, V>* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = 1 + std::max(height(x->left), height(x->right));
        y->height = 1 + std::max(height(y->left), height(y->right));
        return y;
    }

    AVLNode<K, V>* insert(AVLNode<K, V>* node, K key, V value) {
        if (!node) return new AVLNode<K, V>(key, value);
        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else
            return node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        int bal = balance(node);

        if (bal > 1 && key < node->left->key) return rightRotate(node);
        if (bal < -1 && key > node->right->key) return leftRotate(node);
        if (bal > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (bal < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void insert(K key, V value) {
        root = insert(root, key, value);
    }

    AVLNode<K, V>* minValueNode(AVLNode<K, V>* node) {
        AVLNode<K, V>* current = node;
        while (current->left) current = current->left;
        return current;
    }

    AVLNode<K, V>* remove(AVLNode<K, V>* root, K key) {
        if (!root) return root;

        if (key < root->key)
            root->left = remove(root->left, key);
        else if (key > root->key)
            root->right = remove(root->right, key);
        else {
            if (!root->left || !root->right) {
                AVLNode<K, V>* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                AVLNode<K, V>* temp = minValueNode(root->right);
                root->key = temp->key;
                root->value = temp->value;
                root->right = remove(root->right, temp->key);
            }
        }

        if (!root) return root;

        root->height = 1 + std::max(height(root->left), height(root->right));
        int bal = balance(root);

        if (bal > 1 && balance(root->left) >= 0) return rightRotate(root);
        if (bal > 1 && balance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (bal < -1 && balance(root->right) <= 0) return leftRotate(root);
        if (bal < -1 && balance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void remove(K key) {
        root = remove(root, key);
    }

    V search(K key) {
        AVLNode<K, V>* current = root;
        while (current) {
            if (key == current->key) return current->value;
            current = key < current->key ? current->left : current->right;
        }
        return nullptr;
    }

    void inorder_keys_helper(AVLNode<K, V>* node, std::vector<K>& result) {
        if (node) {
            inorder_keys_helper(node->left, result);
            result.push_back(node->key);
            inorder_keys_helper(node->right, result);
        }
    }

    std::vector<K> inorder_keys() {
        std::vector<K> result;
        inorder_keys_helper(root, result);
        return result;
    }
};
