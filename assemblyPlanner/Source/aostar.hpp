#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include <queue>
#include "expander.hpp"

/* Class representing the AOStarSearch State. 
    Used as a return value to the search function.
**/
class AOStarState {
public:
    double total_cost;
    bool valid;
    std::vector< Node* > solution_sequence;
};


/* Class representing the AOStar Search. Used as a functor - enables to store state.
**/
class AOStarSearch{
public:
    AOStarState operator()(Node*, NodeExpander&);


private:
    std::vector<Node *> walkMarkedSubtree(Node* );
    void expandNodes(NodeExpander&);
    void expandNode(Node*);
    void reviseCosts();
    bool flagSolutionTree(Node *, AOStarState *);

    std::vector< Node* > stack_;

    std::vector< Node* > temp_or_nodes;
    Node* temp_and_node;

    std::vector< Node* > min_ors;
    Node* min_and;

}; 

/* Obtain node to be exapnded along the current search path. 
    Use class-wide data structures for efficiency reasons.
        @graph: graph object to perform the search on.
        @start: root node to begin the search at.
        \return: leaf node of the curectly marked subgraph (node to expand).
**/
std::vector<Node *> AOStarSearch::walkMarkedSubtree(Node* start_node){
    
    // Select non terminal leaf node in marked subtree.
    // Begin withh root.
    Node * current_node = start_node;

    // stack_.clear();
    std::vector< Node* > ors_found;
    Node* and_found;

    std::queue< Node* > temp_storage;

    ors_found.clear();
    min_ors.clear();
    temp_storage.push(start_node);

    // Propagate downwards through all CONNECTED and MARKED Nodes
    while(!temp_storage.empty()){
        
        // std::cout << "On Stack: " << current_node->data_.name << std::endl;
        current_node = temp_storage.front();
        temp_storage.pop();
        // If current node is terminal node skip propagation
        if(current_node->hasSuccessor()){

            // Put the current node into the search stack. Is important later.
            // The stack is used for the last cost-revision step.
            stack_.push_back(current_node);
            
            double cost = INT_MAX; // Set the cost to minimize to a large initial value.

            // Search for the minimal successor - best cost and-node, set of or-nodes.
            for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){
                temp_and_node = current_node->getSuccessorNodes()[i];
                double and_cost = temp_and_node->data_.cost;
                temp_or_nodes = temp_and_node->getSuccessorNodes();

                // Calculate cost of i'th path
                double temp_cost = temp_and_node->data_.cost;
                for(std::size_t j=0; j < temp_or_nodes.size(); j++){
                    temp_cost += temp_or_nodes[j]->data_.cost;
                }

                // Set the best and/or successors according to cost.
                if(temp_cost < cost){
                    and_found = temp_and_node;
                    ors_found = temp_or_nodes;
                    cost = temp_cost;
                }
            }


            bool is_leaf = false;
            // Propagate-Down Step.
            // Within the found 'min_ors" best cost successor nodes look for already marked ones.
            for(std::size_t j = 0; j < ors_found.size(); j++){

                // Check if given best-case successor is marked, not solved
                // and that its connected to the graph thorugh a valid and-node.
                if(ors_found[j]->data_.marked &&  
                        ors_found[j]->data_.solved == false &&
                            and_found->data_.marked ){
                    is_leaf = true;
                    temp_storage.push(ors_found[j]);
                    // break;
                }
            }

            if(!is_leaf){
                min_ors.insert(std::end(min_ors), std::begin(ors_found), std::end(ors_found));
            }

        } 
    }

    std::cout << "---------" << std::endl;
    for (auto const& x : min_ors){
        std::cout << "MinOR: " << x->data_.name << std::endl;
        // std::cout << "Solved: " << x->data_.solved << std::endl;
        // std::cout << "Terminal: " << x->data_.terminal << std::endl;
    }

    return ors_found;
}


/* Exapnad set of nodes. Use class-wide data structures for efficiency reasons.
**/
void AOStarSearch::expandNodes(NodeExpander & expander){
    
    // Go through the best-case minOr nodes. Expand if possible. 
    // Mark nodes as they are part of the current best-path.
    // If a node is terminal, flag it solved so that it is not investigated further.
    temp_or_nodes.clear();

    for(std::size_t j = 0; j < min_ors.size(); j++){
        
        Node * n = min_ors[j];
        
        // If investigated OR-Node is a terminal node mark it solved. Otherwise update it's cost.
        if(!n->hasSuccessor()){
            n->data_.solved = true;
        }
        else{
            if(n->data_.expanded == false)
                temp_or_nodes.push_back(n);
            n->data_.marked = true;     // Mark Node because it becomes member of optimal tree.
        }
    }

    expander.expandNodes(temp_or_nodes);

}


/* Revise cost of nodes along the current marked path.
**/
void AOStarSearch::reviseCosts(){

    // Cost-Revision of Nodes along path.
    // The leaf-node cost could have changed in last step.
    while(!stack_.empty()){
        Node* n = stack_.back(); // stack keeps track of nodes present in the path so far.
        stack_.pop_back();

        // std::cout << "On Stack: " << n->data_.name << std::endl;
        double final_cost = INT_MAX;

        // Update cost of every n which is a node on the path from root the leaf found in I.
        for(std::size_t i=0; i< n->numberOfSuccessors(); i++){
            temp_and_node = n->getSuccessorNodes()[i];
            int and_cost = temp_and_node->data_.cost;

            // std::cout << n->data_.name << std::endl;
            // The currently valid and node could have changed.
            // Make sure to de-mark the connecting and-nodes on the path.
            // The valid and_node is marked below...
            temp_and_node->data_.marked = false; 

            temp_or_nodes = temp_and_node->getSuccessorNodes();

            double temp_cost = temp_and_node->data_.cost;
            // std::cout << "Calc: " << n->data_.name << "  And Cost: " << temp_cost << std::endl;

            for(std::size_t j=0; j < temp_or_nodes.size(); j++){
                temp_cost += temp_or_nodes[j]->data_.cost;
                // std::cout << "Calc: " << n->data_.name << "  Temp Cost: " << temp_cost << std::endl;
            }

            // Accept lowest-cost option.
            if(temp_cost<final_cost){
                min_and = temp_and_node;
                min_ors=temp_or_nodes;
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

}

/* Marks nodes of the graph which are part of the best-cost solution sub-graph.
    @start: root node to begin the to mark the found solution sub-graph.
    \return: true if successfull.
**/
bool AOStarSearch::flagSolutionTree(Node * root, AOStarState * search_state){

    Node * current_node = root;
    stack_.clear();
    
    while(current_node && current_node->data_.solved){
    
        current_node->data_.solution = true;
        if(current_node->hasSuccessor()){
    
            for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){
                
                temp_and_node = current_node->getSuccessorNodes()[i];
                temp_or_nodes = temp_and_node->getSuccessorNodes();
                
                if(temp_and_node->data_.marked){
                    temp_and_node->data_.solution = true;
                    
                    search_state->solution_sequence.push_back(temp_and_node);

                    stack_.insert(std::end(stack_), std::begin(temp_or_nodes), std::end(temp_or_nodes));
                }
            }
        }
        
        current_node = stack_.front();
        if(!stack_.empty())
            stack_.erase(stack_.begin());
        else
            return true;
    }
}


/* Functor which triggers the AO* search.
    @graph: graph object to perform the search on.
    @start: root node to begin the search at.
    \return: true if successfull.
**/
AOStarState AOStarSearch::operator()(Node * root, NodeExpander & expander){

    AOStarState search_state;
    search_state.valid = false;
    search_state.total_cost = 0;
   
    Node * current_node = root;
    root->data_.marked = true;

    // Start node is terminating node and sole solution.
    if(!root->hasSuccessor()){
        root->data_.solved = true;
    }

    expander.expandNode(root);

    // Select a non-terminal leaf node from the marked sub-tree
    while(root->data_.solved == false){     

        // Walk down the least-cost path within the currently marked subgraph. 
        // Obtain set of nodes which have not been expanded/marked yet.
        walkMarkedSubtree(root);

        expandNodes(expander);

        reviseCosts();        

        current_node=root;
    }

    // Mark nodes that are part of the optimal subtree.
    flagSolutionTree(root, &search_state);

    search_state.valid = true;
    search_state.total_cost = root->data_.cost;

    return search_state;
}