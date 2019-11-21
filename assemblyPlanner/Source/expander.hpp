#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <vector>
#include "combinator.hpp"
#include "graph.hpp"
#include <set>

class NodeExpander{

public:
    NodeExpander(Graph<>*, CostMap&);
    ~NodeExpander();

    void expandNode(Node*);

private:

    Graph<> * search_graph_;
    CostMap cost_map_;
    Combinator * assignment_generator;
    std::vector< std::vector< std::tuple<std::string, std::string, Node*> > > * assignments_;

    std::size_t iteration = 0;
};


NodeExpander::NodeExpander(Graph<>* graph, CostMap & costs){
    search_graph_ = graph;
    cost_map_ = costs;
    assignment_generator = new Combinator(costs);
}

NodeExpander::~NodeExpander(){
    delete assignment_generator;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNode(Node* node){
    // std::cout << "EXPAND " << node->data_.name;
    // std::cout << "Subassembly Size " << node->data_.subassemblies.size(); 

    // std::cin.get();
    std::vector<Node*> nodes;
    nodes.reserve(node->data_.subassemblies.size());
    for(auto nd : node->data_.subassemblies) {
        // std::cout << "Current subass:  " << nd.second->data_.name;
        if(nd.second->hasSuccessor())
            nodes.push_back(nd.second);  
    } 

    assignments_ = assignment_generator->generateAgentActionAssignments(nodes);

    // Node * inserted = search_tree_->insertNode(*root);
    // new_root->data_.name = "";
    // new_root->data_.subassemblies.push_back(root);
    // for (auto &i : search_stack){
    //     std::cout << "Stack: " << i->data_.name << std::endl;
    // }
    
    for (auto &cur_assignments : *assignments_){

        NodeData ndata;
        ndata.subassemblies = node->data_.subassemblies;
        ndata.actions       = node->data_.actions;
        ndata.marked = false;

        EdgeData edata;
        edata.cost = 0;

        double min_action_agent_cost_ = INT_MAX;

        for (auto &agent_action_assignment : cur_assignments){   

            std::string agent  = std::get<0>(agent_action_assignment);
            std::string action = std::get<1>(agent_action_assignment);
            Node * action_ptr  = std::get<2>(agent_action_assignment);
            
            std::string action_source = action_ptr->getPredecessorNodes()[0]->data_.name;
            std::string action_dest   = action_ptr->getSuccessorNodes()[0]->data_.name;

            ndata.name += action_source + "-" + action + "-" + agent + "     ";
            ndata.subassemblies.erase(action_source);
            ndata.actions.erase(action);

            for (auto & or_successor : action_ptr->getSuccessorNodes()){
                // std::cout << "  OR_NEXT: " << or_successor << std::endl;
                ndata.subassemblies[or_successor->data_.name] = or_successor;
                for (auto &following_action : or_successor->getSuccessorNodes()){
                    ndata.actions[following_action->data_.name] = following_action; 
                }
            }

            if(cost_map_.map_[action][agent] < min_action_agent_cost_){
                min_action_agent_cost_ = cost_map_.map_[action][agent];
            }
            edata.cost   += cost_map_.map_[action][agent];
            // std::cout << "Action: " << action << "   Agent: " << agent << std::endl;
            // std::cout << "Edge Cest: " << cost_map_.map_[action][agent] << "  "<< std::endl;
            edata.agent_actions_.push_back(std::make_pair(action, agent));

        }

        ndata.minimum_cost_action = min_action_agent_cost_;

        Node * next_node = search_graph_->insertNode(ndata);

        search_graph_->insertEdge(edata, node->id_, next_node->id_);
        
        // DotWriter writer("ABC.dot");
        // search_graph_->print(writer);
        // std::cin.get();

    }

}


#endif