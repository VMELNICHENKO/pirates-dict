#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

//#include <helpers/json.h>
#include <panda/string.h>
#include <panda/string_view.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include <xs.h>

using namespace std;
using namespace rapidjson;

const int ISFLOAT  = 1;
const int ISSTRING = 2;
const int ISBOOL   = 3;
const int ISARRAY  = 4;
const int ISOBJECT = 5;
const int ISINT    = 6;
const int ISNILL   = 7;

class PiratesDict {
public:
    PiratesDict(){};
    PiratesDict( panda::string filename ) { load_dict( filename ); }
    PiratesDict( Value* node, Document::AllocatorType& allocator );
    ~PiratesDict();

    void load_dict( panda::string filename );
    void process_node( Value* node, Document::AllocatorType& allocator );

    // SV* export();
    PiratesDict* get( panda::string key );
    // TODO get_child_val directly
    // TODO get child by keys sequence ( array ref )
    void dump( uint32_t level = 0);
private:

    uint8_t type = ISNILL;

    /// TODO: only one may be initialized
    map<string, PiratesDict*> childs    ;
    vector<PiratesDict*>      childs_arr;
    string                    str_val   ;
    int64_t                   int_val   ;
    double                    float_val ;
    bool                      bool_val  ;

};

namespace xs {
    // struct Typemap<MyClass> {
    //     static [inline] PiratesDict in(SV* arg) { ... }
    //     static [inline] Sv out(const PiratesDict& var, const Sv& proto = {}) { ... }
    //     static [inline] void destroy (const PiratesDict&, SV*) { ... }
    // }
    template<> struct Typemap<PiratesDict*> : TypemapObject<PiratesDict*, PiratesDict*,ObjectTypePtr,ObjectStorageMG> {};
}
