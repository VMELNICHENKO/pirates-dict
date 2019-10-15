#include <dict.hpp>
#include <xs/export.h>
#include <xs.h>

#define MAX_ERR_SIZE   256

namespace xs {
    template <class T> struct Typemap<Dict*, T> : TypemapObject<Dict*, T, ObjectTypeForeignPtr, ObjectStorageMG> {
        static std::string package () { return "CSGame::Dict"; }
    };
}

using namespace xs;

using panda::string;
using panda::string_view;

Sv dict2sv (const Dict* dict) {
    return std::visit( overloaded{
            [](const Dict::ObjectMap& m) -> Sv {
                auto ret = Hash::create(m.size());
                for ( const auto& r : m ) ret[r.first.c_str()] = dict2sv( &r.second);
                return Ref::create(ret);
            },
            [](const Dict::ObjectArr& a ) -> Sv {
                auto ret = Array::create(a.size());
                for ( const auto& r : a ) ret.push( dict2sv( &r ) );
                return Ref::create(ret);
            },
            [](std::string s) -> Sv {
                return Simple(s.c_str());
            },
            []( Undef ) -> Sv {
                return Sv::undef;
            },
            [](auto v) -> Sv {
                return Simple(v);
            },
    }, dict->value );
}

MODULE = CSGame::Dict      PACKAGE = CSGame::Dict
PROTOTYPES: DISABLE

Dict * new(SV* CLASS, panda::string filename){
    PROTO = CLASS;
    RETVAL = new Dict(filename);
}

void Dict::load_dict( panda::string filename )

void Dict::dump() : const

const Dict* Dict::get( std::vector<std::string> kk, uint64_t index = 0 ) : const {
    RETVAL = THIS->get( kk, index );
}

Sv Dict::get_value( std::vector<std::string> kk, uint64_t index = 0 ) : const {
    RETVAL = dict2sv(THIS->get( kk, index ));
}

Sv Dict::export() : const {
    RETVAL = dict2sv( THIS );
}
