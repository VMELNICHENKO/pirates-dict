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

class PiratesDict {
public:
    PiratesDict(){};
    PiratesDict( panda::string filename ) { load_dict( filename ); }
    PiratesDict( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator );
    ~PiratesDict();

    void load_dict( panda::string filename );
    void process_node( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator );

    // SV* export();
    PiratesDict* get_child( panda::string key );
    PiratesDict* get_child_value( panda::string key ){};
    // TODO get_child_val directly
    // TODO get child by keys sequence ( array ref )
    void dump( uint32_t level = 0) const;
private:
    using ObjectMap = std::map<std::string, PiratesDict>;
    using ObjectArr = std::vector<PiratesDict>;

    std::variant<ObjectMap, ObjectArr, std::string, int64_t, double, bool> value;
};

namespace xs {
    // struct Typemap<MyClass> {
    //     static [inline] PiratesDict in(SV* arg) { ... }
    //     static [inline] Sv out(const PiratesDict& var, const Sv& proto = {}) { ... }
    //     static [inline] void destroy (const PiratesDict&, SV*) { ... }
    // }
    //    template<> struct Typemap<PiratesDict*> : TypemapObject<PiratesDict*, PiratesDict*,ObjectTypePtr,ObjectStorageMG> {};

    template <> struct Typemap<PiratesDict*> : TypemapObject<PiratesDict*, PiratesDict*, ObjectTypeForeignPtr, ObjectStorageMG> {
        static std::string package () { return "Pirates::Dict"; }
    };

}
