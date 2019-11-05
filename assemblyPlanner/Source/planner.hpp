#include <iostream>
#include <unordered_map>
#include "aostar.hpp"

class ActionCostAssigner{
public:


private:
};

class Planner{
public:
    Planner();
    ~Planner();
    
    void operator()(Graph<> *, Node*, std::unordered_map<std::string, std::vector<int> >);

private:
    std::vector<Node *> nodes_to_expand;
    std::vector<Node *> min_ors;

};

Planner::Planner(){}

Planner::~Planner(){}


void Planner::operator()(Graph<> * graph, Node* root, std::unordered_map<std::string, std::vector<int> > cost_map){

    int a = 1;;
    while(a != 0){
        std::cin >> a; 
        AOStarSearch aostar;
        nodes_to_expand = aostar.nodesToExpand(graph, root);
        // nodes_to_expand.insert(std::end(nodes_to_expand), std::begin(min_ors), std::end(min_ors));
         
        graph->print();
        for (auto const  x : nodes_to_expand){
           std::cout << "TeRM: " << x->data_.name << std::endl;
           x->data_.terminal = false;
        }

        // graph->reset();
        aostar(graph, root);

        if(root->data_.solved)
            std::cout << "SOLVEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED";

    }

}