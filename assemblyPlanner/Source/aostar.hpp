#include <iostream>
#include <set>
#include <vector>
#include "graph.hpp"

/* Class representing the AOStar Search. Used as a functor - enables to store state.
**/
class AOStarSearch{
public:
    AOStarSearch(/* args */);
    ~AOStarSearch();

    bool operator()(Graph<> *, Node* );

private:
    
    std::vector< Node* > min_ors;
    Node* min_and;
    std::vector< Node* > stack_;



};

AOStarSearch::AOStarSearch(/* args */)
{
}

AOStarSearch::~AOStarSearch()
{
} 



// template<typename edgeData, typename nodeData >
// bool AOStarSearch::getMinimalCostSuccessor()(Node<nodeData, edgeData> * start ){


//     return true;
// }


/* Functor which triggers the AO* search.
    @graph: graph object to perform the search on.
    @start: root node to begin the search at.
    \return: true if successfull.
**/
bool AOStarSearch::operator()(Graph<> * graph, Node * root ){
    Node * current_node = root;
    root->data_.marked = true;
    // Start node is terminating node and sole solution.
    if(!root->hasSuccessor()){
        root->data_.solved = true;
    }
    min_ors.push_back(root);

    while(root->data_.solved == false){     
        // Select non terminal leaf node in marked subtree.
        // Begin withh root.
        current_node = root;
        stack_.clear();

        // Walk downwards to the best solution if already in the graph.
        while(current_node && current_node->data_.marked){
            //Nonterminal node
            if(current_node->hasSuccessor()){
                stack_.push_back(current_node);
                double cost = INT_MAX;

                // std::cout << "Going Down:  " << current_node->data_.name << "Solved: " << current_node->data_.solved << std::endl;  

                for(std::size_t i=0; i<current_node->numberOfSuccessors(); i++){

                    // Obtain the i'th intermediate successor Node.
                    Node* and_node = current_node->getSuccessorNodes()[i];
                    double and_cost = and_node->data_.cost;

                    std::vector<Node*> or_nodes = and_node->getSuccessorNodes();

                    double temp_cost = and_node->data_.cost;
                    for(std::size_t j=0; j < or_nodes.size(); j++){
                        temp_cost += or_nodes[j]->data_.cost;
                    }
                    if(temp_cost<cost){
                        min_and = and_node;
                        min_ors = or_nodes;
                        cost = temp_cost;
                    }
                }

                // Select non-terminal leaf node from marked subtree.
                current_node=NULL;
                for(std::size_t j = 0; j < min_ors.size(); j++){
                    if(min_ors[j]->data_.marked && min_ors[j]->data_.solved == false &&
                        min_ors[j]->hasSuccessor() &&
                            min_and->data_.marked){
                        current_node = min_ors[j];
                        break;
                    }
                }

            }
            

        }

        // Exploring
        for(std::size_t j = 0; j < min_ors.size(); j++){
            Node * n = min_ors[j];
            // std::cout << "Exploring :" << n->data_.name << std::endl;
            
            int final_cost=INT_MAX;
            
            if(!n->hasSuccessor()){
                n->data_.solved = true;
            }
            else{
                for(std::size_t i = 0; i < n->numberOfSuccessors(); i++){

                    Node* and_node = n->getSuccessorNodes()[i];
                    double and_cost = and_node->data_.cost;

                    std::vector<Node*> or_nodes = and_node->getSuccessorNodes();

                    double temp_cost = and_cost;
                    for(std::size_t j=0;j<(or_nodes.size());j++){
                        temp_cost+=or_nodes[j]->data_.cost;
                    }
                    if(temp_cost<final_cost)
                    {
                        final_cost=temp_cost;
                    }
                }
                n->data_.cost=final_cost;
                n->data_.marked=true;
                min_and->data_.marked = true;
            }
            std::cout<<"Marked : "<<n->data_.name<<std::endl;
        }

        // for(int i=0; i<20; i++) std::cout<<"=";
        // std::cout << std::endl;


        while(!stack_.empty()){
            Node* n = stack_.back();
            stack_.pop_back();


            double final_cost=INT_MAX;
            for(std::size_t i=0; i< n->numberOfSuccessors(); i++){
                // Obtain the i'th intermediate successor Node.
                Node* and_node = n->getSuccessorNodes()[i];
                int and_cost = and_node->data_.cost;
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
            min_and->data_.marked = true;

            bool solved_flag = true;
            for(std::size_t j=0; j < min_ors.size(); j++){
                solved_flag &= min_ors[j]->data_.solved;
            }
            n->data_.solved = solved_flag;
            n->data_.cost = final_cost;
        }
        std::cout<<std::endl;
        current_node=root;
    }


    //PRINTING

    current_node = root;
    stack_.clear();
    while(current_node && current_node->data_.solved){
        // std::cout << "Search Found: " << current_node->data_.name << std::endl;
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

// void printMarkedTree(Graph<> * graph){

// }