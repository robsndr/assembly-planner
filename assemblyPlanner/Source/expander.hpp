#include <vector>
#include "graph.hpp"


class NodeExpander{
public:
    NodeExpander(Graph<> *, CostMap&);
    // void operator()(std::vector<Node*> &);
    void expandNode(Node * node);
    void expandNodes(std::vector<Node*> &);

    void createNodeActionTable(std::vector<Node*> &);
    void generateAssignments();

private:
    Graph<> * search_tree_;
    CostMap cost_map_;

    std::unordered_map<std::string, std::vector<std::string> > node_actions_; // First entry denotes node.
                                                                             // Second entry denotes action.

};


NodeExpander::NodeExpander(Graph<> * tree, CostMap & costs){
    search_tree_ = tree;
    cost_map_ = costs;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNodes(std::vector<Node*> & nodes){
    // Expand multiple nodes at once.

    createNodeActionTable(nodes);

    // for( auto &node : nodes){
    //     // expandNode(node);
    // }
}

/* Function which performs the node expansion on a single OR.
**/
void NodeExpander::expandNode(Node* node){

    // Expand children and-nod es.
    for (auto & and_node  : node->getSuccessorNodes()){
        
        std::cout << "Expanding action " << and_node->data_.name << std::endl;

        std::size_t expansion_dimension;
        std::cout << "Expansion dimension: "; std::cin >> expansion_dimension;

        for (size_t i = 0; i < expansion_dimension; i++){
            NodeData rdata;
            
            // Appending the string. 
            rdata.name = and_node->data_.name;
            rdata.type = and_node->data_.type;
            rdata.marked = false;
            rdata.solved = false;

            std::cout << "Input cost for Action. Node: " << and_node->data_.name << std::endl;
            std::cout << "Cost: "; std::cin >> rdata.cost;

            search_tree_->insertNode(and_node->id_ * 100 + i , rdata);

            int source_id = and_node->getPredecessorNodes().front()->id_;
            int expanded_and_id = and_node->id_ * 100 + i;

            std::cout << "source_id " << source_id << std::endl;
            std::cout << "destination_id " << source_id << std::endl;


            search_tree_->insertEdge( 0, source_id, expanded_and_id);

            for (auto & successor_or  : and_node->getSuccessorNodes()){
                search_tree_->insertEdge( 0, expanded_and_id, successor_or->id_);
            }

            search_tree_->print();

        }
    }
    node->data_.expanded = true;
}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::createNodeActionTable(std::vector<Node*> & nodes){

    for( auto &node : nodes){
        for (auto & and_node  : node->getSuccessorNodes()){
            std::cout << "Node: " << node->data_.name << " Action: " << and_node->data_.name << std::endl;
            node_actions_[node->data_.name].push_back(and_node->data_.name);
        }
    }
    
}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::generateAssignments( ){

    // int number_of_workers = ;

    // for( auto &node : nodes){
    //     for (auto & and_node  : node->getSuccessorNodes()){
    //         std::cout << "Node: " << node->data_.name << " Action: " << and_node->data_.name << std::endl;
    //         node_actions_[node->data_.name].push_back(and_node->data_.name);
    //     }
    // }
    
}