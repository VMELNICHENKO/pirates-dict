#include <dict.hpp>
#include <xs/export.h>

#define MAX_ERR_SIZE   256

using namespace xs;

using panda::string;
using panda::string_view;

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
