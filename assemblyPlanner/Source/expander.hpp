#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <vector>
#include "combinator.hpp"
#include "graph.hpp"
#include <set>
#include "helpers.hpp"

class NodeExpander{

public:
    NodeExpander(CostMap&);
    ~NodeExpander();

    void expandNode(Node*);

private:
    void createAssignmentNodes(std::vector<Node*> &);

    Graph<> * search_tree_;
    CostMap cost_map_;
    Combinator * assignment_generator;
    std::vector< std::vector< std::tuple<std::string, std::string, Node*> > > * assignments_;

    std::size_t iteration = 0;
};


NodeExpander::NodeExpander(CostMap & costs){
    cost_map_ = costs;
    assignment_generator = new Combinator(costs);
}

NodeExpander::~NodeExpander(){
    delete assignment_generator;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNode(Node* node){

    std::cout << "EXPAND " << node->data_.name;
    assignments_ = assignment_generator->generateAgentActionAssignments(node->data_.subassemblies);

    // Node * inserted = search_tree_->insertNode(*root);
    // new_root->data_.name = "";
    // new_root->data_.subassemblies.push_back(root);
    // for (auto &i : search_stack){
    //     std::cout << "Stack: " << i->data_.name << std::endl;
    // }
    
    for (auto &cur_assignments : *assignments_){

        for (auto &agent_action_assignment : cur_assignments){   

            std::string agent  = std::get<0>(agent_action_assignment);
            std::string action = std::get<1>(agent_action_assignment);
            Node * action_ptr  = std::get<2>(agent_action_assignment);
            
            // nodes_to_delete_.insert(action_ptr);
        
            // Node * next_node = temp.insertNode(ndata);
            // search_tree_->insertEdge(0, node->id_, next_node->id_);

            for (auto & or_successor : action_ptr->getSuccessorNodes()){

            }
        }
    }

    // createAssignmentNodes(nodes);       

}

void NodeExpander::createAssignmentNodes(std::vector<Node*> & or_nodes){

}


#endif