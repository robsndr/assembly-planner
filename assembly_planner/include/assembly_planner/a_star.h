#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <map>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include <boost/heap/fibonacci_heap.hpp>

//#include "utilities/header/time.hpp"

namespace algorithms{

namespace search{

class AStar_node
{
public:
    AStar_node();
    virtual ~AStar_node();

    virtual double heuristic(AStar_node* node)=0;
    virtual bool equalsNode(AStar_node* node)=0;
    virtual std::list<AStar_node*> getNeighbors()=0;
    virtual std::string getUniqueString() = 0;
    virtual double getActionCost(AStar_node* node) = 0;
    double getPathCost() const;
    double getTotalCost() const;
    double getHeuristicCost() const;
    void setHeuristicCost(double h);
    void setPathCost(double g);
    void setTotalCost(double f);
    AStar_node *getPredecessor();
    void setPredecessor(AStar_node *predecessor);

protected:

    double _g;
    double _h;
    double _f;

    AStar_node* _predecessor;

};

struct compare_openList
{
    bool operator()(const AStar_node* n1, const AStar_node* n2) const
    {
        return n1->getTotalCost() > n2->getTotalCost();
    }
};


class AStar
{
public:
    AStar();
    ~AStar();

    AStar_node* search(AStar_node* startNode,AStar_node* goalNode);

private:

    void writeReport(int t_elapsed);

    boost::heap::fibonacci_heap<AStar_node*, boost::heap::compare<compare_openList> > _openList;
    std::unordered_set<std::string> _closedList;
    std::unordered_map<std::string,boost::heap::fibonacci_heap<AStar_node*,boost::heap::compare<compare_openList> >::handle_type> _openMap;

};

}

}

#endif // ASTAR_H
