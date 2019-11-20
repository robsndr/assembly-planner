#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <vector>
#include "combinator.hpp"
#include "graph.hpp"
#include <set>
#include "helpers.hpp"

class NodeExpander{

public:
    NodeExpander(Graph<> *, CostMap&);
    ~NodeExpander();

    void expandNode(Node*);

private:
    void createAssignmentNodes(std::vector<Node*> &);

    Graph<> * search_tree_;
    CostMap cost_map_;
    Combinator * assignment_generator;
    std::vector< std::vector< std::tuple<std::string, std::string, Node*> > > * assignments_;

    std::size_t iteration = 0;
};


NodeExpander::NodeExpander(Graph<> * tree, CostMap & costs){
    search_tree_ = tree;
    cost_map_ = costs;
    assignment_generator = new Combinator(costs);
}

NodeExpander::~NodeExpander(){
    delete assignment_generator;
}

/* Function which performs the node expansion.
**/
void NodeExpander::expandNode(Node* node){

    // for (auto &i : search_stack){
    //     std::cout << "Stack: " << i->data_.name << std::endl;
    // }
    
    // assignments_ = assignment_generator->generateAgentActionAssignments(nodes);

    // createAssignmentNodes(nodes);       

}

void NodeExpander::createAssignmentNodes(std::vector<Node*> & or_nodes){

}


#endif