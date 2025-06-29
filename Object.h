// Object.h
#pragma once
#include "Color.h"
#include <memory>

class Bin;  // forward declaration

class Object {
public:
    int id;
    int size;
    Color color;
    Bin* bin;

    Object(int id_, int size_, Color color_)
        : id(id_), size(size_), color(color_), bin(nullptr) {}
};
