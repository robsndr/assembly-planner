#pragma once

#include "tinyxml2.h"
#include "dotwriter.hpp"
#include "graph_factory.hpp"

// Read the input from the XML file 
struct InputReader
{
    InputReader();
    // Read the provided XML representing the assembly with agents, costs etc.
    std::tuple<Graph<AssemblyData,EdgeData>, config::Configuration, bool> read(std::string path);

  private:

    // Parse graph 
    int parse_graph(tinyxml2::XMLNode *);
    int parse_nodes(tinyxml2::XMLNode *);
    int parse_edges(tinyxml2::XMLNode *);
    // Parse node-associated data
    std::optional<std::unordered_map<std::string, double>> parse_costmap(tinyxml2::XMLNode *);
    std::optional<std::unordered_map<std::string, config::Reach>> parse_reachmap(tinyxml2::XMLNode *);
    std::optional<config::Action> parse_interaction(tinyxml2::XMLNode*);
    std::optional<std::unordered_map<std::string, config::Agent>> parse_agents(tinyxml2::XMLNode *);
    // Final validation
    int validate_config();

    // XML Root Element
    tinyxml2::XMLElement *root;

    // XML Document
    tinyxml2::XMLDocument doc;
    // Configuration 
    config::Configuration config;
    // Graph factory and constructed graph object
    GraphFactory graph_gen;
    Graph<AssemblyData,EdgeData> graph;
};

InputReader::InputReader()
    : graph(), graph_gen(&graph), config(), doc()
{}

// Top level read function
std::tuple<Graph<AssemblyData,EdgeData>, config::Configuration, bool> InputReader::read(std::string path)
{
    // Load XML file into buffer
    tinyxml2::XMLError result = doc.LoadFile(path.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "XML ERROR: Could not open XML file" << std::endl;
        return std::make_tuple(graph, config, false);
    }
    
    // Find the root node of the document
    root = doc.FirstChildElement("assembly");
    if (root == nullptr)
    {
        std::cerr << "XML ERROR: Could not find root element" << std::endl;
        return std::make_tuple(graph, config, false);
    }

    // Find and parse the top-level elements for the <subassemblies/> tree
    tinyxml2::XMLElement *agents_e = root->FirstChildElement("agents");
    if (agents_e == nullptr)
    {
        std::cerr << "XML ERROR: Could not find agents element" << std::endl;
        return std::make_tuple(graph, config, false);
    }
    auto a = parse_agents(agents_e);
    if (!a)
    {
        std::cerr << "XML ERROR: Error Parsing agents" << std::endl;
        return std::make_tuple(graph, config, false);
    }
    config.agents = a.value();

    // Find and parse elements corresponing to the <graph/> structure
    tinyxml2::XMLElement *graph_e = root->FirstChildElement("graph");
    if (graph_e == nullptr)
    {
        std::cerr << "XML ERROR: Could not find graph element" << std::endl;
        return std::make_tuple(graph, config, false);
    }
    if (parse_graph(graph_e) == tinyxml2::XML_ERROR_PARSING)
    {
        std::cerr << "XML ERROR: Error parsing graph" << std::endl;
        return std::make_tuple(graph, config, false);
    }
    // Read root attribute denoting the graph-root
    const char *attribute_text = nullptr;
    attribute_text = graph_e->Attribute("root");
    if (attribute_text == NULL)
        return std::make_tuple(graph, config, false);

    if (!graph_gen.setRoot(attribute_text))
        return std::make_tuple(graph, config, false);

    if(validate_config() != 0)
        return std::make_tuple(graph, config, false);

    return std::make_tuple(graph, config, true);
}

// Top-level graph reader, iterates over edges, nodes and associated data.
int InputReader::parse_graph(tinyxml2::XMLNode *graph_root){

    // Find and Parse <nodes/> tree.
    tinyxml2::XMLElement *nodes_e = graph_root->FirstChildElement("nodes");
    if (nodes_e == nullptr)
    {
        std::cerr << "XML ERROR: Could not find <nodes> element" << std::endl;
        return tinyxml2::XML_ERROR_PARSING;
    }
    if (parse_nodes(nodes_e) == tinyxml2::XML_ERROR_PARSING)
    {
        std::cerr << "XML ERROR: Could not parse nodes" << std::endl;
        return tinyxml2::XML_ERROR_PARSING;
    }

    // Find and Parse <edges/> tree.
    tinyxml2::XMLElement *edges_e = graph_root->FirstChildElement("edges");
    if (edges_e == nullptr)
    {
        std::cerr << "XML ERROR: Could not find <edges> element" << std::endl;
        return tinyxml2::XML_ERROR_PARSING;
    }
    if (parse_edges(edges_e) == tinyxml2::XML_ERROR_PARSING)
    {
        std::cerr << "XML ERROR: Could not parse edges" << std::endl;
        return tinyxml2::XML_ERROR_PARSING;
    }

    return tinyxml2::XML_SUCCESS;
}

// Node element parser, called by the top-level graph parser.
int InputReader::parse_nodes(tinyxml2::XMLNode *nodes_root)
{
    const char *attribute_text = nullptr;

    for (tinyxml2::XMLElement *child = nodes_root->FirstChildElement("node");
         child != nullptr; child = child->NextSiblingElement("node"))
    {
        attribute_text = child->Attribute("name");
        if (attribute_text == NULL){
            std::cerr << "XML ERROR: Can't read [name] attribute of <node>" << std::endl;
            return tinyxml2::XML_ERROR_PARSING;
        }
        std::string node_name = attribute_text;

        attribute_text = child->Attribute("type");
        if (attribute_text == NULL){
            std::cerr << "XML ERROR: Can't read [type] attribute of <node>" << std::endl;
            return tinyxml2::XML_ERROR_PARSING;
        }
        std::string node_type = attribute_text;
        
        // Node is a subassembly (OR)
        if (node_type == "OR"){
            graph_gen.insertOr(node_name);
            
            auto r = parse_reachmap(child);
            if(!r.has_value())
                return tinyxml2::XML_ERROR_PARSING;
            config.subassemblies[node_name].name = node_name;
            config.subassemblies[node_name].reachability = r.value();
        }
        // Node is a action (AND)
        else if (node_type == "AND"){
            graph_gen.insertAnd(node_name);
            config::Action action_temp;
            action_temp.name = node_name;
            
            auto c = parse_costmap(child);
            if(!c.has_value())
                return tinyxml2::XML_ERROR_PARSING;

            action_temp.costs = c.value();
            config.actions[node_name] = action_temp;
        }
        else{
            return tinyxml2::XML_ERROR_PARSING;
        }
    }

    return tinyxml2::XML_SUCCESS;
}

// Edge parsing helper utilized by the graph reading function. 
int InputReader::parse_edges(tinyxml2::XMLNode *edges_root)
{
    const char *attribute_text = nullptr;

    for (tinyxml2::XMLElement *child = edges_root->FirstChildElement("edge");
         child != nullptr; child = child->NextSiblingElement("edge"))
    {
        attribute_text = child->Attribute("start");
        if (attribute_text == NULL){
            std::cerr << "Can't read *start* attribute of edge." << std::endl;
            return tinyxml2::XML_ERROR_PARSING;
        }
        std::string start_node = attribute_text;

        attribute_text = child->Attribute("end");
        if (attribute_text == NULL){
            std::cerr << "Can't read *end* attribute of edge." << std::endl;
            return tinyxml2::XML_ERROR_PARSING;
        }
        std::string end_node = attribute_text;

        bool inserted = graph_gen.insertEdge(start_node, end_node);
    }

    return tinyxml2::XML_SUCCESS;
}

// Parsing of reachability information, that is associated with nodes
std::optional<std::unordered_map<std::string, config::Reach>>
     InputReader::parse_reachmap(tinyxml2::XMLNode *reachmap_root)
{
    std::unordered_map<std::string, config::Reach> reach_map;

    const char *attribute_text = nullptr;
    
    for (tinyxml2::XMLElement *reach = reachmap_root->FirstChildElement("reach");
            reach != nullptr; reach = reach->NextSiblingElement("reach"))
    {
        attribute_text = reach->Attribute("agent");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [agent] attribute of <reach>" << std::endl;
            return std::nullopt;
        }
        std::string agent_name = attribute_text;

        attribute_text = reach->Attribute("reachable");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [reachable] attribute of <action>" << std::endl;
            return std::nullopt;
        }
        std::string agent_part_reach = attribute_text;

        std::transform(agent_part_reach.begin(), agent_part_reach.end(), agent_part_reach.begin(),
                        [](unsigned char c) { return std::tolower(c); });

        config::Action interaction_temp;
        
        if (agent_part_reach == "false")
        {
            reach_map[agent_name].reachable = false; 

            auto i = parse_interaction(reach);
            if(!i.has_value())
                return std::nullopt;
            
            interaction_temp = i.value();
            config.actions[interaction_temp.name] = interaction_temp;

        }
        else if (agent_part_reach == "true")
        {
            reach_map[agent_name].reachable = true;
            interaction_temp.name = "-";
        }
        else
        {
            std::cerr << "XML ERROR: Only True/False [value] is supported for <reach> node" << std::endl;
            return std::nullopt;
        }

        reach_map[agent_name].interaction = interaction_temp;
    }
    return reach_map;
}

// Parse interactions that might optionally 
std::optional<config::Action> InputReader::parse_interaction(tinyxml2::XMLNode* interaction_root){

    const char *attribute_text = nullptr;

    config::Action interaction;

    tinyxml2::XMLElement *interaction_node = interaction_root->FirstChildElement("interaction");
    if (interaction_node == nullptr){
        std::cerr << "XML ERROR: <interaction> node is missing for non-reachable subassembly" << std::endl;
        return std::nullopt;
    }
    
    attribute_text = interaction_node->Attribute("name");
    if (attribute_text == nullptr){
        std::cerr << "XML ERROR: Can't read [name] attribute of <interaction>" << std::endl;
        return std::nullopt;
    }

    auto c = parse_costmap(interaction_node);
    if(!c.has_value())
        return std::nullopt;
    interaction.costs = c.value();
    interaction.name = attribute_text;
    
    return interaction;
}

// Helper for reading costmaps, which are associated with actions and interactions. 
std::optional<std::unordered_map<std::string, double>>
                    InputReader::parse_costmap(tinyxml2::XMLNode *action_node)
{
    std::unordered_map<std::string, double> costmap;
    
    const char *attribute_text = nullptr;

    for (tinyxml2::XMLElement *cost = action_node->FirstChildElement("cost");
             cost != nullptr; cost = cost->NextSiblingElement("cost"))
    {
        attribute_text = cost->Attribute("agent");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [agent] attribute of <cost>" << std::endl;
            return std::nullopt;
        }
        std::string agent_name = attribute_text;

        attribute_text = cost->Attribute("value");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [value] attribute of <cost>" << std::endl;
            return std::nullopt;
        }
        std::string cost_value = attribute_text;

        std::transform(cost_value.begin(), cost_value.end(), cost_value.begin(),
                        [](unsigned char c) { return std::tolower(c); });

        if (cost_value == "inf")
        {
            costmap[agent_name] = INT_MAX;
        }
        else if (is_float(cost_value))
        {
            costmap[agent_name] = std::stod(cost_value);
        }
        else
        {
            std::cerr << "XML ERROR: [cost] must be a number or 'inf'" << std::endl;
            return std::nullopt;
        }
    }
    return costmap;
}

// Read information about the agents (human, robot) that are considers in the planning process.
std::optional<std::unordered_map<std::string, config::Agent>> 
                    InputReader::parse_agents(tinyxml2::XMLNode *agents_root){
    
    std::unordered_map<std::string, config::Agent> agent_map;

    const char *attribute_text = nullptr;

    for (tinyxml2::XMLElement *agent = agents_root->FirstChildElement("agent");
            agent != nullptr; agent = agent->NextSiblingElement("agent"))
    {
        attribute_text = agent->Attribute("name");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [name] attribute of <agent>" << std::endl;
            return std::nullopt;
        }
        std::string agent_name = attribute_text;

        attribute_text = agent->Attribute("host");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [host] attribute of <agent>" << std::endl;
            return std::nullopt;
        }
        std::string host = attribute_text;

        attribute_text = agent->Attribute("port");
        if (attribute_text == nullptr){
            std::cerr << "XML ERROR: Can't read [port] attribute of <agent>" << std::endl;
            return std::nullopt;
        }
        std::string port = attribute_text;

        config::Agent agent_temp;
        agent_temp.name = agent_name;
        agent_temp.hostname = host;
        agent_temp.port = port;

        agent_map[agent_name] = agent_temp;
    }

    return agent_map;
}

int InputReader::validate_config()
{
    if(config.agents.empty())
    {
        std::cerr << "ERROR: no agents provided!" << std::endl;
        return -1;
    }

    for(const auto& sa : config.subassemblies)
    {
        for(const auto& agent : config.agents)
        {
            if(sa.second.reachability.find(agent.second.name) 
                                        == sa.second.reachability.end())
            {
                std::cerr << "ERROR: Agent '" << agent.second.name 
                    << "' reach is missing in reachability map of node '" 
                        << sa.second.name << "'" << std::endl;
                return -1;
            }
        }
    }

    for(const auto& action : config.actions)
    {
        for(const auto& agent : config.agents)
        {
            if(action.second.costs.find(agent.second.name) 
                                    == action.second.costs.end())
            {
                std::cerr << "ERROR: Cost of '" << action.second.name 
                    << "' for agent '" << agent.second.name 
                        << "' is missing" << std::endl;
                return -1;
            }
        }
    }

    return 0;
}