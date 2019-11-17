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

    void printAssignments();

private:

    void createAssignmentNodes(std::vector<Node*> & );

    void assignAgentsToActions(std::vector<std::string> & , std::vector<std::tuple<std::string, Node*> >  & );
    void generateAgentActionAssignments(std::vector<Node*> & );
    void generateActionCombinationSets(std::vector<Node*> & );
    void generateAgentCombinationSets( std::vector<std::string> &, int );

    Graph<> * search_tree_;
    CostMap cost_map_;

    std::unordered_map<std::string, std::vector<std::string> > node_actions_; // First entry denotes node.
                                                                             // Second entry denotes action.
    
    std::vector<std::string> temp_nodes;


    std::vector< std::vector< std::tuple<std::string, std::string, Node*> > > agent_action_assignements_;

    // Data Structures needed to calculate the agent combinations
    // Defines as class-wide objects to facilitate reuse between iterations.
    std::vector< std::vector<std::string> > temp_agent_combinations_;
    std::vector<std::string> temp_agent_set_;

    // Data Structures needed for the Action-Combination Generation.
    // Defined here to create them once and reuse without repeating allocation.
    std::vector< std::vector<  std::tuple<std::string, Node*> > > temp_action_combinations_;
    std::vector< std::tuple<std::string, Node*> > temp_action_set_;
    std::unordered_map<std::string, Node*> action_pointer_map_;


};


NodeExpander::NodeExpander(Graph<> * tree, CostMap & costs){
    search_tree_ = tree;
    cost_map_ = costs;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNodes(std::vector<Node*> & nodes){
    // Expand multiple nodes at once.
    std::cout << "NodeSize: " << nodes.size() << std::endl;

    generateAgentActionAssignments(nodes);

    printAssignments();        

}

void NodeExpander::createAssignmentNodes(std::vector<Node*> & or_nodes){

    // std::vector<Node*> actions;
    // for (auto &x : or_nodes){
    //     actions.push_back(x);
    // }

    
    // std::vector< std::unordered_map<std::string, std::string> > agent_action_assignements_;
    // NodeData rdata;


    // for (auto &assignment : agent_action_assignements_){
    //     for (auto &agent_action : assignment){
    //         std::string agent = agent_action.first;
    //         std::string action = agent_action.second;


    //         Node * action_node = action_pointer_map_[action];
            

    //         rdata.name   = action;
    //         rdata.worker = agent;
    //         rdata.expanded = true;
    //         rdata.cost = cost_map_.map_[action][agent];
    //         rdata.type = NodeType::AND;
    //         rdata.marked = false;
    //         rdata.solved = false;
    //         Node * new_action_node = search_tree_->insertNode( , rdata);

    //     }
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

            Node * new_node = search_tree_->insertNode(rdata);

            int source_id = and_node->getPredecessorNodes().front()->id_;
            int expanded_and_id = new_node->id_;

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
void NodeExpander::generateAgentActionAssignments(std::vector<Node*> & nodes){

    std::size_t l = std::min(nodes.size(), cost_map_.number_of_agents_);

    generateActionCombinationSets(nodes);

    agent_action_assignements_.clear();

    for (size_t j = 1; j <= l; j++){
        // std::cout << "Brun" << std::endl;

        generateAgentCombinationSets(cost_map_.vector_of_agents_, j);
        
        for (auto &agents : temp_agent_combinations_){
            for (auto &actions : temp_action_combinations_){
                assignAgentsToActions(agents, actions);
            }
        }

    }

}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::assignAgentsToActions(std::vector<std::string> & cur_agents, 
                                std::vector<  std::tuple<std::string, Node*> > & cur_actions ){
    
    int n = cur_actions.size();
    int k = cur_agents.size();

    std::vector<  std::tuple<std::string,std::string, Node*> > assignment;

    // Create selector vector
    std::vector<int> d(n);
    std::iota(d.begin(),d.end(),0);
    // std::cout << "These are the Possible Permutations: " << std::endl;
    do{
        for (int i = 0; i < k; i++){
            // std::cout << cur_actions[d[i]] << " ";
            assignment.push_back( std::make_tuple(cur_agents[i], 
                                                    std::get<0>(cur_actions[d[i]]), 
                                                        std::get<1>(cur_actions[d[i]])));
        }
        agent_action_assignements_.push_back(assignment);
        // std::cout << std::endl;
        assignment.clear();
        std::reverse(d.begin()+k,d.end());
    } while (next_permutation(d.begin(),d.end()));


}

/* Function which performs the generation of assignments of workers to actions.
**/
void NodeExpander::generateAgentCombinationSets( std::vector<std::string> & agents, int k){
    
    
    int n = agents.size();
    temp_agent_combinations_.clear();
    
    // Create selector vector
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + k, true);
    do {
        temp_agent_set_.clear();
        for (int i = 0; i < n; ++i) {
            if (v[i]) {
                // std::cout << agents[i] << " ";
                temp_agent_set_.push_back(agents[i]);
            }
        }
        temp_agent_combinations_.push_back(temp_agent_set_);
        // std::cout << "\n";
    } while (std::prev_permutation(v.begin(), v.end()));
}

/* Function which performs the generation the possible action combinations
**/
void NodeExpander::generateActionCombinationSets( std::vector<Node*> & nodes ){ 


    temp_action_combinations_.clear();

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
            Node* and_node = nodes[i]->getSuccessorNodes()[indices[i]];
            temp_action_set_.push_back(std::make_tuple(and_node->data_.name, and_node));
            // std::cout << temp_action_name << " "; 
        }
        temp_action_combinations_.push_back(temp_action_set_);
        // std::cout << std::endl; 
  
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

/* Debug functionality. Prints the current state of the assignment vector.
**/
void NodeExpander::printAssignments(){

    std::cout << std::endl << "***************Print current assignments: ********************" << std::endl;

    for (auto const& assignment : agent_action_assignements_){
        for (auto const&  elem: assignment){
            std::cout << std::get<0>(elem)  << " : " << std::get<1>(elem) << std::endl ;
        }
        std::cout << "----------" << std::endl; 
    }

    std::cout << "**************************************************************" << std::endl << std::endl;

}