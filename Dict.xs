#include <dict.hpp>
#include <xs/export.h>
#include <xs/dict.h>
#include <panda/string.h>

using namespace json_tree;
using namespace xs;
using panda::string;

namespace json_tree {
Sv dict2sv (const Dict* dict) {
    if ( dict == nullptr ) return Sv::undef;
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
}

MODULE = CSGame::Dict      PACKAGE = CSGame::Dict
PROTOTYPES: DISABLE

Dict * new(SV* CLASS, std::string filename){
    PROTO = CLASS;
    RETVAL = new Dict(filename);
}

void Dict::load_dict( std::string filename )

void Dict::dump() : const

const Dict* Dict::get( ... ) : const {
    std::vector<panda::string> list;
    for ( int i = 1; i < items; ++i) {
        list.push_back( xs::in<panda::string>(ST(i)) );
    }
    RETVAL = THIS->get( list, 0 );
}

Sv Dict::get_value( ... ) : const {
    std::vector<panda::string> list;
    for ( int i = 1; i < items; ++i) {
        list.push_back( xs::in<panda::string>(ST(i)) );
    }
    RETVAL = dict2sv(THIS->get( list, 0 ));
}

Sv Dict::export() : const {
    RETVAL = dict2sv( THIS );
}
