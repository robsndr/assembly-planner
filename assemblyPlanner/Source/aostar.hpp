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
    
    std::vector< Node* > marked_;

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
bool AOStarSearch::operator()(Graph<> * graph, Node * start ){

    // Start node is terminating node and sole solution.
    Node * current_node;

    if(start->numberOfSuccessors() == 0){
        start->data_.solved = true;
    }

    start->data_.marked = true;


    while(start->data_.solved == false){










    }

    return true;
}
