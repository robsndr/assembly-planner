#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <vector>
#include "combinator.hpp"
#include "graph.hpp"
#include <set>
#include "bfs.hpp"

class NodeExpander{

public:
    NodeExpander(Graph<> *, CostMap&);
    ~NodeExpander();

    void expandNodes(std::vector<Node*> &, std::vector<Node*> &);

private:
    void createAssignmentNodes(std::vector<Node*> &, std::vector<Node*> &);

    std::vector< std::vector< std::tuple<std::string, std::string, Node*> > > * assignments_;
    Graph<> * search_tree_;
    Combinator * assignment_generator;

    std::size_t iteration = 0;
};


NodeExpander::NodeExpander(Graph<> * tree, CostMap & costs){
    search_tree_ = tree;
    assignment_generator = new Combinator(costs);
}

NodeExpander::~NodeExpander(){
    delete assignment_generator;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNodes(std::vector<Node*> & nodes, std::vector<Node*> & search_stack){
    // Expand multiple nodes at once.
    if(nodes.size() > 0)
    // std::cout << "NodeSize: " << nodes.size() << std::endl;

    // for (auto &i : search_stack){
    //     std::cout << "Stack: " << i->data_.name << std::endl;
    // }
    
    assignments_ = assignment_generator->generateAgentActionAssignments(nodes);

    createAssignmentNodes(nodes, search_stack);       

}

void NodeExpander::createAssignmentNodes(std::vector<Node*> & or_nodes, std::vector<Node*> & search_stack){

    BFS bfs;

    // std::cout << "ROOOT   " << search_tree_->root_->data_.name << std::endl;

    std::cout << "Iteration: " << iteration << std::endl;

    Graph<> * newone = bfs.run(search_tree_->root_, true);

    std::string filename = "Sub" + std::to_string(iteration) + ".dot";
    iteration++;

    // DotWriter writer(filename);
    // newone->print(writer);
    
    std::set<Node* > nodes_to_delete_;

    NodeData ndata;

    for (auto &cur_assignments : *assignments_){

        Graph<> temp(*newone);

        for (auto &agent_action_assignment : cur_assignments){   

            std::string agent  = std::get<0>(agent_action_assignment);
            std::string action = std::get<1>(agent_action_assignment);
            // Node * action_ptr  = std::get<2>(agent_action_assignment);
            
            // nodes_to_delete_.insert(action_ptr);

            // ndata.name   = action;
            // ndata.worker = agent;
            // ndata.expanded = true;
            // ndata.cost = cost_map_.map_[action][agent];
            // ndata.type = NodeType::AND;
            // ndata.marked = false;
            // ndata.solved = false;
           
            // Node * new_action_node = temp.insertNode(ndata);
            // search_tree_->insertEdge(0, action_ptr->getPredecessorNodes()[0]->id_, new_action_node->id_);

            // for (auto & or_successor : action_ptr->getSuccessorNodes()){


            //     nodes_to_delete_.insert(or_successor);
            //     Node * inserted = search_tree_->insertNode(*or_successor);
            //     inserted->data_.expanded = false;
                
            //     for (auto & follower : or_successor->getSuccessorNodes()){
            //         search_tree_->insertEdge(0, inserted->id_,  follower->id_);
            //     }
                
            //     search_tree_->insertEdge(0, new_action_node->id_, inserted->id_);
            // }
        }
    }

    // // std::cout << "LALALA" << std::endl;

    // // search_tree_->print();
    // for (auto & node_delete : nodes_to_delete_){
    //     // search_tree_->eraseEdge(node_delete->getPredecessorNodes()[0]->id_, node_delete->id_);        
    // }
}


#endif