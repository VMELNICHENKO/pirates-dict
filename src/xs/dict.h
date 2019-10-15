#pragma once
#include <xs.h>
#include <dict.hpp>
using namespace json_tree;

namespace xs {
    template <class T> struct Typemap<Dict*, T> : TypemapObject<Dict*, T, ObjectTypeForeignPtr, ObjectStorageMG> {
        static std::string package () { return "CSGame::Dict"; }
    };
}
