#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

class Node
{
   public:

      Node *parent; // used during the search to record the parent of successor nodes
      std::vector< std::vector<Node*>> children; // used after the search for the application to view the search in reverse

      std::string subassembly;

      float g; // cost of this node + it's predecessors
      float h; // heuristic estimate of distance to goal
      float f; // sum of cumulative cost of predecessors and self and heuristic

      // UserState m_UserState;
};

Node * construct_tree(void){

   Node * root = new Node();
   Node * previous_node = root;
   
   
   std::vector<Node*> graph;
   for(int j=0; j<18; j++){
      Node * temp = new Node();
      graph.push_back(temp);
   }

   graph[0]->




   return root;
}

int main(void){

   return 0;
}