#include <dict.hpp>
#include <xs/export.h>

#define MAX_ERR_SIZE   256

using namespace xs;

using panda::string;
using panda::string_view;

MODULE = Pirates::Dict      PACKAGE = Pirates::Dict
PROTOTYPES: DISABLE

PiratesDict * new(SV* CLASS, panda::string filename){
    PROTO = CLASS;
    RETVAL = new PiratesDict(filename);
}

void PiratesDict::load_dict( panda::string filename )

void PiratesDict::dump() : const

const PiratesDict* PiratesDict::get( std::vector<std::string> kk, uint64_t index = 0 ) : const {
    RETVAL = THIS->get( kk, index );
}
