#pragma once
#include "AVLTree.h"
#include "Bin.h"
#include "Object.h"
#include "Color.h"
#include <stdexcept>
#include <climits>

class GCMS {
private:
    AVLTree<int, Bin*> bins_by_id;
    AVLTree<std::pair<int, int>, Bin*> bins_by_capacity; // (capacity, id)
    AVLTree<int, Object*> objects;

    Bin* blue(const Object* obj) {
        Bin* answer = nullptr;
        auto keys = bins_by_capacity.inorder_keys();
        for (auto& key : keys) {
            if (key.first >= obj->size) {
                answer = bins_by_capacity.search(key);
                break;
            }
        }
        return answer;
    }

    
    Bin* green(const Object* obj) {
        Bin* answer = nullptr;
        auto keys = bins_by_capacity.inorder_keys();
        for (auto it = keys.rbegin(); it != keys.rend(); ++it) {
            answer = bins_by_capacity.search(*it);
            break;
        }
        return (answer && answer->capacity >= obj->size) ? answer : nullptr;
    }

    Bin* red(const Object* obj) {
        Bin* answer = nullptr;
        auto keys = bins_by_capacity.inorder_keys();
        for (auto it = keys.rbegin(); it != keys.rend(); ++it) {
            if (bins_by_capacity.search(*it)->capacity >= obj->size) {
                answer = bins_by_capacity.search(*it);
            }
        }
        return answer;
    }

    Bin* yellow(const Object* obj) {
        Bin* answer = nullptr;
        auto keys = bins_by_capacity.inorder_keys();
        for (auto& key : keys) {
            if (key.first >= obj->size) {
                answer = bins_by_capacity.search(key);
            }
        }
        return answer;
    }

public:
    void add_bin(int bin_id, int capacity) {
        Bin* bin = new Bin(bin_id, capacity);
        bins_by_id.insert(bin_id, bin);
        bins_by_capacity.insert({capacity, bin_id}, bin);
    }

    void add_object(int object_id, int size, Color color) {
        Object* obj = new Object(object_id, size, color);
        Bin* target_bin = nullptr;

        if (color == Color::BLUE) target_bin = blue(obj);
        else if (color == Color::GREEN) target_bin = green(obj);
        else if (color == Color::RED) target_bin = red(obj);
        else target_bin = yellow(obj);

        if (!target_bin) throw std::runtime_error("NoBinFoundException");

        bins_by_capacity.remove({target_bin->capacity, target_bin->id});
        target_bin->add_object(obj);
        bins_by_capacity.insert({target_bin->capacity, target_bin->id}, target_bin);

        objects.insert(object_id, obj);
    }

    void delete_object(int object_id) {
        Object* obj = objects.search(object_id);
        if (!obj) return;

        Bin* bin = obj->bin;
        int prev_capacity = bin->capacity;

        bin->remove_object(object_id);
        bins_by_capacity.remove({prev_capacity, bin->id});
        bins_by_capacity.insert({bin->capacity, bin->id}, bin);

        objects.remove(object_id);
    }

    std::pair<int, std::vector<int>> bin_info(int bin_id) {
        Bin* bin = bins_by_id.search(bin_id);
        return { bin->capacity, bin->list_object_ids() };
    }

    int object_info(int object_id) {
        Object* obj = objects.search(object_id);
        return obj->bin->id;
    }
};