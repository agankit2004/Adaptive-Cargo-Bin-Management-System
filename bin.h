#pragma once
#include "AVLTree.h"
#include "Object.h"

class Bin {
public:
    int id;
    int capacity;
    AVLTree<int, Object*> objects;

    Bin(int bin_id, int cap) : id(bin_id), capacity(cap) {}

    void add_object(Object* obj) {
        if (capacity >= obj->size) {
            objects.insert(obj->id, obj);
            obj->bin = this;
            capacity -= obj->size;
        }
    }

    void remove_object(int object_id) {
        Object* obj = objects.search(object_id);
        if (obj) {
            capacity += obj->size;
            objects.remove(object_id);
        }
    }

    std::vector<int> list_object_ids() {
        return objects.inorder_keys();
    }
};
