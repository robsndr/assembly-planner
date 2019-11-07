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
    double total_cost;
    bool valid;
    std::vector< Node* > nodes_investigated;
};


/* Class representing the AOStar Search. Used as a functor - enables to store state.
**/
class AOStarSearch{
public:
    AOStarSearch();
    ~AOStarSearch();

    AOStarState operator()(Graph<> *, Node*);
    std::vector<Node *> nodesToExpand(Graph<> *, Node* );
    void expandNodes();


private:
    bool flagSolutionTree(Node * root);
    void reviseCosts();

    std::vector< Node* > stack_;

    std::vector< Node* > temp_or_nodes;
    Node* temp_and_node;

    std::vector< Node* > min_ors;
    Node* min_and;

};

AOStarSearch::AOStarSearch(){}

AOStarSearch::~AOStarSearch(){} 

/* Obtain node to be exapnded along the current search path. 
    Use class-wide data structures for efficiency reasons.
        @graph: graph object to perform the search on.
        @start: root node to begin the search at.
        \return: leaf node of the curectly marked subgraph (node to expand).
**/
std::vector<Node *> AOStarSearch::nodesToExpand(Graph<> * graph, Node* root){
    
    // Select non terminal leaf node in marked subtree.
    // Begin withh root.
    Node * current_node = root;

    stack_.clear();
    min_ors.clear();

    min_ors.push_back(root);

    // Propagate downwards through all CONNECTED and MARKED Nodes
    while(current_node && current_node->data_.marked){
        
        std::cout << "STUCL!";
        // If current node is terminal node skip propagation
        if(current_node->hasSuccessor() && !current_node->data_.terminal){

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
                    min_and = temp_and_node;
                    min_ors = temp_or_nodes;
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
                    min_and->data_.marked && !min_ors[j]->data_.terminal){
                    
                    current_node = min_ors[j];
                    break;
                }
            }
        } 
    }

    // std::cout << "MinOrs: " << std::endl;
    // for (auto const& x : min_ors){
    //     std::cout << "MinOR: " << x->data_.name << std::endl;
    // }

    std::cout << "MINORS SIZE: " << min_ors.size() << std::endl;

    return min_ors;

}


/* Exapnad set of nodes. Use class-wide data structures for efficiency reasons.
        @nodes: reference to set of nodes to expand.
**/
void AOStarSearch::expandNodes(){
    
    // Update cost of found leaf node.
    // For the non-terminal leaf-node of the marked subtree found in the last step update cost.
    for(std::size_t j = 0; j < min_ors.size(); j++){
        
        Node * n = min_ors[j];
        
        int final_cost = INT_MAX; // Set cost to a high value for the minimization
        
        std::cout << "Investigated MinOr: " << n->data_.name << std::endl;
        // If investigated OR-Node is a terminal node mark it solved. Otherwise update it's cost.
        if(!n->hasSuccessor()){
            n->data_.solved = true;
        }
        else{

            if(n->data_.terminal)
                n->data_.solved = true;
            // Search for the minimal successor - best cost and-node, set of or-nodes.
            // for(std::size_t i = 0; i < n->numberOfSuccessors(); i++){

            //     temp_and_node = n->getSuccessorNodes()[i];
            //     double and_cost = temp_and_node->data_.cost;
            //     temp_or_nodes = temp_and_node->getSuccessorNodes();

            //     // Calculate cost of i'th path
            //     double temp_cost = and_cost;
            //     for(std::size_t j=0;j<(temp_or_nodes.size());j++){
            //         temp_cost+=temp_or_nodes[j]->data_.cost;
            //     }

            //     // Update best-case cost.
            //     if(temp_cost < final_cost){
            //         final_cost = temp_cost;
            //     }
            // }

            // final_cost = ;

            // Update the investigated non-terminal leaf-node.
            // n->data_.cost = final_cost; // Update cost
            n->data_.marked = true;     // Mark Node because it becomes member of optimal tree.
            min_and->data_.marked = true; // Mark the connecting AND-Node. It is now member of the path.
        }
    }
}


/* Revise cost of nodes along the current marked path.
**/
void AOStarSearch::reviseCosts(){

    // Cost-Revision of Nodes along path.
    // The leaf-node cost could have changed in last step.
    while(!stack_.empty()){
        Node* n = stack_.back(); // stack keeps track of nodes present in the path so far.
        stack_.pop_back();

        std::cout << "On Stack: " << n->data_.name << std::endl;
        double final_cost = INT_MAX;

        // Update cost of every n which is a node on the path from root the leaf found in I.
        for(std::size_t i=0; i< n->numberOfSuccessors(); i++){
            temp_and_node = n->getSuccessorNodes()[i];
            int and_cost = temp_and_node->data_.cost;

            // The currently valid and node could have changed.
            // Make sure to de-mark the connecting and-nodes on the path.
            // The valid and_node is marked below...
            temp_and_node->data_.marked = false; 

            temp_or_nodes = temp_and_node->getSuccessorNodes();

            double temp_cost = temp_and_node->data_.cost;
            for(std::size_t j=0; j < temp_or_nodes.size(); j++){
                temp_cost += temp_or_nodes[j]->data_.cost;
            }

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
bool AOStarSearch::flagSolutionTree(Node * root){

    Node * current_node = root;
    stack_.clear();
    
    while(current_node && current_node->data_.solved){
    
        current_node->data_.solution = true;
        if(current_node->hasSuccessor() && !current_node->data_.terminal){
    
            for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){
                
                temp_and_node = current_node->getSuccessorNodes()[i];
                temp_or_nodes = temp_and_node->getSuccessorNodes();
                
                if(temp_and_node->data_.marked){
                    temp_and_node->data_.solution = true;
                    
                    std::cout << "Action:  " << temp_and_node->data_.name << std::endl;

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
AOStarState AOStarSearch::operator()(Graph<> * graph, Node * root){

    AOStarState search_state;
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

        // Walk down the least-cost path within the currently marked subgraph. 
        // Obtain set of nodes which have not been expanded/marked yet.
        nodesToExpand(graph, root);

        expandNodes();

        reviseCosts();        

        current_node=root;
    }

    // Mark nodes that are part of the optimal subtree.
    // flagSolutionTree(root);

    // Update search state.
    search_state.valid = true;
    search_state.total_cost = root->data_.cost;

    return search_state;
}
