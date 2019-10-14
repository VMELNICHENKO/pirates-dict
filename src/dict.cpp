#include <iostream>
#include <fstream>
#include <sstream>
#include <dict.hpp>
#include <string>
#include <charconv>
#include <panda/string.h>
#include <panda/string_view.h>

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/reader.h>

#include <panda/excepted.h>

using namespace std;

void Dict::load_dict( panda::string  filename ) {
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

void Dict::process_node( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator ) {
    switch ( node->GetType() ) {
    case rapidjson::Type::kObjectType : {
        ObjectMap childs;
        for ( auto itr = node->MemberBegin(); itr != node->MemberEnd(); ++itr ) {
            string hkey = itr->name.GetString();

            rapidjson::Value val(rapidjson::Type::kObjectType);

            val.CopyFrom(itr->value, allocator);
            childs.insert( pair<string, Dict>(hkey, Dict( &val, allocator)) );
        }
        this->value = move(childs);
        break;
    }
    case rapidjson::Type::kArrayType : {
        vector<Dict> childs;
        for (auto itr = node->Begin(); itr != node->End(); ++itr) {
            childs.push_back( Dict( itr, allocator ) );
        }

        this->value = move(childs);
        break;
    }
    case rapidjson::Type::kNumberType : {
        this->value = node->IsDouble() ? node->GetDouble() : (int64_t)node->GetInt64();
        break;
    }
    case rapidjson::Type::kStringType : { this->value = node->GetString(); break; }
    case rapidjson::Type::kFalseType  : { this->value = false; break; }
    case rapidjson::Type::kTrueType   : { this->value = true ; break; }
    case rapidjson::Type::kNullType   : { break; }
    }

}

const Dict* Dict::get(const vector<string>& keys, uint64_t index ) const {

    if ( index >= keys.size() ) return this;

    string key = keys.at(index);

    cout << "get_child: " << key << endl;
    return visit( overloaded{
            [=](const ObjectMap& m) -> const Dict* {
                auto i = m.find(key);
                if ( i == m.end() ) return nullptr;
                return i->second.get( keys, index + 1 );
            },
            [=](const ObjectArr& a) -> const Dict* {
                uint64_t i;
                if ( auto [p, ec] = std::from_chars(key.data(), key.data()+key.size(), i); ec == errc() ) {
                    if ( i < a.size() ) {
                        return a[i].get( keys, index + 1 );
                    }
                }
                return nullptr;
            },
            [=](auto v) -> const Dict* {
                return this;
            }
        }, this->value );
}

void Dict::dump( uint32_t level) const {
    visit( overloaded{
            [level](const ObjectMap& m) {
                string level_tab ( level, 9 );

                cout << "{" << endl;
                for ( auto const&[k,v] : m ) {
                    cout << "\t" << level_tab << k << " => " ;
                    v.dump( level + 1 );
                }
                cout << level_tab << "}" << endl;
            },
            [level](const ObjectArr& a) {
                string level_tab ( level, 9 );

                cout << "[" << endl;
                for ( auto const& v : a ) {
                    cout << "\t" << level_tab;
                    v.dump( level + 1 );
                }
                cout << level_tab << "]" << endl;
            },
            [](string s){
                cout << "\"" << s << "\"" << endl;
            },
            [](auto v){
                cout << v << endl;
            },
            [](bool v){
                cout << ( v == true ? "true" : "false" ) << endl;
            }
        }, this->value );
}

Dict::Dict( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator ) {
    this->process_node( node, allocator );
}
