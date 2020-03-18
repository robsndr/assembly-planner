#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/value.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

int main(int argc, char *argv[])
{
    
    mongocxx::instance inst{};
    mongocxx::client db_handle = mongocxx::client{mongocxx::uri{"mongodb://collective-panda-012:27017"}};
    mongocxx::database db = db_handle["mios"];
    mongocxx::collection coll = db["environment"];
    
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = coll.find_one(document{} << "name" << "test_object_1" << finalize);
    if(maybe_result) {
        bsoncxx::document::view docview = maybe_result->view();
        bsoncxx::document::element store = docview["name"];
        if (store) {
            // this block will only execute if "store" was found in the document
            // bsoncxx::types::value ele_val{store.get_value()};
            std::cout << "Examining inventory at " << store.get_utf8().value<< std::endl;
        }
        // std::cout << bsoncxx::to_json(store.get_value()) << "\n";
    }
    return 0;
}