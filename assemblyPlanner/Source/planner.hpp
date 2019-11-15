#include <iostream>
#include <unordered_map>
#include "aostar.hpp"
#include "bfs.hpp"

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

    // search_graph = new Graph<>();

    Graph<> * tree = bfs(root);
    Node * tree_root = tree->root_;

    NodeExpander expander(tree, cost_map);

    int a = 1;
    
    while(a != 0){
        
        AOStarSearch aostar;
        AOStarState state = aostar(tree_root, expander);

        for (auto node : state.solution_sequence){
            std::cout << "Action:  " << node->data_.name << std::endl;
        }
        

        std::cout << "Run next iteration? " << std::endl;
        std::cin >> a; 
    }

}