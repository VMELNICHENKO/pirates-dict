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

    cout << "Object size: " << sizeof( this->value ) << endl;

    cout << "Doc size: " << sizeof( &doc ) << endl;

    delete doc;
}

void PiratesDict::process_node( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator ) {
    switch ( node->GetType() ) {
    case rapidjson::Type::kObjectType : {
        ObjectMap childs;
        for ( auto itr = node->MemberBegin(); itr != node->MemberEnd(); ++itr ) {
            string hkey = itr->name.GetString();

            rapidjson::Value val(rapidjson::Type::kObjectType);

            val.CopyFrom(itr->value, allocator);
            PiratesDict* child = new PiratesDict( &val, allocator);
            childs.insert( pair<string, PiratesDict*>(hkey, child) );
        }
        this->value = move(childs);
        break;
    }
    case rapidjson::Type::kArrayType : {
        vector<PiratesDict*> childs;
        for (auto itr = node->Begin(); itr != node->End(); ++itr) {
            PiratesDict* child = new PiratesDict( itr, allocator );
            childs.push_back( child );
        }

        this->value = move(childs);

        break;
    }
    case rapidjson::Type::kStringType : {
        this->value = node->GetString();
        break;
    }
    case rapidjson::Type::kNumberType : {
        this->value = node->IsDouble() ? node->GetDouble() : (int64_t)node->GetInt64();
        break;
    }
    case rapidjson::Type::kFalseType : {
        this->value = false;
        break;
    }
    case rapidjson::Type::kTrueType : {
        this->value = true;
        break;
    }
    case rapidjson::Type::kNullType : {
        break;
    }
    }

}

PiratesDict* PiratesDict::get(panda::string key){
    cout << "get_child: " << key << endl;

    // visit( overloaded{
    //         [level](const ObjectMap& m){
    //             string level_tab ( level, 9 );

    //             cout << "{" << endl;
    //             for ( auto const&[k,v] : m ) {
    //                 cout << "\t" << level_tab << k << " => " ;
    //                 v->dump( level + 1 );
    //             }
    //             cout << level_tab << "}" << endl;
    //         },
    //         [level](const ObjectArr& a){
    //             string level_tab ( level, 9 );

    //             cout << "[" << endl;
    //             for ( auto const& v : a ) {
    //                 cout << "\t" << level_tab;
    //                 v->dump( level + 1 );
    //             }
    //             cout << level_tab << "]" << endl;
    //         },
    //         [](string s){
    //             cout << "\"" << s << "\"" << endl;
    //         },
    //         [](auto v){
    //             cout << v << endl;
    //         },
    //         [](bool v){
    //             cout << ( v == true ? "true" : "false" ) << endl;
    //         }
    //     }, this->value );


    // switch ( this->type ) {
    // case ISOBJECT :
    //     ObjectMap obj = get<ObjectMap>(this->value);
    //     if ( obj.find(key) != obj.end() ) return obj.at(key);

    //     //        if ( this->value.find(key) != this->value.end() ) return this->value.at(key);
    //     break;
    // case ISARRAY :
    //     int index;
    //     try {
    //         index = stoi( key );
    //     } catch(std::invalid_argument& e){
    //         cerr << "I'm an array. You key conversion was failed by reason `invalid argument`" << endl;
    //         break;

    //     } catch(std::out_of_range& e){
    //         cerr << "I'm an array. You key conversion was failed by reason `out of range`" << endl;
    //         break;
    //     } catch(...) {
    //         cerr << "I'm an array. You key conversion was failed by unknown reason" << endl;
    //         break;
    //     }

    //     vector<PiratesDict*> obj = get<vector<PiratesDict*>>(this->value);
    //     if ( index < obj.size() ) {
    //         return obj[index];
    //     }
    // }
    return new PiratesDict();
}

void PiratesDict::dump( uint32_t level) {

    visit( overloaded{
            [level](const ObjectMap& m){
                string level_tab ( level, 9 );

                cout << "{" << endl;
                for ( auto const&[k,v] : m ) {
                    cout << "\t" << level_tab << k << " => " ;
                    v->dump( level + 1 );
                }
                cout << level_tab << "}" << endl;
            },
            [level](const ObjectArr& a){
                string level_tab ( level, 9 );

                cout << "[" << endl;
                for ( auto const& v : a ) {
                    cout << "\t" << level_tab;
                    v->dump( level + 1 );
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

PiratesDict::PiratesDict( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator ) {
    this->process_node( node, allocator );
}

PiratesDict::~PiratesDict(){
    // switch ( this->type ) {
    // case ISOBJECT :
    //     for ( auto const&[k,v] : this->value )  delete v;
    //     break;
    // case ISARRAY :
    //     for ( auto const& v : this->value ) delete v;
    //     break;
    // }
}
