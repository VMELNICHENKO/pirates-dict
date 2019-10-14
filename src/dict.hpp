#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>

#include <panda/string.h>
#include <panda/string_view.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include <xs.h>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

class Dict {
public:
    Dict(){};
    Dict( panda::string filename ) { load_dict( filename ); }
    Dict( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator );
    ~Dict(){};

    void load_dict( panda::string filename );
    void process_node( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator );

    const Dict* get( const std::vector<std::string>& keys, uint64_t index = 0 ) const;

    void dump( uint32_t level = 0) const;
private:
    using ObjectMap = std::map<std::string, Dict>;
    using ObjectArr = std::vector<Dict>;

    std::variant<ObjectMap, ObjectArr, std::string, int64_t, double, bool> value;
};

namespace xs {

    template <class T> struct Typemap<Dict*, T> : TypemapObject<Dict*, T, ObjectTypeForeignPtr, ObjectStorageMG> {
        static std::string package () { return "CSGame::Dict"; }
    };

}
