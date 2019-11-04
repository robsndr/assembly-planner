#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include "graph.hpp"


/* Class representing the AOStarSearch State. 
    Used as a return value to the search function.
**/
class AOStarState {
public:
    int depth;
    double total_cost;
    bool valid;
    std::vector< Node* > nodes;
};


/* Class representing the AOStar Search. Used as a functor - enables to store state.
**/
class AOStarSearch{
public:
    AOStarSearch();
    ~AOStarSearch();

    AOStarState operator()(Graph<> *, Node*, int);

private:
    bool flagSolutionTree(Node * root);
    std::vector< Node* > min_ors;
    std::vector< Node* > stack_;
    Node* min_and;

};

AOStarSearch::AOStarSearch(){}

AOStarSearch::~AOStarSearch(){} 


/* Functor which triggers the AO* search.
    @graph: graph object to perform the search on.
    @start: root node to begin the search at.
    \return: true if successfull.
**/
AOStarState AOStarSearch::operator()(Graph<> * graph, Node * root, int depth_to_search = INT_MAX){

    AOStarState search_state;
    search_state.depth = 0;
    search_state.valid = false;
    search_state.total_cost = 0;
   
    Node * current_node = root;
    root->data_.marked = true;

    // Start node is terminating node and sole solution.
    if(!root->hasSuccessor()){
        root->data_.solved = true;
    }

    // Select a non-terminal leaf node from the marked sub-tree
    while(root->data_.solved == false){     

        // Select non terminal leaf node in marked subtree.
        // Begin withh root.
        current_node = root;
        stack_.clear();

        search_state.depth = 0;

        // Propagate downwards through all CONNECTED and MARKED Nodes
        while(current_node && current_node->data_.marked){
           
            // If current node is terminal node skip propagation
            if(current_node->hasSuccessor() && search_state.depth < depth_to_search){

                search_state.depth += 1; // we get one level deeper within the search.

                // Put the current node into the search stack. Is important later.
                // The stack is used for the last cost-revision step.
                stack_.push_back(current_node);
                
                double cost = INT_MAX; // Set the cost to minimize to a large initial value.

                // Search for the minimal successor - best cost and-node, set of or-nodes.
                for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){
                    Node* and_node = current_node->getSuccessorNodes()[i];
                    double and_cost = and_node->data_.cost;
                    std::vector<Node*> or_nodes = and_node->getSuccessorNodes();

                    // Calculate cost of i'th path
                    double temp_cost = and_node->data_.cost;
                    for(std::size_t j=0; j < or_nodes.size(); j++){
                        temp_cost += or_nodes[j]->data_.cost;
                    }

                    // Set the best and/or successors according to cost.
                    if(temp_cost < cost){
                        min_and = and_node;
                        min_ors = or_nodes;
                        cost = temp_cost;
                    }
                }
                
                current_node = NULL; // Make sure that NULL if nothing found in the next step. 
                                     // Terminates loop if nothing found in next step.

                // Propagate-Down Step.
                // Within the found 'min_ors" best cost successor nodes look for already marked ones.
                for(std::size_t j = 0; j < min_ors.size(); j++){
                    
                    // Check if given best-case successor is marked, not solved
                    // and that its connected to the graph thorugh a valid and-node.
                    if(min_ors[j]->data_.marked &&  min_ors[j]->data_.solved == false &&
                        min_and->data_.marked){
                        
                        current_node = min_ors[j];
                        break;
                    }
                }
            } 
        }


        // Update cost of found leaf node.
        // For the non-terminal leaf-node of the marked subtree found in the last step update cost.
        for(std::size_t j = 0; j < min_ors.size(); j++){
            
            Node * n = min_ors[j];
            
            int final_cost = INT_MAX; // Set cost to a high value for the minimization
            
            // If investigated OR-Node is a terminal node mark it solved. Otherwise update it's cost.
            if(!n->hasSuccessor() || search_state.depth == depth_to_search){
                n->data_.solved = true;
            }
            else{
                // Search for the minimal successor - best cost and-node, set of or-nodes.
                for(std::size_t i = 0; i < n->numberOfSuccessors(); i++){

                    Node* and_node = n->getSuccessorNodes()[i];
                    double and_cost = and_node->data_.cost;
                    std::vector<Node*> or_nodes = and_node->getSuccessorNodes();

                    // Calculate cost of i'th path
                    double temp_cost = and_cost;
                    for(std::size_t j=0;j<(or_nodes.size());j++){
                        temp_cost+=or_nodes[j]->data_.cost;
                    }

                    // Update best-case cost.
                    if(temp_cost < final_cost){
                        final_cost = temp_cost;
                    }
                }

                // Update the investigated non-terminal leaf-node.
                n->data_.cost = final_cost; // Update cost
                n->data_.marked = true;     // Mark Node because it becomes member of optimal tree.
                min_and->data_.marked = true; // Mark the connecting AND-Node. It is now member of the path.
            }
        }


        // Cost-Revision of Nodes along path.
        // The leaf-node cost could have changed in last step.
        while(!stack_.empty()){
            Node* n = stack_.back(); // stack keeps track of nodes present in the path so far.
            stack_.pop_back();

            double final_cost = INT_MAX;

            // Update cost of every n which is a node on the path from root the leaf found in I.
            for(std::size_t i=0; i< n->numberOfSuccessors(); i++){
                Node* and_node = n->getSuccessorNodes()[i];
                int and_cost = and_node->data_.cost;

                // The currently valid and node could have changed.
                // Make sure to de-mark the connecting and-nodes on the path.
                // The valid and_node is marked below...
                and_node->data_.marked = false; 

                std::vector<Node*> or_nodes = and_node->getSuccessorNodes();

                double temp_cost = and_node->data_.cost;
                for(std::size_t j=0; j < or_nodes.size(); j++){
                    temp_cost += or_nodes[j]->data_.cost;
                }

                if(temp_cost<final_cost){
                    min_and = and_node;
                    min_ors=or_nodes;
                    final_cost=temp_cost;
                }
            }

            // Mark the and-node which connects to the minimal solution.
            min_and->data_.marked = true;

            // If EACH successor OR-Node is labeled SOLVED, then label n as SOLVED.
            bool solved_flag = true;
            for(std::size_t j=0; j < min_ors.size(); j++){
                solved_flag &= min_ors[j]->data_.solved;
            }

            // Udpdate node data.
            n->data_.solved = solved_flag;
            n->data_.cost = final_cost;
        }
        current_node=root;
    }

    // Mark nodes that are part of the optimal subtree.
    flagSolutionTree(root);

    // Update search state.
    search_state.valid = true;
    search_state.total_cost = root->data_.cost;

    return search_state;
}

bool AOStarSearch::flagSolutionTree(Node * root){
    Node * current_node = root;
    stack_.clear();
    while(current_node && current_node->data_.solved){
        current_node->data_.solution = true;
        if(current_node->hasSuccessor()){
            for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){
                Node* and_node = current_node->getSuccessorNodes()[i];
                std::vector<Node*> or_nodes = and_node->getSuccessorNodes();
                if(and_node->data_.marked){
                    and_node->data_.solution = true;
                    std::cout << "Action:  " << and_node->data_.name << std::endl;

                    stack_.insert(std::end(stack_), std::begin(or_nodes), std::end(or_nodes));
                }
            }
        }
        // std::cout << "NODEEEEEE:" << current_node->data_.name << std::endl;
        current_node = stack_.front();
        if(!stack_.empty())
            stack_.erase(stack_.begin());
        else
            return true;
    }
}