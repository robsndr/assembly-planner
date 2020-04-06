#pragma once

#include <iostream>
#include <queue>

#include "expander.hpp"

// Comparator used to sort the A* priotity queue
struct LessThan
{
    bool operator()(const Node<SearchData> *lhs, const Node<SearchData> *rhs) const
    {
        return (lhs->data.f_score) > (rhs->data.f_score);
    }
};


// Class used to execute the search on a given graph.
// The expander is used to perform the node-expansion step of the hypergraph.
struct AStarSearch
{
    AStarSearch(Graph<AssemblyData,EdgeData>& assembly);

    Node<SearchData>* search(Graph<SearchData,EdgeData>&, Node<SearchData>*, NodeExpander&);
    bool isGoal(Graph<AssemblyData,EdgeData>&, Node<SearchData>*);
    double calc_hscore(Node<SearchData>* current);
    double calc_fscore(Node<SearchData>* current);

    Graph<AssemblyData,EdgeData>& assembly_;
};

// Check if given sueprnode is Goal.
bool AStarSearch::isGoal(Graph<AssemblyData,EdgeData>& graph, Node<SearchData>* current)
{
    for (auto &x : current->data.subassemblies)
    {
        if (graph.hasSuccessor(x.second))
        {
            return false;
        }
    }
    return true;
}

double AStarSearch::calc_hscore(Node<SearchData>* current)
{
    std::size_t maximum_length_subassembly = 0;
    for (auto &x : current->data.subassemblies)
    {
        auto node = assembly_.getNode(x.second);
        if (node->data.name.length() > maximum_length_subassembly)
            maximum_length_subassembly = node->data.name.length();
    }
    
    double temp = log2f(maximum_length_subassembly) * current->data.minimum_cost_action;
    return temp;
}

double AStarSearch::calc_fscore(Node<SearchData>* current)
{
    return current->data.g_score + current->data.h_score;
}


AStarSearch::AStarSearch(Graph<AssemblyData,EdgeData>& assembly)
  : assembly_(assembly)
{}

// Perform the graph search:
//   @graph: pointer to graph on which the search should be performed.
//   @root: pointer to node at which the search should begin.
//   @exapnder: exapnder object used for node expansion.
//
Node<SearchData>* AStarSearch::search(Graph<SearchData,EdgeData>& graph, 
                                Node<SearchData>* root, NodeExpander& expander)
{

    Node<SearchData> *current = nullptr;
    std::priority_queue<Node<SearchData> *, std::vector<Node<SearchData> *>, LessThan> openSet;

    // Closed set is redundant as the search is performed on a acyclic graph where every path is unique.
    // Nodes can only be reached in one way. Not using the closed-set saves some time used for lookups.
    expander.expandNode(root->id);
    root->data.h_score  = this->calc_hscore(root);
    root->data.f_score = this->calc_fscore(root);

    openSet.push(root);

    while (!openSet.empty())
    {
        current = openSet.top();
        openSet.pop();

        if (this->isGoal(assembly_, current))
        {
            return current;
        }
        
        current->data.marked = true;

        for (auto &edge : graph.getSuccessorEdges(current->id))
        {
            Node<SearchData> *child = graph.getNode(edge->getDestination());
            expander.expandNode(child->id);

            child->data.g_score = current->data.g_score + edge->data.cost;
            child->data.h_score = this->calc_hscore(child);
            child->data.f_score = this->calc_fscore(child);

            openSet.push(child);
        }
    }
    return current;
}