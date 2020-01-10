#pragma once

#include <string>
#include <exception>
#include <tuple>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "dotwriter.hpp"
#include "tinyxml2.hpp"
#include "graph_generator.hpp"
#include "containers.hpp"

bool is_float( std::string my_string ) {
    std::istringstream iss(my_string);
    float f;
    iss >> std::noskipws >> f; 
    return iss.eof() && !iss.fail(); 
}

class InputReader{
    
    public:
        std::tuple<Graph<>*,Config*,bool> read(std::string);

        InputReader(std::string);
        ~InputReader();
    
    private:
        int parse_nodes(tinyxml2::XMLNode*);
        int parse_edges(tinyxml2::XMLNode*);
        int parse_costmap(tinyxml2::XMLNode*);
        int parse_reachmap(tinyxml2::XMLNode*);

        tinyxml2::XMLElement* root;
        tinyxml2::XMLDocument* doc;
        GraphGenerator* graph_gen;
        Graph<>* graph;
        Config * config;
};

InputReader::InputReader(std::string path){
    doc = new tinyxml2::XMLDocument;
    tinyxml2::XMLError result = doc->LoadFile(path.c_str());
    if (result != tinyxml2::XML_SUCCESS) 
        throw std::runtime_error("Could not open XML file.");

    graph          = new Graph;
    graph_gen      = new GraphGenerator(graph);
    config         = new Config;
    config->costs_ = new CostMap;
    config->reach_ = new ReachMap;
}

InputReader::~InputReader(){
    delete graph;
    delete config->costs_;
    delete config->reach_;
    delete config;
    delete graph_gen;
}

std::tuple<Graph<>*,Config*,bool> InputReader::read(std::string root_name){

    root = doc->FirstChildElement(root_name.c_str());
    if (root == nullptr){
        std::cerr << "XML: Could not find root element." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    tinyxml2::XMLElement * nodes_e = root->FirstChildElement("nodes");
    if (nodes_e == nullptr){
        std::cerr << "XML: Could not find nodes element." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    tinyxml2::XMLElement * edges_e = root->FirstChildElement("edges");
    if (edges_e == nullptr){
        std::cerr << "XML: Could not find edges element." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    tinyxml2::XMLElement * costmap_e = root->FirstChildElement("costmap");
    if (costmap_e == nullptr){
        std::cerr << "XML: Could not find costmap element." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    tinyxml2::XMLElement * reachmap_e = root->FirstChildElement("reachabilitymap");
    if (costmap_e == nullptr){
        std::cerr << "XML: Could not find reachabilitymap element." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }


    if(parse_nodes(nodes_e) == tinyxml2::XML_ERROR_PARSING){
        std::cerr << "XML: Error Parsing Nodes." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    if(parse_edges(edges_e) == tinyxml2::XML_ERROR_PARSING){
        std::cerr << "XML: Error Parsing Edges." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    if(parse_costmap(costmap_e) == tinyxml2::XML_ERROR_PARSING){
        std::cerr << "XML: Error Parsing CostMap." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    if(parse_reachmap(reachmap_e) == tinyxml2::XML_ERROR_PARSING){
        std::cerr << "XML: Error Parsing ReachMap." << std::endl;
        return std::make_tuple(nullptr, nullptr, false);
    }

    graph_gen->setRoot(root->Attribute("root"));
    return std::make_tuple(graph_gen->graph_, config, true);
}

int InputReader::parse_nodes(tinyxml2::XMLNode* nodes_root){

    const char * attribute_text = nullptr;

    for (tinyxml2::XMLElement* child = nodes_root->FirstChildElement("node"); 
                        child != nullptr; child = child->NextSiblingElement("node")){
                        
        attribute_text = child->Attribute("name");
        std::string node_name = attribute_text;
        // if (attribute_text == NULL) 
            // return tinyxml2::XML_ERROR_PARSING;

        attribute_text = child->Attribute("type");
        std::string node_type = attribute_text;
        // if (attribute_text == NULL) 
            // return tinyxml2::XML_ERROR_PARSING;
        // std::cout << node_name << " " << node_type << std::endl;

        if(node_type == "OR")
            graph_gen->insertOr(node_name);
        else if(node_type == "AND")
            graph_gen->insertAnd(node_name);
        else
            return tinyxml2::XML_ERROR_PARSING;
    }

    return tinyxml2::XML_SUCCESS;
}


int InputReader::parse_edges(tinyxml2::XMLNode* edges_root){

    const char * attribute_text = nullptr;

    for (tinyxml2::XMLElement* child = edges_root->FirstChildElement("edge"); 
                        child != nullptr; child = child->NextSiblingElement("edge")){
                        
        attribute_text = child->Attribute("start");
        std::string start_node = attribute_text;
        // if (attribute_text == NULL) 
            // return tinyxml2::XML_ERROR_PARSING;

        attribute_text = child->Attribute("end");
        std::string end_node = attribute_text;
        // if (attribute_text == NULL) 
            // return tinyxml2::XML_ERROR_PARSING;
        // std::cout << start_node << " " << end_node << std::endl;

        bool inserted = graph_gen->insertEdge(start_node, end_node);
    }

    DotWriter dot("parsed.dot");
    graph_gen->graph_->print(dot);

    return tinyxml2::XML_SUCCESS;
}

int InputReader::parse_reachmap(tinyxml2::XMLNode* reachmap_root){

    const char * attribute_text = nullptr;

    for (tinyxml2::XMLElement* part = reachmap_root->FirstChildElement("part"); 
                        part != nullptr; part = part->NextSiblingElement("part")){

        std::string part_name = part->Attribute("name");
        
        for (tinyxml2::XMLElement* agent = part->FirstChildElement("agent"); 
                        agent != nullptr; agent = agent->NextSiblingElement("agent")){                  
            
            std::string agent_name = agent->Attribute("name");
            
            std::string agent_part_reach = agent->Attribute("reachability");
            std::transform(agent_part_reach.begin(), agent_part_reach.end(), agent_part_reach.begin(),
                [](unsigned char c){ return std::tolower(c); });

            if(agent_part_reach == "true"){
                config->reach_->addMapping(part_name, agent_name, true);
            }
            else if(agent_part_reach == "false"){
                config->reach_->addMapping(part_name, agent_name, false);
            }
            else{
                std::cerr << "XML: Wrong value for cost."
                          << "  Agent: "  << agent_name 
                          << "  Part: " << part_name << std::endl;
                
                return tinyxml2::XML_ERROR_PARSING;
            }

        }
    }
    return tinyxml2::XML_SUCCESS;
}

int InputReader::parse_costmap(tinyxml2::XMLNode* costmap_root){

    const char * attribute_text = nullptr;

    for (tinyxml2::XMLElement* action = costmap_root->FirstChildElement("action"); 
                        action != nullptr; action = action->NextSiblingElement("action")){

        attribute_text = action->Attribute("name");
        // if (attribute_text == NULL) 
            // return tinyxml2::XML_ERROR_PARSING;
        std::string action_name = attribute_text;
        // std::cout     << "  Agent: ";
        
        for (tinyxml2::XMLElement* agent = action->FirstChildElement("agent"); 
                        agent != nullptr; agent = agent->NextSiblingElement("agent")){                  
            

            attribute_text = agent->Attribute("name");
            // if (attribute_text == NULL) 
                // return tinyxml2::XML_ERROR_PARSING;
            std::string agent_name = attribute_text;

            attribute_text = agent->Attribute("cost");
            // if (attribute_text == NULL) 
                // return tinyxml2::XML_ERROR_PARSING;
            std::string agent_action_cost = attribute_text;
            std::transform(agent_action_cost.begin(), agent_action_cost.end(), agent_action_cost.begin(),
                [](unsigned char c){ return std::tolower(c); });

            // std::cout     << "  Agent: "  << agent_name 
                        //   << "  Action: " << action_name << std::endl;

            if(agent_action_cost == "inf"){
                config->costs_->addMapping(action_name, agent_name, INT_MAX);
            }
            else if(is_float(agent_action_cost)){
                config->costs_->addMapping(action_name, agent_name, std::stod(agent_action_cost));
            }
            else{
                std::cerr << "XML: Wrong value for cost."
                          << "  Agent: "  << agent_name 
                          << "  Action: " << action_name << std::endl;
                
                return tinyxml2::XML_ERROR_PARSING;
            }

        }
    }

    DotWriter dot("parsed.dot");
    graph_gen->graph_->print(dot);

    return tinyxml2::XML_SUCCESS;
}
