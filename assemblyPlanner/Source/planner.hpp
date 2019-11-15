#include <iostream>
#include <unordered_map>
#include "aostar.hpp"
#include "bfs.hpp"

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

    // search_graph = new Graph<>();

    Node * tree_root = BFS(root);
    // Node * tree_root = root;


    int a = 1;
    
    while(a != 0){
        
        AOStarSearch aostar(graph);
        AOStarState state = aostar(graph, tree_root);

        for (auto node : state.solution_sequence){
            std::cout << "Action:  " << node->data_.name << std::endl;
        }
        

        std::cout << "Run next iteration? " << std::endl;
        std::cin >> a; 
    }

}