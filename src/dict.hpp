#pragma once
#include <map>
#include <vector>
#include <string>
#include <variant>
#include <charconv>

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include <panda/string.h>
#include <panda/string_view.h>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace json_tree {
    struct Undef {};

    struct Dict {
        Dict(){};
        Dict( std::string filename ) { load_dict( filename ); }
        Dict( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator );
        ~Dict(){};

        void load_dict( std::string filename );
        void process_node( rapidjson::Value* node, rapidjson::Document::AllocatorType& allocator );

        template<typename Range>
        const Dict* get( const Range& keys, uint64_t index ) const {
            if ( index >= keys.size() ) return this;

            return visit( overloaded{
                    [&](const ObjectMap& m) -> const Dict* {
                        auto i = m.find(keys[index]);
                        if ( i == m.end() ) return nullptr;
                        return i->second.get( keys, index + 1 );
                    },
                    [&](const ObjectArr& a) -> const Dict* {
                        panda::string_view key = keys[index];
                        uint64_t i;
                        if ( auto [p, ec] = std::from_chars(key.data(), key.data()+key.size(), i); ec == std::errc() ) {
                            if ( i < a.size() ) {
                                return a[i].get( keys, index + 1 );
                            }
                        }
                        return nullptr;
                    },
                        [](auto v) -> const Dict* {
                            return nullptr;
                        }
                }, this->value );
        }

        void dump( uint32_t level = 0) const;

        using ObjectMap = std::map<std::string, Dict>;
        using ObjectArr = std::vector<Dict>;

        std::variant<Undef,ObjectMap, ObjectArr, std::string, int64_t, double, bool> value;
    };
}
