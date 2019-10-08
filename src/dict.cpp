#include <iostream>
#include <fstream>
#include <sstream>
#include <dict.hpp>
#include <string>
#include <panda/string.h>
#include <panda/string_view.h>

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/reader.h>

#include <panda/excepted.h>

using namespace std;

void PiratesDict::load_dict( panda::string  filename ) {
    if ( filename.empty() ) return void();

    ifstream inFile(filename);
    if ( inFile.fail() ) {
        cout << "ERROR: cannot open file: " << filename << endl;
        return void();
    }

    stringstream strStream;
    strStream << inFile.rdbuf();
    string str = strStream.str();

    rapidjson::Document* doc = new rapidjson::Document();

    try {
        doc->Parse(str.c_str());
    } catch ( ... ) {
        cout << "Parsing error" << endl;
        return void();
    }
    rapidjson::Document::AllocatorType& a = doc->GetAllocator();

    if(doc->HasParseError()==true) {
         cout << "Parsing error" << endl;
         return void();
     } else {
         this->process_node(doc, a);
    }
    delete doc;
}

void PiratesDict::process_node( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator ) {
    switch ( node->GetType() ) {
    case rapidjson::Type::kObjectType :
        this->type = ISOBJECT;
        for ( auto itr = node->MemberBegin(); itr != node->MemberEnd(); ++itr ) {
            string hkey = itr->name.GetString();

            rapidjson::Value val(rapidjson::Type::kObjectType);

            val.CopyFrom(itr->value, allocator);
            PiratesDict* child = new PiratesDict( &val, allocator);
            this->childs.insert( pair<string, PiratesDict*>(hkey, child) );
        }
        break;

    case rapidjson::Type::kArrayType :
        this->type = ISARRAY;

        for (auto itr = node->Begin(); itr != node->End(); ++itr) {
            PiratesDict* child = new PiratesDict( itr, allocator );
            this->childs_arr.push_back(child);
        }

        break;

    case rapidjson::Type::kStringType :
        this->type = ISSTRING;
        //        this->Value = { node->GetString() };
        this->str_val = node->GetString();
        break;

    case rapidjson::Type::kNumberType :
        if ( node->IsDouble() ) {
            this->type = ISFLOAT;
            this->float_val = node->GetDouble();
        } else {
            this->type = ISINT;
            this->int_val = (int64_t)node->GetInt64();
        }
        break;

    case rapidjson::Type::kFalseType :
        this->type = ISBOOL;
        this->bool_val = false;
        break;

    case rapidjson::Type::kTrueType :
        this->type = ISBOOL;
        this->bool_val = true;
        break;

    case rapidjson::Type::kNullType :
        this->type = ISNILL;
        break;

    }

}

PiratesDict* PiratesDict::get(panda::string key){
    cout << "get_child: " << key << endl;
    switch ( this->type ) {
    case ISOBJECT :
        if ( this->childs.find(key) != this->childs.end() ) return this->childs.at(key);
        break;
    case ISARRAY :
        int index;
        try {
            index = stoi( key );
        } catch(std::invalid_argument& e){
            cerr << "I'm an array. You key conversion was failed by reason `invalid argument`" << endl;
            break;

        } catch(std::out_of_range& e){
            cerr << "I'm an array. You key conversion was failed by reason `out of range`" << endl;
            break;
        } catch(...) {
            cerr << "I'm an array. You key conversion was failed by unknown reason" << endl;
            break;
        }

        if ( index < this->childs_arr.size() ) {
            return this->childs_arr[index];
        }
    }
    return new PiratesDict();
}

void PiratesDict::dump( uint32_t level) {

    string level_tab ( level, 9 );

    switch ( this->type ) {
    case ISOBJECT :
        cout << "{" << endl;
        for ( auto const&[k,v] : this->childs ) {
            cout << "\t" << level_tab << k << " => " ;
            v->dump( level + 1 );
        }
        cout << level_tab << "}" << endl;
        break;
    case ISARRAY  :
        cout << "[" << endl;
        for ( auto const& v : this->childs_arr ) {
            cout << "\t" << level_tab;
            v->dump( level + 1 );
        }
        cout << level_tab << "]" << endl;
        break;
    case ISBOOL   :  cout << ( this->bool_val == true ? "true" : "false" ) << endl; break;
    case ISINT    :  cout << this->int_val                                 << endl; break;
    case ISFLOAT  :  cout << this->float_val                               << endl; break;
    case ISSTRING :  cout << "\"" << this->str_val << "\""                 << endl; break;
    }
}

PiratesDict::PiratesDict( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator ) {
    this->process_node( node, allocator );
}

PiratesDict::~PiratesDict(){
    switch ( this->type ) {
    case ISOBJECT :
        for ( auto const&[k,v] : this->childs )  delete v;
        break;
    case ISARRAY :
        for ( auto const& v : this->childs_arr ) delete v;
        break;
    }
}
