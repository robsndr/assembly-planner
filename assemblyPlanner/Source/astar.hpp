#pragma once

#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include <queue>

#include "expander.hpp"

/* Comparator function.
    Used to sort the priosirty queue inside AStarSearch.
    Orders elements by ascending f_score.
**/
struct LessThan
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return (lhs->data_.f_score) > (rhs->data_.f_score);
    }
};

/* Class representing the A* Search Algorithm.
    It executes the search on a given graph.
    The provided Exapander-Object is used to perform the Node-Expansion step.
**/
class AStarSearch
{
private:
    /* data */
public:
    // Constructor, Destructor
    AStarSearch();
    ~AStarSearch();

    // Search function
    Node *search(Graph<> *, Node *, NodeExpander *);
};

/* Constructor
**/
AStarSearch::AStarSearch(/* args */)
{
}

/* Destructor
**/
AStarSearch::~AStarSearch()
{
}

/* Perform the A* graph search.
    @graph: pointer to graph on which the search should be performed.
    @root: pointer to node at which the search should begin.
    @exapnder: exapnder object used for node expansion.
**/
Node *AStarSearch::search(Graph<> *graph, Node *root, NodeExpander *expander)
{

    Node *current = nullptr;
    std::priority_queue<Node *, std::vector<Node *>, LessThan> openSet;

    // Closed-Set is redundant as the search is performed on a acyclic graph where every path is unique.
    // -> Nodes can only be reached in one way.
    // Not using the closed-set saves some processing time needed to lookuo-stuff.
    // NodeSet closedSet; 
    expander->expandNode(root);
    root->data_.calc_hscore();
    root->data_.calc_fscore();

    openSet.push(root);

    while (!openSet.empty())
    {
        current = openSet.top();
        openSet.pop();

        if (current->data_.isGoal())
        {
            return current;
        }
        
        current->data_.marked = true;

        for (auto &edge : current->getSuccessors())
        {
            Node *child = edge->getDestination();
            expander->expandNode(child);

            child->data_.g_score = current->data_.g_score + edge->data_.cost;
            child->data_.calc_hscore();
            child->data_.calc_fscore();

            openSet.push(child);
        }
    }
    return current;
}