#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unistd.h>

#include "task.hpp"
#include "graph.hpp"
#include "websocket_client.hpp"
#include "nlohmann/json.hpp"


class ExecAgent
{
public:
    ExecAgent(WebsocketEndpoint*, std::string);
    ~ExecAgent();
    bool exec(std::string);
    bool sendToWebsocket(std::string, nlohmann::json);

private:
    WebsocketEndpoint* endpoint_;
    int connection_id;
};

ExecAgent::ExecAgent(WebsocketEndpoint* endpoint, std::string host){ 
        std::cout<< "Franka Agent." << std::endl;
        std::string uri = "ws://" + host;
        endpoint_ = endpoint;
        connection_id = endpoint_->connect(uri);
        if (connection_id != -1) {
            std::cout << "> Created connection with id " << connection_id << std::endl;
        }
       
        ConnectionMetadata::ptr metadata = endpoint_->get_metadata(connection_id);
        while (metadata->get_status() != "Open"){
            usleep(100000);  // Sleep for 500k microseconds == 0.1s.
        }       
}

ExecAgent::~ExecAgent(){
    // Close the open connection. Wait until thread completes. 
    endpoint_->close(connection_id, websocketpp::close::status::normal, ""); 
    ConnectionMetadata::ptr metadata = endpoint_->get_metadata(connection_id);
    while (metadata->get_status() != "Closed"){
        usleep(100000);  
    }
}

bool ExecAgent::exec(std::string task_name){

    nlohmann::json payload;
    payload["task"] = "home_gripper";

    sendToWebsocket("start_task", payload);
    return true;
}


bool ExecAgent::sendToWebsocket(std::string method, nlohmann::json payload){

    nlohmann::json temp;
    temp["method"] = method;
    temp["request"] = payload;
    std::string message = temp.dump();
    endpoint_->send(connection_id, message);
    return true;
}

// bool ExecAgent::callFranka(hostname, method, payload=None, port=9000, endpoint="mios/core", timeout=100, silent=False):
//     try:
//         request = {
//             "method": method,
//             "request": payload
//         }
//         asyncio.set_event_loop(asyncio.new_event_loop())
//         return asyncio.get_event_loop().run_until_complete(send(hostname, request=request, port=port,
//                                                                 endpoint=endpoint, timeout=timeout, silent=silent))
//     except socket.gaierror as e:
//         print(e)
//         print("Hostname: " + hostname + ", port:" + str(port) + ", endpoint: " + endpoint)
//         return None