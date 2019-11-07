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
    std::vector<Node *> ors;

    Graph<> * search_graph;

};

Planner::Planner(){}

Planner::~Planner(){}


void Planner::operator()(Graph<> * graph, Node* root, std::unordered_map<std::string, std::vector<int> > cost_map){

    search_graph = new Graph<>();

    int a = 1;
    
    while(a != 0){
        
        AOStarSearch aostar;

        ors = aostar.nodesToExpand(graph, root);

        nodes_to_expand.insert(std::end(nodes_to_expand), std::begin(ors), std::end(ors));

        for (auto const  x : nodes_to_expand){
           x->data_.terminal = false;
        }
        
        while(a != 0){
            for (auto &i : ors){
                std::cout << "Current MinOr: " << i->data_.name << std::endl;
                for (auto &j : i->getSuccessorNodes()){
                    double action;
                    std::cout << "Input cost for action: " << j->data_.name << std::endl;
                    std::cin  >> action; 
                    j->data_.cost = action;
                }
                std::cout << "Expanded Node: " << i->data_.name << std::endl;
            }

            // graph->print();
            AOStarState state = aostar(graph, root);
            // graph->print();

            std::cout << "Iteration Cost: " << state.total_cost << std::endl;
            graph->reset();
            std::cout << "Update cost further? " << std::endl;
            std::cin >> a; 
        }
        
        

        // search_graph->insertNodes(nodes_to_expand);
        // for (auto const  x : nodes_to_expand){
        //     search_graph->insertNodes(x->getSuccessorNodes());
        // }

        // for (auto const  x : nodes_to_expand){
        //     if(x->hasSuccessor()){
        //         for (auto const  y : x->getSuccessorNodes()){
        //             search_graph->insertEdge(0, x->id_, y->id_);
        //         }
        //     }
        // }
        
        // search_graph->print();

        // nodes_to_expand.insert(std::end(nodes_to_expand), std::begin(min_ors), std::end(min_ors));
        // search_graph->print();
        // for (auto const  x : nodes_to_expand){
        //    std::cout << "TeRM: " << x->data_.name << std::endl;
        //    x->data_.terminal = false;
        // }

        std::cout << "Run next iteration? " << std::endl;
        std::cin >> a; 
    }

}