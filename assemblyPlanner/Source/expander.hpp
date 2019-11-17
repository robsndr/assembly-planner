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
    std::vector< std::vector<std::string> > generateActionCombinationSets(std::vector<Node*> & );
    void NodeExpander::generateAgentCombinationSets( std::vector<std::string>, int );

private:
    Graph<> * search_tree_;
    CostMap cost_map_;

    std::unordered_map<std::string, std::vector<std::string> > node_actions_; // First entry denotes node.
                                                                             // Second entry denotes action.
    std::vector<std::string> temp_nodes;

};


NodeExpander::NodeExpander(Graph<> * tree, CostMap & costs){
    search_tree_ = tree;
    cost_map_ = costs;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNodes(std::vector<Node*> & nodes){
    // Expand multiple nodes at once.

    // createNodeActionTable(nodes);

    generateActionCombinationSets(nodes);

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

    node_actions_.clear();
    temp_nodes.clear();

    for( auto &node : nodes){
        for (auto & and_node  : node->getSuccessorNodes()){
            // std::cout << "Node: " <<  node->data_.name << " Action: " << and_node->data_.name << std::endl;
            temp_nodes.push_back(node->data_.name);
            node_actions_[node->data_.name].push_back(and_node->data_.name);
        }
    }
    
}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::generateAssignments( ){
    
}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::generateAgentCombinationSets( std::vector<std::string> agents, int k){
    
}

/* Function which performs the generation the possible action combinations
**/
std::vector< std::vector<std::string> > NodeExpander::generateActionCombinationSets( std::vector<Node*> & nodes ){ 

    std::vector< std::vector<std::string> > action_sets;
    std::vector<std::string> temp_action_set;
    std::string temp_action_name;

    // number of arrays 
    int n = nodes.size(); 
    int* indices = new int[n]; 
  
    // initialize with first element's index 
    for (int i = 0; i < n; i++) 
        indices[i] = 0; 
  
    while (1) { 
  
        // print current combination 
        for (int i = 0; i < n; i++){
            temp_action_name = nodes[i]->getSuccessorNodes()[indices[i]]->data_.name;
            temp_action_set.push_back(temp_action_name);
            std::cout << temp_action_name << " "; 
        }
        action_sets.push_back(temp_action_set);
        std::cout << std::endl; 
  
        // find the rightmost array that has more 
        // elements left after the current element  
        // in that array 
        int next = n - 1; 
        while (next >= 0 &&  
              (indices[next] + 1 >= nodes[next]->numberOfSuccessors())) 
            next--; 
  
        // no such array is found so no more  
        // combinations left 
        if (next < 0) 
            return action_sets; 
  
        // if found move to next element in that  
        // array 
        indices[next]++; 
  
        // for all arrays to the right of this  
        // array current index again points to  
        // first element 
        for (int i = next + 1; i < n; i++) 
            indices[i] = 0; 
    } 
} 
