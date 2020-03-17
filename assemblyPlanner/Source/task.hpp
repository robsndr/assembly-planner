#pragma once

#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

#include <string>

class Task
{

private:
    std::string action_;
    std::string agent_; 
    double cost_;
    bool finished;

public:
    Task(std::string, std::string, double);
    ~Task();
    bool exec();
    void getInfoFromDatabase(mongocxx::client &);
};

Task::Task(std::string action, std::string agent, double cost)
{
    agent_ = agent;
    action_ = action;
    cost_ = cost;
    finished = false;
}

bool Task::exec(){
    return true;
}

void Task::getInfoFromDatabase(mongocxx::client & db_handle){
    // mongocxx::instance inst{};
    // mongocxx::client conn{mongocxx::uri{"mongodb://localhost:27017"}};

    // bsoncxx::builder::stream::document document{};

    auto collection = db_handle["mios"]["environment"];

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = collection.find_one(document{} << "name" << "test_object_1" << finalize);
    if(maybe_result) {
        std::cout << bsoncxx::to_json(*maybe_result) << "\n";
    }
}

Task::~Task()
{
}
