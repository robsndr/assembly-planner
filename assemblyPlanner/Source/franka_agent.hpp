#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "task.hpp"
#include "graph.hpp"
#include "websocket_client.hpp"
#include <unistd.h>

class FrankaAgent
{
public:
    FrankaAgent(Graph<> *);
    ~FrankaAgent();
    bool exec(std::vector< std::vector< Task*>> &);
private:
    WebsocketEndpoint endpoint;
    int connection_id;
    std::vector<Node*> available_leaves;
};

FrankaAgent::FrankaAgent(Graph<> * ao_graph){ 
        std::cout<< "Franka Agent." << std::endl;
        std::string uri = "ws://localhost:9000";
        connection_id = endpoint.connect(uri);
        if (connection_id != -1) {
            std::cout << "> Created connection with id " << connection_id << std::endl;
        }
       
        ConnectionMetadata::ptr metadata = endpoint.get_metadata(connection_id);
        while (metadata->get_status() != "Open"){
            usleep(100000);  // Sleep for 500k microseconds == 0.1s.
        }       
}

FrankaAgent::~FrankaAgent(){ }

bool FrankaAgent::exec(std::vector< std::vector< Task*>> & plan){

    for(auto step : plan)
    {
        for(auto var: step)
        {
            std::cout << *var;            
        }
        std::cout << std::endl;
    }

    std::string input;
    std::cout << "Enter Command: ";
    std::getline(std::cin, input);
    if (input.substr(0,4) == "send") {
        endpoint.send(connection_id, "Hello");
    }
        
    // Close the open connection. Wait until thread completes. 
    endpoint.close(connection_id, websocketpp::close::status::normal, ""); 
    ConnectionMetadata::ptr metadata = endpoint.get_metadata(connection_id);
    while (metadata->get_status() != "Closed"){
        usleep(100000);  
    }

    return true;
}



bool FrankaAgent::callFranka(hostname, method, payload=None, port=9000, endpoint="mios/core", timeout=100, silent=False):
    try:
        request = {
            "method": method,
            "request": payload
        }
        asyncio.set_event_loop(asyncio.new_event_loop())
        return asyncio.get_event_loop().run_until_complete(send(hostname, request=request, port=port,
                                                                endpoint=endpoint, timeout=timeout, silent=silent))
    except socket.gaierror as e:
        print(e)
        print("Hostname: " + hostname + ", port:" + str(port) + ", endpoint: " + endpoint)
        return None