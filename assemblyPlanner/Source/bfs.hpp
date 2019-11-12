#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include "graph.hpp"


/* Class representing the AOStar Search. Used as a functor - enables to store state.
**/
class AOStarSearch{
public:
    AOStarSearch(Graph<> *);
    ~AOStarSearch();

    AOStarState operator()(Graph<> *, Node*);


private:
    std::vector<Node *> nodesToExpand(Graph<> *, Node* );
    void expandNodes();
    void expandNode(Node*);
    void reviseCosts();
    bool flagSolutionTree(Node *, AOStarState *);

    std::vector< Node* > stack_;

    std::vector< Node* > temp_or_nodes;
    Node* temp_and_node;

    std::vector< Node* > min_ors;
    Node* min_and;

    Graph<> * ao_graph;

};

AOStarSearch::AOStarSearch(Graph<> * graph){
    ao_graph = graph;
}

AOStarSearch::~AOStarSearch(){} 