#include <vector>
#include "graph.hpp"
#include <set>
#include <algorithm>
#include <numeric>

class NodeExpander{
public:
    NodeExpander(Graph<> *, CostMap&);
    // void operator()(std::vector<Node*> &);
    void expandNode(Node * node);
    void expandNodes(std::vector<Node*> &);

    void createNodeActionTable(std::vector<Node*> &);

private:

    void assignAgentsToActions(std::vector<std::string> & agents, int k);
    void generateAgentActionAssignments(std::vector<Node*> & );
    void generateActionCombinationSets(std::vector<Node*> & );
    void generateAgentCombinationSets( std::vector<std::string> &, int );

    Graph<> * search_tree_;
    CostMap cost_map_;

    std::unordered_map<std::string, std::vector<std::string> > node_actions_; // First entry denotes node.
                                                                             // Second entry denotes action.
    std::vector<std::string> temp_nodes;



    std::vector< std::unordered_map<std::string, std::string> > agent_action_assignements_;

    // Data Structures needed to calculate the agent combinations
    // Defines as class-wide objects to facilitate reuse between iterations.
    std::vector< std::vector<std::string> > temp_agent_combinations_;
    std::vector<std::string> temp_agent_set_;

    // Data Structures needed for the Action-Combination Generation.
    // Defined here to create them once and reuse without repeating allocation.
    std::vector< std::vector<std::string> > temp_action_combinations_;
    std::vector<std::string> temp_action_set_;


};


NodeExpander::NodeExpander(Graph<> * tree, CostMap & costs){
    search_tree_ = tree;
    cost_map_ = costs;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNodes(std::vector<Node*> & nodes){
    // Expand multiple nodes at once.

    generateAgentActionAssignments(nodes);

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
void NodeExpander::generateAgentActionAssignments(std::vector<Node*> & nodes){

    int l = std::min(nodes.size(), cost_map_.number_of_agents_);

    generateActionCombinationSets(nodes);
    assignAgentsToActions(cost_map_.vector_of_agents_, 2);

    // for (size_t j = 0; j < l; j++){
    //     std::cout << "Brun" << std::endl;
    //     generateAgentCombinationSets(cost_map_.vector_of_agents_, j);
    // }



}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::assignAgentsToActions(std::vector<std::string> & agents, int k){
    
    int n = agents.size();

    // Create selector vector
    std::vector<int> d(n);
    std::iota(d.begin(),d.end(),0);
    std::cout << "These are the Possible Permutations: " << std::endl;
    do
    {
        for (int i = 0; i < k; i++){
            std::cout << agents[d[i]] << " ";
        }
        std::cout << std::endl;
        std::reverse(d.begin()+k,d.end());
    } while (next_permutation(d.begin(),d.end()));
}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::generateAgentCombinationSets( std::vector<std::string> & agents, int k){
    
    int n = agents.size();

    // Create selector vector
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + k, true);

    do {
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                std::cout << agents[i] << " ";
                temp_agent_set_.push_back(agents[i]);
            }
        }
        temp_agent_combinations_.push_back(temp_agent_set_);
        std::cout << "\n";
    } while (std::prev_permutation(v.begin(), v.end()));
    std::cout << "--------------" << std::endl;
}

/* Function which performs the generation the possible action combinations
**/
void NodeExpander::generateActionCombinationSets( std::vector<Node*> & nodes ){ 


    temp_action_combinations_.clear();
    std::string temp_action_name;

    // number of arrays 
    int n = nodes.size(); 
    int* indices = new int[n]; 
  
    // initialize with first element's index 
    for (int i = 0; i < n; i++) 
        indices[i] = 0; 
  
    while (1) { 
        temp_action_set_.clear();
        // print current combination 
        for (int i = 0; i < n; i++){
            temp_action_name = nodes[i]->getSuccessorNodes()[indices[i]]->data_.name;
            temp_action_set_.push_back(temp_action_name);
            std::cout << temp_action_name << " "; 
        }
        temp_action_combinations_.push_back(temp_action_set_);
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
            return; 
  
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
